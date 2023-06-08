#include "tcp/buffer.h"

CircleBuffer::CircleBuffer(): buf_(nullptr), write_index_(0), read_index_(0), total_handle_bytes(0), buffer_full_(false), buffer_empty_(true) {
}

CircleBuffer::~CircleBuffer() {
  if (buf_ != nullptr) {
    delete buf_;
    buf_ = nullptr;
  }
}

CircleBuffer::Init() {
  buf_ = new char[buffer_size_];
}
char* CircleBuffer::GetBuffer() {
  return buf_;
}
int CircleBuffer::GetWriteIndex() {
  return write_index_;
}
int CircleBuffer::GetReadIndex() {
  return read_index_;
}
bool CircleBuffer::IsBufferFull() {
  return buffer_full_;
}

bool CircleBuffer::IsBufferEmpty() {
  return buffer_empty_;
}

uint64_t CircleBuffer::GetTotalHandleBytes() {
  return total_handle_bytes_;
}

int CircleBuffer::GetFreeBytes() {
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

int CircleBuffer::GetUsedBytes() {
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





