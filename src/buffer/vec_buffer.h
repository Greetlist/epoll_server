#ifndef __VEC_BUFFER_H_
#define __VEC_BUFFER_H_

#include <arpa/inet.h>
#include <sys/uio.h>
#include <string.h>
#include <errno.h>

class VecBuffer {
public:
  VecBuffer();
  ~VecBuffer();
  void Init();
  void SetCallback(std::function<int(char*, int)> func);

  int ReadFromFd(int);
  int SaveData(char*, int);
  int WriteToFd(int);

  inline char* GetReadIndex();
private:
  void EnlargeBufferAndMoveData(char*, int);
private:
  std::function<int(char*, int)> on_recv_data_;
  char* buffer_;
  int current_buffer_size_;
  int write_index_;
  int read_index_;
  int enlarge_count_;
  static constexpr int init_buffer_size_ = 65536;
};
#endif
