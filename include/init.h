#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "socket.h"
#include "login.h"
#include "view.h"

#define MESSAGE_MAX 4096
#define MAX_PLAYER 5
#define USERNAME_MAX 50
#define PASSWORD_MAX 50

char message[MESSAGE_MAX];



