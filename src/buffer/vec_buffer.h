#ifndef __VEC_BUFFER_H_
#define __VEC_BUFFER_H_

#include <arpa/inet.h>
#include <sys/uio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

class VecBuffer {
public:
  VecBuffer();
  ~VecBuffer();
  void Init();

  int ReadFromFd(int);
  void SaveData(char*, int);
  int WriteToFd(int);

  char* GetReadIndex();
  int GetUnHandleBytesNum();
  uint64_t GetTotalHandleBytes();
  void IncrReadIndex(int);
private:
  void EnlargeBufferAndMoveData(char*, int);
private:
  char* buffer_;
  int current_buffer_size_;
  int write_index_;
  int read_index_;
  int enlarge_count_;
  uint64_t total_handle_bytes_;
  static constexpr int init_buffer_size_ = 65536;
};
#endif
