#ifndef __VEC_BUFFER_H_
#define __VEC_BUFFER_H_

#include <arpa/inet.h>
#include <sys/uio.h>
#include <string.h>

class VecBuffer {
public:
  VecBuffer();
  ~VecBuffer();
  void Init();

  // produce function
  int ReadFromFd(int);
  int SaveData(char*, int);
  // consume function
  int FetchData(char*);
  int WriteToFd(int);

  inline int GetConsumableNum();
  inline int GetCurrentDataNum();
private:
  void EnlargeBufferAndMoveData(char*, int);
private:
  std::function<int(char*, int)> OnMessage;
  char* buffer_;
  int current_buffer_size_;
  int write_index_;
  int read_index_;
  int enlarge_count_;
  static constexpr int init_buffer_size_ = 65536;
};
#endif
