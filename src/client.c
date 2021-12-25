#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "login.h"
#include "socket.h"

int main(int argc, char *argv[])
{
    int sockfd = ClientCreateSocket(atoi(argv[1]));
    // ViewWelcomeScreen();
    LoginGame(sockfd);
    return 0;
}