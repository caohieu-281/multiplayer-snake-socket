#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <signal.h>
#include "login.h"
#include "socket.h"

void ctrl_c_handler(){
    printf("\nQuit game!.\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, ctrl_c_handler);
    int sockfd = ClientCreateSocket(atoi(argv[1]));
    // ViewWelcomeScreen();
    LoginGame(sockfd);
    return 0;
}