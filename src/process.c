#include "init.h"

int string_split(const char *str, char c, char ***arr)
{
    int count = 1;
    int token_len = 1;
    int i = 0;
    char *p;
    char *t;

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
            count++;
        p++;
    }

    *arr = (char **)malloc(sizeof(char *) * count);
    // if (*arr == NULL)
    //     exit(1);

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
        {
            (*arr)[i] = (char *)malloc(sizeof(char) * token_len);
            // if ((*arr)[i] == NULL)
            //     exit(1);

            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }
    (*arr)[i] = (char *)malloc(sizeof(char) * token_len);
    // if ((*arr)[i] == NULL)
    //     exit(1);

    i = 0;
    p = str;
    t = ((*arr)[i]);
    while (*p != '\0')
    {
        if (*p != c && *p != '\0')
        {
            *t = *p;
            t++;
        }
        else
        {
            *t = '\0';
            i++;
            t = ((*arr)[i]);
        }
        p++;
    }
    return count;
}

void freeMemory(char ***arr, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(arr[i]);
    }

    free(arr);
}

void readUserFromFile()
{
    FILE *fin = fopen(FILE_DATA, "r");
    if (fin == NULL)
    {
        printf("Can't open file\n");
        return;
    }
    char readFile[MAX_LENGTH];
    int id = -1;
    while (fgets(readFile, MAX_LENGTH, fin))
    {
        char **arr = NULL;
        id++;
        int count = string_split(readFile, ' ', &arr);
        for (int i = 0; i < count; i++)
            if (arr[i][strlen(arr[i]) - 1] == '\n')
                arr[i][strlen(arr[i]) - 1] = 0;

        strcpy(listUsers[id].username, arr[0]);
        strcpy(listUsers[id].password, arr[1]);
        listUsers[id].status = 0;
        freeMemory(arr, count);
    }
    numberUsers = id + 1;

    fclose(fin);
}

int checkSignIn(char *username, char *password, int socket)
{
    for (int i = 0; i < numberUsers; i++)
    {
        if (strcmp(username, listUsers[i].username) == 0)
        {
            if (strcmp(password, listUsers[i].password) == 0)
            {
                listUsers[i].status = 1;
                listUsers[i].socketID = socket;
                return 1;
            }
            else
                // wrong password
                return 0;
        }
    }
    // not find users
    return -1;
}

int checkSignUp(char *username, char *password, int socket)
{
    for (int i = 0; i < numberUsers; i++)
    {
        if (strcmp(username, listUsers[i].username) == 0)
        {
            return 0;
        }
    }
    return 1;
}

void addUser(char *usename, char *password)
{
    strcpy(listUsers[numberUsers].username, usename);
    strcpy(listUsers[numberUsers].password, password);
    numberUsers++;
}

void writeUserToFile()
{
    FILE *fin = fopen(FILE_DATA, "w");
    if (fin == NULL)
    {
        printf("Can't open file\n");
        return;
    }
    char charLine[MAX_LENGTH];
    for (int i = 0; i < numberUsers; i++)
    {
        sprintf(charLine, "%s %s\n", listUsers[i].username, listUsers[i].password);
        fputs(charLine, fin);
    }
    fclose(fin);
}

// Search th user in listUser with sockfd and status
int SearchUser(User listUsers[], int sockfd, int status)
{
    for (int i = 0; i < numberUsers; i++)
        if (listUsers[i].socketID == sockfd && listUsers[i].status == status)
            return i;
    return -1;
}

// Search th room in listRoom, return th room in list
int SearchRoom(int roomID)
{
    for (int i = 0; i < numberRooms; i++)
        if (listRooms[i].roomID == roomID)
            return i;
    return -1;
}

// Delete room
void DeleteRoom(int roomID)
{
    int thRoom = SearchRoom(roomID);
    for (int i = thRoom; i < numberRooms - 1; i++)
    {
        for (int j = 0; j < listRooms[i + 1].numberUsersInRoom; j++)
        {
            strcpy(listRooms[i].usersInRoom[j].password, listRooms[i + 1].usersInRoom[j].password);
            strcpy(listRooms[i].usersInRoom[j].username, listRooms[i + 1].usersInRoom[j].username);
            listRooms[i].usersInRoom[j].socketID = listRooms[i + 1].usersInRoom[j].socketID;
            listRooms[i].usersInRoom[j].score = listRooms[i + 1].usersInRoom[j].score;
            listRooms[i].usersInRoom[j].status = listRooms[i + 1].usersInRoom[j].status;
        }
        listRooms[i].numberUsersInRoom = listRooms[i + 1].numberUsersInRoom;
        listRooms[i].roomID = listRooms[i + 1].roomID;
    }
    numberRooms--;
}

void MakeGame(int roomID)
{
    int thRoom = SearchRoom(roomID);
    for (int i = 1; i < listRooms[thRoom].numberUsersInRoom; i++)
    {
        // -1: Let out the room
        memset(messageServer, 0, sizeof(messageServer));
        sprintf(messageServer, "15");
        ServerSendToClient(listRooms[thRoom].usersInRoom[i].socketID);
    }
}

void UserOutRoom(int roomID, int userID)
{
    int thRoom = SearchRoom(roomID);
    // If host out room -> Out all player in room
    if (listRooms[thRoom].roomID == userID)
    {
        for (int i = 0; i < listRooms[thRoom].numberUsersInRoom; i++)
        {
            // -1: Let out the room
            memset(messageServer, 0, sizeof(messageServer));
            sprintf(messageServer, "-1");
            ServerSendToClient(listRooms[thRoom].usersInRoom[i].socketID);
        }
        DeleteRoom(roomID);
    }
    // If player in room (not host)
    else
    {
        memset(messageServer, 0, sizeof(messageServer));
        sprintf(messageServer, "-1");
        ServerSendToClient(userID);
        int thUserInRoom = SearchUser(listRooms[thRoom].usersInRoom, userID, 1);
        for (int i = thUserInRoom; i < listRooms[thRoom].numberUsersInRoom - 1; i++)
        {
            strcpy(listRooms[thRoom].usersInRoom[i].password, listRooms[thRoom].usersInRoom[i + 1].password);
            strcpy(listRooms[thRoom].usersInRoom[i].username, listRooms[thRoom].usersInRoom[i + 1].username);
            listRooms[thRoom].usersInRoom[i].socketID = listRooms[thRoom].usersInRoom[i + 1].socketID;
            listRooms[thRoom].usersInRoom[i].score = listRooms[thRoom].usersInRoom[i + 1].score;
            listRooms[thRoom].usersInRoom[i].status = listRooms[thRoom].usersInRoom[i + 1].status;
        }
        listRooms[thRoom].numberUsersInRoom--;
    }
}

void RefreshScreenWaitingRoom(int roomID)
{
    int thRoom = SearchRoom(roomID);
    int numberInRoom = listRooms[thRoom].numberUsersInRoom;
    char inforPlayerInRoom[MAX_LENGTH];
    sprintf(inforPlayerInRoom, "%d %s", 1, listRooms[thRoom].usersInRoom[0].username);
    for (int i = 1; i < listRooms[thRoom].numberUsersInRoom; i++)
    {
        sprintf(inforPlayerInRoom, "%s %d %s", inforPlayerInRoom, i + 1, listRooms[thRoom].usersInRoom[i].username);
    }
    for (int i = 1; i < listRooms[thRoom].numberUsersInRoom; i++)
    {
        memset(messageServer, 0, sizeof(messageServer));
        sprintf(messageServer, "14 %d %s", numberInRoom, inforPlayerInRoom);
        ServerSendToClient(listRooms[thRoom].usersInRoom[i].socketID);
    }
}

/*game function for server*/
int someone_won = 0;
int game_map[HEIGHT + 10][WIDTH + 10];
int map_size = (HEIGHT + 10) * (WIDTH + 10) * sizeof(game_map[0][0]);
pthread_mutex_t map_lock = PTHREAD_MUTEX_INITIALIZER;

void AddWall()
{
    int x, y, a;
    do
    {
        y = rand() % (HEIGHT - 6) + 3;
        x = rand() % (WIDTH - 6) + 3;
    } while (game_map[y][x] != 0);
    pthread_mutex_lock(&map_lock);
    a = rand() % 10;
    while (a + y >= HEIGHT || a < 3)
    {
        a = rand() % 10;
    }
    for (int i = 0; i < a; i++)
    {
        if (game_map[y + i][x] == 0)
            game_map[y + i][x] = WALL;
    }
    pthread_mutex_unlock(&map_lock);
}

void AddWall2()
{
    int x, y, a;
    do
    {
        y = rand() % (HEIGHT - 6) + 3;
        x = rand() % (WIDTH - 6) + 3;
    } while (game_map[y][x] != 0);
    pthread_mutex_lock(&map_lock);
    a = rand() % 10;
    while (a + x >= WIDTH || a < 3)
    {
        a = rand() % 10;
    }
    for (int i = 0; i < a; i++)
    {
        if (game_map[y][x + i] == 0)
            game_map[y][x + i] = WALL2;
    }
    pthread_mutex_unlock(&map_lock);
}

void AddFruit()
{
    int x, y;
    do
    {
        y = rand() % (HEIGHT - 6) + 3;
        x = rand() % (WIDTH - 6) + 3;
    } while (game_map[y][x] != 0);
    pthread_mutex_lock(&map_lock);
    game_map[y][x] = FRUIT;
    pthread_mutex_unlock(&map_lock);
}

//Function to create a snake
snake *MakeSnake(int player_no, int head_y, int head_x)
{

    //Place the snake on the map (matrix)
    pthread_mutex_lock(&map_lock);
    game_map[head_y][head_x] = -player_no;
    game_map[head_y + 1][head_x] =
        game_map[head_y + 2][head_x] = player_no;
    pthread_mutex_unlock(&map_lock);

    //Create snake struct, set coordinates facing up
    snake *s = malloc(sizeof(snake));

    s->player_no = player_no;
    s->length = 3;

    s->head.y = head_y;
    s->head.x = head_x;
    s->head.d = UP;

    s->body_segment[0].y = head_y + 1;
    s->body_segment[0].x = head_x;
    s->body_segment[0].d = UP;

    s->tail.y = head_y + 2;
    s->tail.x = head_x;
    s->tail.d = UP;

    return s;
}

//Function to kill snake and free memory
void KillSnake(snake *s)
{

    //Set all snake coordinates to zero on map
    pthread_mutex_lock(&map_lock);
    game_map[s->head.y][s->head.x] = game_map[s->tail.y][s->tail.x] = 0;
    int i;
    for (i = 0; i < s->length - 2; i++)
        game_map[s->body_segment[i].y][s->body_segment[i].x] = 0;
    pthread_mutex_unlock(&map_lock);
}

//Function for a snake to eat a fruit in front of it
void EatFruit(snake *s, direction d, int player_no)
{
    memmove(&(s->body_segment[1]),
            &(s->body_segment[0]),
            (s->length - 2) * sizeof(coordinate));
    s->body_segment[0].y = s->head.y;
    s->body_segment[0].x = s->head.x;
    s->body_segment[0].d = s->head.d;
    switch (d)
    {
    case UP:
    {
        s->head.y = s->head.y - 1;
        s->head.d = UP;
        if (game_map[s->head.y][s->head.x + 1] == FRUIT)
        {
            pthread_mutex_lock(&map_lock);
            game_map[s->head.y][s->head.x + 1] = 0;
            pthread_mutex_unlock(&map_lock);
        }
        break;
    }
    case DOWN:
    {
        s->head.y = s->head.y + 1;
        s->head.d = DOWN;
        if (game_map[s->head.y][s->head.x + 1] == FRUIT)
        {
            pthread_mutex_lock(&map_lock);
            game_map[s->head.y][s->head.x + 1] = 0;
            pthread_mutex_unlock(&map_lock);
        }
        break;
    }
    case LEFT:
    {
        s->head.x = s->head.x - 1;
        s->head.d = LEFT;
        break;
    }
    case RIGHT:
    {
        s->head.x = s->head.x + 1;
        s->head.d = RIGHT;
        break;
    }
    default:
        break;
    }
    pthread_mutex_lock(&map_lock);
    game_map[s->head.y][s->head.x] = -(s->player_no);
    game_map[s->body_segment[0].y][s->body_segment[0].x] = s->player_no;
    pthread_mutex_unlock(&map_lock);
    s->length++;
    game_map[HEIGHT + player_no][WIDTH + 2] = s->length + 10000;
    AddFruit();
}

//Function to move snake
void MoveSnake(snake *s, direction d)
{
    memmove(&(s->body_segment[1]),
            &(s->body_segment[0]),
            (s->length - 2) * sizeof(coordinate));
    s->body_segment[0].y = s->head.y;
    s->body_segment[0].x = s->head.x;
    s->body_segment[0].d = s->head.d;
    switch (d)
    {
    case UP:
    {
        s->head.y = s->head.y - 1;
        s->head.d = UP;
        break;
    }
    case DOWN:
    {
        s->head.y = s->head.y + 1;
        s->head.d = DOWN;
        break;
    }
    case LEFT:
    {
        s->head.x = s->head.x - 1;
        s->head.d = LEFT;
        break;
    }
    case RIGHT:
    {
        s->head.x = s->head.x + 1;
        s->head.d = RIGHT;
        break;
    }
    default:
        break;
    }
    pthread_mutex_lock(&map_lock);
    game_map[s->head.y][s->head.x] = -(s->player_no);
    game_map[s->body_segment[0].y][s->body_segment[0].x] = s->player_no;
    game_map[s->tail.y][s->tail.x] = 0;
    pthread_mutex_unlock(&map_lock);

    s->tail.y = s->body_segment[s->length - 2].y;
    s->tail.x = s->body_segment[s->length - 2].x;
}

void PlayGame(int socket)
{
    int i;
    // MakeGame(atoi(arr[1]));

    //Fill gamestate matrix with zeros
    memset(game_map, 0, map_size);

    //Set game borders
    for (i = 0; i < HEIGHT; i++)
        game_map[i][0] = game_map[i][WIDTH - 2] = BORDER;
    for (i = 0; i < WIDTH; i++)
        game_map[0][i] = game_map[HEIGHT - 1][i] = BORDER;

    //Randomly add five fruit
    srand(time(NULL));
    for (i = 0; i < 5; i++)
        AddFruit(map_lock);
    for (i = 0; i < 3; i++)
        AddWall(map_lock);
    for (i = 0; i < 2; i++)
        AddWall2(map_lock);

    //Find three consecutive zeros in map for starting snake position
    int head_y, head_x;
    srand(time(NULL));
    do
    {
        head_y = rand() % (HEIGHT - 6) + 3;
        head_x = rand() % (WIDTH - 6) + 3;
    } while (!(((game_map[head_y][head_x] == game_map[head_y + 1][head_x]) == game_map[head_y + 2][head_x]) == 0));
    int player_no = socket - 3;
    snake *player_snake = MakeSnake(player_no, head_y, head_x);

    //Variables for user input
    char key = UP;
    char key_buffer;
    char map_buffer[map_size];
    int bytes_sent, n;
    int success = 1;

    memcpy(map_buffer, game_map, map_size);
    bytes_sent = 0;

    while (bytes_sent < map_size)
    {
        bytes_sent += send(socket, game_map, map_size, 0);
        if (bytes_sent < 0)
            error("ERROR writing to socket");
    }
    while (success)
    {
        // Check if someone won
        if (someone_won)
            success = 0;

        // Check if you are the winner
        if (player_snake->length - 3 >= WINNER_LENGTH)
        {
            someone_won = player_no;
            pthread_mutex_lock(&map_lock);
            game_map[0][0] = WINNER;
            pthread_mutex_unlock(&map_lock);
        }
        else if (game_map[0][0] != BORDER)
        {
            pthread_mutex_lock(&map_lock);
            game_map[0][0] = someone_won;
            pthread_mutex_unlock(&map_lock);
        }

        //Copy map to buffer, and send to client
        memcpy(map_buffer, game_map, map_size);
        bytes_sent = 0;

        while (bytes_sent < map_size)
        {
            bytes_sent += send(socket, game_map, map_size, 0);
            if (bytes_sent < 0)
                error("ERROR writing to socket");
        }

        // Player key input
        bzero(&key_buffer, 1);
        n = read(socket, &key_buffer, 1);
        if (n <= 0)
            break;

        // If user key is a direction, then apply it
        key_buffer = toupper(key_buffer);
        if (((key_buffer == UP) && !(player_snake->head.d == DOWN)) || ((key_buffer == DOWN) && !(player_snake->head.d == UP)) || ((key_buffer == LEFT) && !(player_snake->head.d == RIGHT)) || ((key_buffer == RIGHT) && !(player_snake->head.d == LEFT)))
            key = key_buffer;

        if (player_snake != NULL)
        {
            switch (key) {
            case UP:
            {
                if ((game_map[player_snake->head.y - 1][player_snake->head.x] == 0) &&
                    !(game_map[player_snake->head.y - 1][player_snake->head.x + 1] == FRUIT))
                {
                    MoveSnake(player_snake, UP);
                }
                else if ((game_map[player_snake->head.y - 1][player_snake->head.x] == FRUIT) ||
                         (game_map[player_snake->head.y - 1][player_snake->head.x + 1] == FRUIT))
                {
                    EatFruit(player_snake, UP, player_no);
                }
                else
                {
                    KillSnake(player_snake);
                }
                break;
            }

            case DOWN:
            {
                if ((game_map[player_snake->head.y + 1][player_snake->head.x] == 0) &&
                    !(game_map[player_snake->head.y + 1][player_snake->head.x + 1] == FRUIT))
                {
                    MoveSnake(player_snake, DOWN);
                }
                else if ((game_map[player_snake->head.y + 1][player_snake->head.x] == FRUIT) ||
                         (game_map[player_snake->head.y + 1][player_snake->head.x + 1] == FRUIT))
                {
                    EatFruit(player_snake, DOWN, player_no);
                }
                else
                {
                    KillSnake(player_snake);
                }
                break;
            }

            case LEFT:
            {
                if (game_map[player_snake->head.y][player_snake->head.x - 1] == 0)
                {
                    MoveSnake(player_snake, LEFT);
                }
                else if (game_map[player_snake->head.y][player_snake->head.x - 1] == FRUIT)
                {
                    EatFruit(player_snake, LEFT, player_no);
                }
                else
                {
                    KillSnake(player_snake);
                }
                break;
            }

            case RIGHT:
            {
                if (game_map[player_snake->head.y][player_snake->head.x + 1] == 0)
                {
                    MoveSnake(player_snake, RIGHT);
                }
                else if (game_map[player_snake->head.y][player_snake->head.x + 1] == FRUIT)
                {
                    EatFruit(player_snake, RIGHT, player_no);
                }
                else
                {
                    KillSnake(player_snake);
                }
                break;
            }

            default:
                break;
            }
        }
    }
    if (player_snake->length - 3 == WINNER_LENGTH)
    {
        fprintf(stderr, "Player %d had won!\n", player_no);
        // writeFile("nguoidung.txt", l);
        free(player_snake);
        player_snake = NULL;
        game_map[HEIGHT + player_no][WIDTH + 2] = 0;
        return 0;
    }
    else
    {
        fprintf(stderr, "Player %d had exited game!\n", player_no);
        free(player_snake);
        player_snake = NULL;
        game_map[HEIGHT + player_no][WIDTH + 2] = 0;
        return 0;
    }
}

void sortScore()
{
    return;
}