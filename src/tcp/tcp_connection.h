#ifndef __TCP_CONNECTION_H_
#define __TCP_CONNECTION_H_

#include <arpa/inet.h>
#include <sys/uio.h>
#include <string.h>

#include <functional>

#include "logger/logger.h"
#include "buffer/vec_buffer.h"
#include "buffer/ring_buffer.h"

class TcpConnection {
public:
  TcpConnection();
  TcpConnection(int);
  ~TcpConnection();
  void SetSocket(int socket_fd);
  void Init();
  void SetCallback(std::function<void(char*, int)>);
  int Read();
  void ExtractMessageFromInput();
  void QueueMessage(char*, int);

  int Write(char*, int);

  int input_free_bytes_count();
  int input_data_bytes_count();
  int output_data_bytes_count();
  int output_free_bytes_count();
private:
  VecBuffer read_buffer_;
  VecBuffer write_buffer_;
  int socket_fd_;
  char* input_buffer_;
  char* output_buffer_;
  int write_index_;
  int read_index_;
  uint32_t latest_message_len_;
  uint64_t total_read_bytes_;
  bool input_buffer_full_;
  bool input_buffer_empty_;
  bool output_buffer_full_;
  bool output_buffer_empty_;
  static constexpr int buffer_size_ = 65536;
  static constexpr int INT_SIZE = sizeof(int);
};

#endif
