#include "epoll_server/server_factory.h"
#include "epoll_server/global_def.h"

#include <unordered_map>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
  std::unordered_map<std::string, std::string> config{
    {"listen_addr", "0.0.0.0"},
    {"listen_port", "8888"},
  };
  EpollServerBase* server = EpollServerFactory::GetServer(ServerType::TCP, config["listen_addr"], std::stoi(config["listen_port"]));
  server->Init();
  server->Start();
  return 0;
}
