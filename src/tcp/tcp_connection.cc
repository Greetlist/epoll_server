#include "tcp/tcp_connection.h"

TcpConnection::TcpConnection(int sock_fd) : socket_fd_(sock_fd), latest_message_type_(-1) {
}

TcpConnection::~TcpConnection() {
  LOG_INFO("Total Read Bytes: %ld", read_buffer_.GetTotalHandleBytes());
  LOG_INFO("Total Write Bytes: %ld", write_buffer_.GetTotalHandleBytes());
}

void TcpConnection::Init() {
  read_buffer_.Init();
  write_buffer_.Init();
}

int TcpConnection::Read() {
  int n_read = read_buffer_.ReadFromFd(socket_fd_);
  return n_read;
}

int TcpConnection::ExtractMessage() {
  int total_handle_bytes = 0;
  int unhandle_bytes = read_buffer_.GetUnHandleBytesNum();
  char* read_index = read_buffer_.GetReadIndex();
  while (true) {
    if (unhandle_bytes <= 0) {
      break;
    }

    //read message type first
    if (latest_message_type_ == -1 && unhandle_bytes < INT_SIZE) {
      break;
    }
    if (latest_message_type_ == -1 && unhandle_bytes >= INT_SIZE) {
      uint32_t cur;
      memcpy(&cur, read_index, INT_SIZE);
      latest_message_type_ = ntohl(cur);
      read_index += INT_SIZE;
      total_handle_bytes += INT_SIZE;
      unhandle_bytes -= INT_SIZE;
    }

    uint32_t message_len = GetMessageLen(latest_message_type_);
    // current data is not enough to construct one message
    if (message_len > unhandle_bytes - INT_SIZE) {
      return INT_SIZE;
    } else { //has enough data for construction
      char message[message_len];
      memcpy(message, read_index, message_len);
      QueueMessage(message, message_len);
      total_handle_bytes += message_len;
      unhandle_bytes -= message_len;
      latest_message_type_ = -1;
    }
  }
  return total_handle_bytes;
}

void TcpConnection::QueueMessage(char* message, int message_len) {
  LOG_INFO("message_type is: %d, message_len: %d", latest_message_type_, message_len);
}

int TcpConnection::Write(char* data, int data_len) {
  write_buffer_.SaveData(data, data_len);
  return 0;
}

//TODO
uint32_t TcpConnection::GetMessageLen(int message_type) {
  return 1;
}
