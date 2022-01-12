#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <assert.h>
#include <ncurses.h>

#include "socket.h"
#include "login.h"
#include "view.h"
#include "process.h"
#include "gamefunction.h"
#include "gameplay.h"


#define MESSAGE_MAX 4096
#define MAX_PLAYER 5
#define MAX_USER 1000
#define MAX_ROOM 200
#define USERNAME_MAX 50
#define PASSWORD_MAX 50
#define MAX_LENGTH 1000
#define FILE_DATA "./data/datauser.txt"

#define HEIGHT      24
#define WIDTH       80
#define FRUIT       -111
#define WALL        -1111
#define WALL2       -1112
#define BORDER      -99
#define REFRESH     0.15
#define ONGOING     -34
#define MAX_SNAKE_LENGTH    HEIGHT * WIDTH
#define UP_KEY              'W'
#define DOWN_KEY            'S'
#define LEFT_KEY            'A'
#define RIGHT_KEY           'D'
#define INTERRUPTED -30

char messageClient[MESSAGE_MAX];
char messageServer[MESSAGE_MAX];

typedef struct _User
{
    char username[USERNAME_MAX];
    char password[PASSWORD_MAX];
    int socketID;
    int score;
    int status; // 0: not sign in, 1: signned in
} User;
int numberUsers;
User listUsers[MAX_USER];

typedef struct _Room
{
    User usersInRoom[MAX_PLAYER];
    int numberUsersInRoom;
    int roomID; // roomID is socketID of host room
    int game_map[HEIGHT+10][WIDTH+10];
} Room;

Room listRooms[MAX_ROOM];
int numberRooms;

#define HEIGHT      24
#define WIDTH       80
#define FRUIT       -111
#define WALL        -1111
#define WALL2       -1112
#define BORDER      -99
#define REFRESH     0.15
#define ONGOING     -34
#define MAX_SNAKE_LENGTH    HEIGHT * WIDTH
#define UP_KEY              'W'
#define DOWN_KEY            'S'
#define LEFT_KEY            'A'
#define RIGHT_KEY           'D'
#define INTERRUPTED -30
#define WINNER_LENGTH       5
#define WINNER              -94
#define LOSE              -100

//Direction key types
typedef enum{
    UP    = UP_KEY, 
    DOWN  = DOWN_KEY, 
    LEFT  = LEFT_KEY, 
    RIGHT = RIGHT_KEY
} direction;

typedef struct{
    int x, y;
    direction d;
} coordinate;

typedef struct{
    int player_no, length;
    coordinate head;
    coordinate body_segment[MAX_SNAKE_LENGTH - 2];
    coordinate tail;
} snake;