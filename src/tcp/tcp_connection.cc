#include "tcp/tcp_connection.h"

TcpConnection::TcpConnection() : socket_fd_(-1), input_buffer_(nullptr), output_buffer_(nullptr), write_index_(0), read_index_(0), latest_message_len_(0), total_read_bytes_(0), input_buffer_full_(false), input_buffer_empty_(true), output_buffer_full_(false), output_buffer_empty_(true) {
}

TcpConnection::TcpConnection(int sock_fd) : socket_fd_(sock_fd), input_buffer_(nullptr), output_buffer_(nullptr), write_index_(0), read_index_(0), latest_message_len_(0), total_read_bytes_(0), input_buffer_full_(false), input_buffer_empty_(true), output_buffer_full_(false), output_buffer_empty_(true) {
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
  LOG_INFO("Total Read Bytes: %ld", total_read_bytes_);
}

void TcpConnection::Init() {
  input_buffer_ = new char[buffer_size_];
  output_buffer_ = new char[buffer_size_];
}

void TcpConnection::SetCallback(std::function<void(char*)> message_callback, std::function<void()> write_callback) {
  OnMessage = message_callback;
  OnWriteFinish = write_callback;
}

int TcpConnection::Read() {
  if (input_buffer_full_) {
    return -1;
  }

  int n_read;
  if (read_index_ > write_index_) {
    struct iovec iov;
    iov.iov_base = input_buffer_ + write_index_;
    iov.iov_len = read_index_ - write_index_;
    if ((n_read = readv(socket_fd_, &iov, 1)) > 0) {
      if (n_read == read_index_ - write_index_) {
        input_buffer_full_ = true;
      }
      write_index_ += n_read;
    }
  } else if (read_index_ <= write_index_) {
    struct iovec iov[2];
    iov[0].iov_base = input_buffer_ + write_index_;
    iov[0].iov_len = buffer_size_ - write_index_;
    iov[1].iov_base = input_buffer_;
    iov[1].iov_len = read_index_;
    n_read = readv(socket_fd_, iov, 2);
    if (n_read > 0) {
      if (n_read == buffer_size_ - write_index_ + read_index_) {
        input_buffer_full_ = true;
        write_index_ = read_index_;
      } else if (n_read < buffer_size_ - write_index_) {
        write_index_ += n_read;
      } else if (n_read >= buffer_size_ - write_index_) {
        write_index_ = n_read - (buffer_size_ - write_index_);
      }
    }
  }
  LOG_INFO("read_index: %d, write_index: %d", read_index_, write_index_);
  LOG_INFO("n_read: %d", n_read);
  if (n_read > 0) {
    input_buffer_empty_ = false;
    total_read_bytes_ += n_read;
  }
  //read_buffer_->ReadFromFd(socket_fd_);
  //if (read_buffer_->GetConsumableNum() > 0) {
  //
  //}
  return n_read;
}

void TcpConnection::ExtractMessageFromInput() {
  while (true) {
    if (input_buffer_empty_) {
      return;
    }

    //read message len first
    if (latest_message_len_ == 0 && input_data_bytes_count() < INT_SIZE) {
      return;
    }
    if (latest_message_len_ == 0 && input_data_bytes_count() >= INT_SIZE) {
      char cur_buf[INT_SIZE];
      if (buffer_size_ - read_index_ < INT_SIZE) {
        int left_size = buffer_size_ - read_index_;
        memcpy(cur_buf, input_buffer_ + read_index_, left_size);
        memcpy(cur_buf + left_size, input_buffer_, INT_SIZE - left_size);
        read_index_ = INT_SIZE - left_size;
      } else {
        memcpy(cur_buf, input_buffer_ + read_index_, INT_SIZE);
        read_index_ += INT_SIZE;
      }
      uint32_t cur;
      memcpy(&cur, cur_buf, INT_SIZE);
      latest_message_len_ = ntohl(cur);
      if (read_index_ == write_index_) {
        input_buffer_empty_ = true;
      }
    }
    if (input_buffer_empty_) {
      return;
    }

    // current data is not enough to construct one message
    if (latest_message_len_ > input_data_bytes_count()) {
      return;
    } else { //has enough data for constuction
      char message[latest_message_len_];
      if (buffer_size_ - read_index_ < latest_message_len_) {
        int left_size = buffer_size_ - read_index_;
        memcpy(message, input_buffer_ + read_index_, left_size);
        memcpy(message + left_size, input_buffer_, latest_message_len_ - left_size);
        read_index_ = latest_message_len_ - left_size;
      } else {
        memcpy(message, input_buffer_ + read_index_, latest_message_len_);
        read_index_ += latest_message_len_;
      }
      if (read_index_ == write_index_) {
        input_buffer_empty_ = true;
      }
      QueueMessage(message, latest_message_len_);
      latest_message_len_ = 0;
    }
  }
}

void TcpConnection::QueueMessage(char* message, int message_len) {
  uint32_t req_type_no;
  memcpy(&req_type_no, message, INT_SIZE);
  uint32_t real_request_type = ntohl(req_type_no);
  LOG_INFO("message_len is: %d, req_type: %d", message_len, real_request_type);
}

int TcpConnection::input_free_bytes_count() {
  if (input_buffer_full_) {
    return 0;
  }
  if (input_buffer_empty_) {
    return buffer_size_;
  }
  if (read_index_ < write_index_) {
    return buffer_size_ - write_index_ + read_index_;
  }
  return read_index_ - write_index_;
}

int TcpConnection::input_data_bytes_count() {
  if (input_buffer_full_) {
    return buffer_size_;
  }
  if (input_buffer_empty_) {
    return 0;
  }
  if (read_index_ > write_index_) {
    return buffer_size_ - read_index_ + write_index_;
  }
  return write_index_ - read_index_;
}

int TcpConnection::output_data_bytes_count() {
  return 0;
}

int TcpConnection::output_free_bytes_count() {
  return 0;
}

int TcpConnection::Write(char* buf, int) {
  return 0;
}
