#include "tcp/tcp_connection.h"

TcpConnection::TcpConnection() : socket_fd_(-1), input_buffer_(nullptr), output_buffer_(nullptr), write_index_(0), read_index_(0), latest_message_len_(-1) {
}

TcpConnection::TcpConnection(int sock_fd) : socket_fd_(sock_fd), input_buffer_(nullptr), output_buffer_(nullptr), write_index_(0), read_index_(0), latest_message_len_(-1) {
}

TcpConnection::~TcpConnection() {
  if (input_buffer_) {
    delete input_buffer_;
    input_buffer_ = nullptr;
  }
  if (output_buffer_) {
    delete output_buffer_;
    output_buffer_ = nullptr;
  }
}

TcpConnection::Init() {
  input_buffer_ = new char[buffer_size_];
  output_buffer_ = new char[buffer_size_];
}

void TcpConnection::SetCallback(std::function<char*> message_callback, std::function<void()> write_callback) {
  OnMessage = message_callback;
  OnWriteFinish = write_callback;
}

int TcpConnection::Read() {
  if (input_buffer_full_) {
    return -1;
  }

  struct iovec iov[2];
  if (read_index_ > write_index_) {
    iov[0].iov_base = input_buffer_[write_index_];
    iov[0].iov_len = read_index_ - write_index_;
    iov[1].iov_base = nullptr;
    iov[1].iov_len = 0;
    int n_read = readv(socket_fd_, iov, 2);
    if (n_read == read_index_ - write_index_) {
      input_buffer_full_ = true;
    }
    write_index_ += n_read;
  } else if (read_index_ < write_index_) {
    iov[0].iov_base = input_buffer_[write_index_];
    iov[0].iov_len = buffer_size_ - write_index_;
    iov[1].iov_base = input_buffer_;
    iov[1].iov_len = read_index_;
  }
}

void TcpConnection::Write() {
}






