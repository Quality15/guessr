#include<iostream>
#include<stdio.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string>
#include<vector>
#include<unistd.h>
#include<cstring>

class tcp_server
{
public:
  tcp_server(int port);
  void run();

private:
  int port;
  int sockfd;
  void handle_clients(int clientfd_1, int clientfd_2);
};

class tcp_client
{
public:
  tcp_client(const std::string& ip, int port);
  void run();

  void play_chooser();
  void play_guesser();

private:
  std::string ip;
  int port;
  int clientfd;
};