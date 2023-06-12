#include "buffer/vec_buffer.h"

VecBuffer::VecBuffer() : buffer_(nullptr), current_buffer_size_(0), write_index_(0), read_index_(0), enlarge_count_(0) {
}

VecBuffer::~VecBuffer() {
  if (buffer_) {
    delete buffer_;
    buffer_ = nullptr;
  }
}

void VecBuffer::Init() {
  buffer_ = new char[init_buffer_size_];
  current_buffer_size_ = init_buffer_size_;
}

void VecBuffer::SetCallback(std::function<int(char*, int)> func) {
  on_recv_data_ = func;
}

int VecBuffer::ReadFromFd(int fd) {
  int left_data_size = current_buffer_size_ - write_index_;
  char cur_buffer[init_buffer_size_];
  memset(cur_buffer, 0, init_buffer_size_);
  struct iovec iov[2];
  iov[0].iov_base = buffer_ + write_index_;
  iov[0].iov_len = left_data_size;
  iov[1].iov_base = cur_buffer;
  iov[1].iov_len = init_buffer_size_;
  int n_read = readv(socket_fd_, iov, 2);
  if (n_read < 0) {
    if (errno != EAGAIN) {
      //log fatal
    } else {
      //just quit
    }
    return;
  }

  if (n_read > left_data_size) {
    EnlargeBufferAndMoveData(cur_buffer, n_read - left_data_size);
  } else {
    write_index_ += n_read;
  }
  read_index_ += on_recv_data_(buffer_ + read_index_, write_index_ - read_index_);
  if (read_index_ == write_index_) {
    write_index_ = 0;
    read_index_ = 0;
  }
}

int VecBuffer::SaveData(char* data, int data_len) {
  int left_data_size = current_buffer_size_ - write_index_;
  if (left_data_size < data_len) {
    memmove(buffer_ + write_index_, data, left_data_size);
    EnlargeBufferAndMoveData(data + left_data_size, data_len - left_data_size);
  } else {
    memmove(buffer_ + write_index_, data, data_len);
  }
}

int VecBuffer::WriteToFd(int fd) {
  int left_data_size = write_index_ - read_index_;
  struct iovec iov;
  iov.iov_base = buffer_ + read_index_;
  iov.iov_len = left_data_size;
  int n_write = writev(socket_fd_, iov, 1);
  if (n_write < 0) {
    if (errno != EAGAIN) {
      //log fatal
    } else {
      //just quit
    }
    return;
  }

  read_index_ += n_write;
  if (read_index == write_index_) {
    write_index_ = 0;
    read_index_ = 0;
  }
}

char* VecBuffer::GetReadIndex() {
  return buffer_;
}

void VecBuffer::EnlargeBufferAndMoveData(char* new_data, int new_data_len) {
  char* new_buffer = new char[current_buffer_size_ * 2];
  if (new_buffer_ == nullptr) {
    //log fatal
    return;
  }
  memmove(new_buffer_, buffer_, current_buffer_size_);
  memmove(new_buffer + current_buffer_size_, new_data, new_data_len);

  delete buffer_;
  buffer_ = new_buffer;
  write_index_ = current_buffer_size_ + new_data_len;
  current_buffer_size_ = current_buffer_size_ * 2;
  enlarge_count_++;
}
