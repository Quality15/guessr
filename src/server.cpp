#include "network.h"
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

tcp_server::tcp_server(int port) : port(port) {}

void tcp_server::run()
{
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    std::cerr << "[ ERR ] Failed to create socket!\n";
    return;
  }

  sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "[ ERR ] Failed to bind socket!\n";
    return;
  }

  listen(sockfd, 2); // inf con
  std::cout << "[ DBG ] Listening on port " << port << '\n';

  int clientfd_1 = accept(sockfd, nullptr, nullptr);
  if(clientfd_1 < 0) {
    std::cerr << "[ ERR ] Failed to accept connection (1)!\n";
    return;
  }
  std::cout << "[ DBG ] Player 1 connected!\n";

  int clientfd_2 = accept(sockfd, nullptr, nullptr);
  if(clientfd_1 < 0) {
    std::cerr << "[ ERR ] Failed to accept connection (2)!\n";
    return;
  }
  std::cout << "[ DBG ] Player 2 connected!\n";

  handle_clients(clientfd_1, clientfd_2);
  
  close(clientfd_1);
  close(clientfd_2);
  close(sockfd);
}

void tcp_server::handle_clients(int clientfd_1, int clientfd_2)
{
  // send to players their roles
  std::string msg_chooser = "chooser";
  std::string msg_guesser = "guesser";

  send(clientfd_1, msg_guesser.c_str(), msg_guesser.length(), 0);
  send(clientfd_2, msg_chooser.c_str(), msg_chooser.length(), 0);

  // receive number to guess
  int num2guess;
  recv(clientfd_2, &num2guess, sizeof(num2guess), 0); // we use `&` before `num2guess` becuse `recv` needs `void *_buf`, and `int num2guess` isnt pointer
  std::cout << "[ DBG ] Number to guess: " << num2guess << '\n';

  // send message to guesser that he can now guess
  const char *msg = "guess";
  send(clientfd_1, msg, strlen(msg), 0);

  // get player's guesses and answer
  for(;;) {
    int guess;
    recv(clientfd_1, &guess, sizeof(guess), 0);
    std::cout << "[ DBG ] Player guessed: " << guess << '\n';

    if(guess == num2guess) {
      const char* correct_msg = "correct";
      send(clientfd_1, correct_msg, strlen(correct_msg), 0); // we do NOT use `&` before `correct_msg` because `const char*` is already a pointer
      send(clientfd_2, correct_msg, strlen(correct_msg), 0);
      std::cout << "[ DBG ] Correct " << '\n';
    } else if(guess > num2guess) {
      const char* higher_msg = "higher";
      send(clientfd_1, higher_msg, strlen(higher_msg), 0);
      std::cout << "[ DBG ] Higher " << '\n';
    } else if(guess < num2guess) {
      const char* lower_msg = "lower";
      send(clientfd_1, lower_msg, strlen(lower_msg), 0);
      std::cout << "[ DBG ] Lower " << '\n';
    }
  }
}