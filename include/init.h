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
#include "process.h"
#include <assert.h>

#define MESSAGE_MAX 4096
#define MAX_PLAYER 5
#define USERNAME_MAX 50
#define PASSWORD_MAX 50
#define MAX_LENGTH 1000
#define FILE_DATA "./data/datauser.txt"

char messageClient[MESSAGE_MAX];
char messageServer[MESSAGE_MAX];

typedef struct _User{
    char username[USERNAME_MAX];
    char password[PASSWORD_MAX];
    int socketID;
    int score;
} User;

int numberUsers;
User listUsers[MAX_PLAYER];


