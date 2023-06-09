#include "buffer/ring_buffer.h"

RingBuffer::RingBuffer(): buf_(nullptr), write_index_(0), read_index_(0), total_handle_bytes(0), buffer_full_(false), buffer_empty_(true) {
}

RingBuffer::~RingBuffer() {
  if (buf_ != nullptr) {
    delete buf_;
    buf_ = nullptr;
  }
}

RingBuffer::Init() {
  buf_ = new char[buffer_size_];
}
char* RingBuffer::GetBuffer() {
  return buf_;
}
int RingBuffer::GetWriteIndex() {
  return write_index_;
}
int RingBuffer::GetReadIndex() {
  return read_index_;
}
bool RingBuffer::BufferFull() {
  return buffer_full_;
}

bool RingBuffer::BufferEmpty() {
  return buffer_empty_;
}

uint64_t RingBuffer::GetTotalHandleBytes() {
  return total_handle_bytes_;
}

int RingBuffer::GetFreeBytes() {
  if (buffer_full_) {
    return 0;
  }
  if (buffer_empty_) {
    return buffer_size_;
  }
  if (read_index_ < write_index_) {
    return buffer_size_ - write_index_ + read_index_;
  }
  return write_index_ - read_index_;
}

int RingBuffer::GetUsedBytes() {
  if (buffer_full_) {
    return buffer_size_;
  }
  if (buffer_empty_) {
    return 0;
  }
  if (read_index_ > write_index_) {
    return buffer_size_ - read_index_ + write_index_;
  }
  return write_index_ - read_index_;
}

int RingBuffer::ReadFromFd(int fd) {
  if (BufferFull()) {
    return -1;
  }
}

int RingBuffer::WriteToFd(int fd) {
}





