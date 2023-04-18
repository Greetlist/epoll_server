#ifndef __TCP_CONNECTION_H_
#define __TCP_CONNECTION_H_

#include <arpa/inet.h>

#include <functional>

class TcpConnection {
public:
  TcpConnection();
  TcpConnection(int);
  ~TcpConnection();
  void Init();
  void SetCallback(std::function<char*>, std::function<void()>);
  int Read();
  void ExtractMessage();
private:
  std::function<void(char*)> OnMessage;
  std::function<void()> OnWriteFinish;
  int socket_fd_;
  char* input_buffer_;
  char* output_buffer_;
  int write_index_;
  int read_index_;
  uint32_t latest_message_len_;
  bool intput_buffer_full_;
  bool output_buffer_full_;
  static constexpr int buffer_size_ = 65536;
  static constexpr int INT_SIZE = sizeof(int);
};

#endif
