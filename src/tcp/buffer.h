#ifndef __BUFFER_H_
#define __BUFFER_H_

class CircleBuffer {
public:
  CircleBuffer();
  ~CircleBuffer();
  void Init();
  
  inline char* GetBuffer();
  inline int GetWriteIndex();
  inline int GetReadIndex();
  inline bool IsBufferFull();
  inline bool IsBufferEmpty();
  inline uint64_t GetTotalHandleBytes();
  inline int GetFreeBytes();
  inline int GetUsedBytes();
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
