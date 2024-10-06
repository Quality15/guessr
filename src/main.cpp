#include<iostream>
#include<stdio.h>
#include<arpa/inet.h>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>

#include "network.h"

int main(int argc, char *argv[])
{
  if(argc < 2) {
    std::cerr << "Usage: ./guessr <server/client>\n";
    return 1;
  }
  std::string mode = std::string(argv[1]);
  if(mode == "server") {
    tcp_server serv(4444);
    serv.run();
  } 
  else if (mode == "client") {
    std::string ip;// = "127.0.0.1";
    std::cout << "Enter host ip: "; std::cin >> ip;
    tcp_client client(ip, 4444);
    client.run();
  }

  return 0;   
}