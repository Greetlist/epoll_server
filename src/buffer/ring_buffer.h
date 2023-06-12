#ifndef __RING_BUFFER_H_
#define __RING_BUFFER_H_

#include <stdint.h>

class RingBuffer {
public:
  RingBuffer();
  ~RingBuffer();
  void Init();
  
  inline char* GetBuffer();
  inline int GetWriteIndex();
  inline int GetReadIndex();
  inline bool BufferFull();
  inline bool BufferEmpty();
  inline uint64_t GetTotalHandleBytes();
  inline int GetFreeBytes();
  inline int GetUsedBytes();
  int WriteToFd(int);
  int ReadFromFd(int);
private:
  char* buf_;
  int write_index_;
  int read_index_;
  uint64_t total_handle_bytes_;
  bool buffer_full_;
  bool buffer_empty_;
  static constexpr int buffer_size_ = 65536;
};

#endif
