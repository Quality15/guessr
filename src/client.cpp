#include "network.h"
#include <string>
#include <unistd.h>

tcp_client::tcp_client(const std::string& ip, int port) : ip(ip), port(port) {}

void tcp_client::run()
{
  clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if(clientfd < 0) {
    std::cerr << "[ ERR ] Failed to create socket!\n";
    return;
  }

  sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);

  if(connect(clientfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
    std::cerr << "[ ERR ] Failed to connect to server!\n";
    return;
  }

  // get role from server
  char role_buff[32];
  int b_read = recv(clientfd, &role_buff, sizeof(role_buff)-1, 0);
  if(b_read > 0) {
    role_buff[b_read] = '\0';
    std::string role = role_buff;

    // std::cout << role << '\n';
    if(role == "guesser") {
      std::cout << "[GAME] You are guesser!\n";
      this->play_guesser();
    }
    else if(role == "chooser") {
      std::cout << "[GAME] You are chooser!\n";
      this->play_chooser();
    }
  }

  close(clientfd);
}

void tcp_client::play_chooser()
{
  int num;
  std::cout << "Enter number from 1 to 100: ";
  std::cin >> num;

  if(num < 1 || num > 100) {
    std::cerr << "[ ERR ] You need to enter number only from 1 to 100!\n";
    return;
  }

  send(clientfd, &num, sizeof(num), 0);

  // wait for server answer
  char buff[32];
  recv(clientfd, buff, sizeof(buff), 0); // char buff[256] is alreay pointer
  // std::cout << "[ DBG ] " << buff << '\n';
  if(std::string(buff) == "correct") {
    std::cout << "[GAME]: Player No2 guessed your number!\n";
  }
}

void tcp_client::play_guesser()
{
  int guess;
  char buff[32];

  std::cout << "[GAME] Wait for another player to choose the number...\n";
  
  int b_read = recv(clientfd, buff, sizeof(buff), 0);
  if(b_read > 0) {
    buff[b_read] = '\0';
    if(std::string(buff) == "guess") {
      for(;;) {
        // send guess on server
        std::cout << "Enter your guess (1-100): ";
        std::cin >> guess;
        send(clientfd, &guess, sizeof(guess), 0);

        char buff[256];
        int b_read = recv(clientfd, buff, sizeof(buff), 0);
        if(b_read > 0) {
          buff[b_read] = '\0';
          // std::cout << "[ DBG ] " << buff << '\n';

          if(std::string(buff) == "correct") {
            std::cout << "[WIN] Congrats! You guessed it\n";
            break;
          } else if(std::string(buff) == "higher") {
            std::cout << "[+] Your guess is higher than needed!\n";
          } else if(std::string(buff) == "lower") {
            std::cout << "[-] Your guess is lower than needed!\n";
          }
        }
      }
    }
  }


}