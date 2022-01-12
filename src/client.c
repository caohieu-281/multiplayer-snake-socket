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

void INThandler(int sig)
{
    char c;
    signal(sig, SIG_IGN);
    printf("OUCH, did you hit Ctrl-C?\n"
           "Do you really want to quit? [y/n] ");
    c = getchar();
    if (c == 'y' || c == 'Y')
        exit(0);
    else
        signal(SIGINT, INThandler);
    getchar(); // Get new line character
}

int main(int argc, char *argv[])
{
    signal(SIGINT, INThandler);
    int sockfd = ClientCreateSocket(atoi(argv[1]));
    LoginGame(sockfd);
    return 0;
}