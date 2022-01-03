#include "init.h"
void GameFunction(int sockfd)
{
    char choice[MAX_LENGTH];
    do
    {
        ViewFunctionInGameScreen();
        scanf("%s", choice);
        fflush(stdin);
        // Create room
        if (strcmp(choice, "1") == 0)
        {
            system("clear");
            CreateRoom(sockfd);
            printf("----------------------Create Room------------------\n");
        }
        // Join waiting-room
        else if (strcmp(choice, "2") == 0)
        {
            system("clear");
            ViewWelcomeScreen();
            JoinRoom(sockfd);
            printf("\n_______Let's play________\n\n");
        }
        // Change Password
        else if (strcmp(choice, "3") == 0)
        {
            system("clear");
            printf("\n _________________Change Password_________________\n\n");
            if (ChangePassword(sockfd))
                GameFunction(sockfd);
            else
            {
                printf(" ____________Password is not exactly!!!___________\n\n");
                ChangePassword(sockfd);
            }
        }
        // Show Profile
        else if (strcmp(choice, "4") == 0)
        {
            system("clear");
            ShowProfile(sockfd);
        }
        // Show leaderboard
        else if (strcmp(choice, "5") == 0)
        {
            system("clear");
            printf("\n_______This is leaderboard________\n\n");
        }
        // Exit
        else if (strcmp(choice, "6") == 0)
        {
            system("clear");
            if (LogOut(sockfd))
            {
                system("clear");
                LoginGame(sockfd);
            }
            else
                GameFunction(sockfd);
        }
        else
            printf(" ___________We don't have this choice!!!__________\n\n");
    } while (choice != 6);
}

void CreateRoom(int sockfd)
{
    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "3 ");
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    char **arr = NULL;
    int count = string_split(messageClient, ' ', &arr);
    // Cannot create room cuz max room
    if (strcmp(arr[0], "0") == 0)
    {
        printf("\nCannot create room cuz max room\n");
        CountTime("Return play game screen", 5);
        system("clear");
    }
    // Create success
    else
    {
        int play = 0;
        char command[MAX_LENGTH];
        do
        {
            printf("\n           Room Id: %s\n", arr[1]);
            printf("You are host of the room, let's start game!\n");
            printf("        Press [S] to start game\n");
            printf("        Press [Q] to quit game, return main screen!\n");
            printf("Press anything to waiting more players!!!\n");
            printf("====>");
            scanf("%s", command);
            if ((strcasecmp(command, "s") == 0) || (strcasecmp(command, "q") == 0))
            {
                memset(messageClient, 0, sizeof(messageClient));
                sprintf(messageClient, "15 %s %s", arr[1], command);
                ClientSendMessageToServer(sockfd);
                ClientReceiveMessageFromServer(sockfd);
                if (strcmp(messageClient, "start") == 0)
                {
                    memset(messageClient, 0, sizeof(messageClient));
                    sprintf(messageClient, "16");
                    ClientSendMessageToServer(sockfd);
                    CountTime("Game will start", 1);
                    system("clear");
                    freeMemory(arr, count);
                    return InGamePlay(sockfd);
                }
                else if (strcmp(messageClient, "-1") == 0)
                {
                    CountTime("Make other players out room", 3);
                    system("clear");
                    freeMemory(arr, count);
                    return GameFunction(sockfd);
                }
            }
            system("clear");
        } while (play != 1);
    }
    freeMemory(arr, count);
}

void JoinRoom(int sockfd)
{
    // In list room
    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "13 ");
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    char **lsRoom = NULL;
    int countRoom = string_split(messageClient, ' ', &lsRoom);
    if (strcmp(lsRoom[0], "-3") == 0)
    {
        printf("\nNo room exist\n");
        CountTime("Return play game screen to create room", 4);
        system("clear");
        freeMemory(lsRoom, countRoom);
        return GameFunction(sockfd);
    }
    else
    {
        printf("\nNumber room %s\n", lsRoom[1]);
        for (int i = 1; i <= atoi(lsRoom[1]); i++)
        {
            printf("Room %s have %s players in room\n", lsRoom[2 * i], lsRoom[2 * i + 1]);
        }
        freeMemory(lsRoom, countRoom);
        // Select room
        char inputRoomID[MAX_LENGTH];
        printf("Input room id to join room: ");
        scanf("%s", inputRoomID);
        memset(messageClient, 0, sizeof(messageClient));
        sprintf(messageClient, "4 %s", inputRoomID);
        ClientSendMessageToServer(sockfd);
        ClientReceiveMessageFromServer(sockfd);
        // Room not exist
        if (strcmp(messageClient, "-2") == 0)
        {
            printf("\nRoomID is not exist\n");
            CountTime("Return play game screen", 4);
            system("clear");
        }
        // Cannot join room cuz max player in room
        else if (strcmp(messageClient, "-1") == 0)
        {
            printf("\nCannot join room cuz max player in this room\n");
            CountTime("Return play game screen", 5);
            system("clear");
        }
        // Player in room
        else
        {
            memset(messageClient, 0, sizeof(messageClient));
            sprintf(messageClient, "14 %s", inputRoomID);
            ClientSendMessageToServer(sockfd);

            // Waiting mess from server to do something after
            int recvBytes;
            // Cho
            while ((recvBytes = recv(sockfd, messageClient, MESSAGE_MAX, 0)) > 0)
            {
                messageClient[recvBytes] = 0;
                char **arr = NULL;
                int count = string_split(messageClient, ' ', &arr);
                if (strcmp(arr[0], "-1") == 0)
                {
                    printf("\nHost out room so you must out the room\n");
                    CountTime("Return play game screen", 5);
                    system("clear");
                    freeMemory(arr, count);
                    return GameFunction(sockfd);
                }
                else if (strcmp(arr[0], "14") == 0)
                {
                    system("clear");
                    ViewWelcomeScreen();
                    printf("\n_____________Room Id: %s_____________\n", inputRoomID);
                    for (int i = 1; i <= atoi(arr[1]); i++)
                    {
                        printf("%s. %s\n", arr[2 * i], arr[2 * i + 1]);
                    }
                    printf("_____________________________________\n");
                    printf("\n Game will be started by host\n");
                    printf(" Press [Ctr + C] to quit game!\n");
                }
                else if (strcmp(arr[0], "15") == 0)
                {
                    memset(messageClient, 0, sizeof(messageClient));
                    sprintf(messageClient, "16");
                    ClientSendMessageToServer(sockfd);
                    CountTime("Game will start", 1);
                    system("clear");
                    freeMemory(arr, count);
                    return InGamePlay(sockfd);
                }
            }
        }
    }
    freeMemory(lsRoom, countRoom);
}

int ChangePassword(sockfd)
{
    char currentPassword[PASSWORD_MAX], newPassword[PASSWORD_MAX], renewPassword[PASSWORD_MAX];
    int check = 0;
    printf("Current password: ");
    scanf("%s", currentPassword);
    fflush(stdin);
    do
    {
        printf("New password: ");
        scanf("%s", newPassword);
        fflush(stdin);
        printf("Confirm new password: ");
        scanf("%s", renewPassword);
        fflush(stdin);
        if (strcmp(newPassword, renewPassword) == 0)
            check = 1;
        else
        {
            printf("New password and confirm new password not match!!!\n");
            if (Back("continue change password", "return") == 0)
            {
                system("clear");
                GameFunction(sockfd);
            }
            else
            {
                system("clear");
                printf("\n _________________Change Password_________________\n");
                printf("Current password: %s\n", currentPassword);
            }
        }
    } while (check != 1);

    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "5 %s %s", currentPassword, newPassword);
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);

    if (strcmp(messageClient, "1") == 0)
    {
        printf(" ____________Change password success!!!___________\n");
        int back = 0;
        do
        {
            back = Back("return", "continue stay here");
            if (back)
                system("clear");
            else
            {
                system("clear");
                printf("\n _________________Change Password_________________\n\n");
                printf("Current password: %s\n", currentPassword);
                printf("New password: %s\n", newPassword);
                printf("Confirm new password: %s\n", newPassword);
                printf(" ____________Change password success!!!___________\n\n");
            }
        } while (back != 1);
        return 1;
    }
    else
        return 0;
}

void ShowProfile(int sockfd)
{
    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "6 ");
    ClientSendMessageToServer(sockfd);
    printf("%s\n", messageClient);
    ClientReceiveMessageFromServer(sockfd);
    printf("%s\n", messageClient);
    char **arr = NULL;
    int count = string_split(messageClient, ' ', &arr);
    printf("\n _______________This is your profile______________\n");
    printf("|     Username: %-21s             |\n", arr[0]);
    printf("|     Password: %-21s             |\n", arr[1]);
    printf("|_________________________________________________|\n");
    freeMemory(arr, count);
    if (Back("return", "continue stay here"))
        system("clear");
    else
        ShowProfile(sockfd);
}

int LogOut(int sockfd)
{
    if (Back("logout", "continue"))
    {
        memset(messageClient, 0, sizeof(messageClient));
        sprintf(messageClient, "7 ");
        ClientSendMessageToServer(sockfd);
        ClientReceiveMessageFromServer(sockfd);
        if (strcmp(messageClient, "1") == 0)
            return 1;
        else
            return 0;
    }
    return 0;
}

int Back(char *want1, char *want2)
{
    char input[MAX_LENGTH];
    printf("\nDo you want %s?\n", want1);
    printf("Press [y] or [Y] to %s, other to %s: ", want1, want2);
    scanf("%s", input);
    fflush(stdin);
    if (strcasecmp(input, "y") == 0)
        return 1;
    else
    {
        system("clear");
        return 0;
    }
}

// Countdown time to do something
void CountTime(char *message, int time)
{
    for (int i = time; i > 0; i--)
    {
        printf("%s in %ds\n", message, i);
        sleep(1);
    }
}


int someone_won = 0;
int game_map[HEIGHT+10][WIDTH+10];
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

/*game function for server*/
//Function to create a snake
snake* MakeSnake(int player_no, int head_y, int head_x){
    
    //Place the snake on the map (matrix)
    pthread_mutex_lock(&map_lock);
    game_map[head_y][head_x]   = -player_no;
    game_map[head_y+1][head_x] = 
    game_map[head_y+2][head_x] = player_no;
    pthread_mutex_unlock(&map_lock);    
    
    //Create snake struct, set coordinates facing up
    snake* s = malloc(sizeof(snake));
    
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
void KillSnake(snake* s){

    //Set all snake coordinates to zero on map
    pthread_mutex_lock(&map_lock);
    game_map[s->head.y][s->head.x] = game_map[s->tail.y][s->tail.x] = 0;    
    int i;
    for(i = 0; i < s->length - 2; i++)
        game_map[s->body_segment[i].y][s->body_segment[i].x] = 0;
    pthread_mutex_unlock(&map_lock);

    //Free memory
    free(s);    
    s = NULL;
}

//Function for a snake to eat a fruit in front of it
void EatFruit(snake* s, direction d, int player_no){
    memmove(&(s->body_segment[1]), 
            &(s->body_segment[0]), 
            (s->length-2) * sizeof(coordinate));
    s->body_segment[0].y = s->head.y;
    s->body_segment[0].x = s->head.x; 
    s->body_segment[0].d = s->head.d;
    switch(d){
        case UP:{
            s->head.y = s->head.y-1;
            s->head.d = UP; 
            if(game_map[s->head.y][s->head.x + 1] == FRUIT){
                pthread_mutex_lock(&map_lock);
                game_map[s->head.y][s->head.x + 1] = 0;   
                pthread_mutex_unlock(&map_lock);        
            }
            break;
        }
        case DOWN:{
            s->head.y = s->head.y+1;
            s->head.d = DOWN; 
            if(game_map[s->head.y][s->head.x + 1] == FRUIT){
                pthread_mutex_lock(&map_lock);
                game_map[s->head.y][s->head.x + 1] = 0; 
                pthread_mutex_unlock(&map_lock);
            }
            break;
        }
        case LEFT:{
            s->head.x = s->head.x-1;
            s->head.d = LEFT;  
            break;
        }
        case RIGHT:{
            s->head.x = s->head.x+1;
            s->head.d = RIGHT;  
            break;
        }
        default: break;
    }
    pthread_mutex_lock(&map_lock);
    game_map[s->head.y][s->head.x] = -(s->player_no);
    game_map[s->body_segment[0].y][s->body_segment[0].x] = s->player_no;
    pthread_mutex_unlock(&map_lock);
    s->length++;
    game_map[HEIGHT+player_no][WIDTH+2] = s->length+10000;
    AddFruit();
}

//Function to move snake
void MoveSnake(snake* s, direction d){
    memmove(&(s->body_segment[1]), 
            &(s->body_segment[0]), 
            (s->length-2) * sizeof(coordinate));
    s->body_segment[0].y = s->head.y;
    s->body_segment[0].x = s->head.x; 
    s->body_segment[0].d = s->head.d;
    switch(d){
        case UP:{
            s->head.y = s->head.y-1;
            s->head.d = UP;            
            break;
        }
        case DOWN:{
            s->head.y = s->head.y+1;
            s->head.d = DOWN;  
            break;
        }
        case LEFT:{
            s->head.x = s->head.x-1;
            s->head.d = LEFT;  
            break;
        }
        case RIGHT:{
            s->head.x = s->head.x+1;
            s->head.d = RIGHT;  
            break;
        }
        default: break;
    }
    pthread_mutex_lock(&map_lock);
    game_map[s->head.y][s->head.x] = -(s->player_no);
    game_map[s->body_segment[0].y][s->body_segment[0].x] = s->player_no;
    game_map[s->tail.y][s->tail.x] = 0;
    pthread_mutex_unlock(&map_lock);

    s->tail.y = s->body_segment[s->length-2].y;
    s->tail.x = s->body_segment[s->length-2].x;
}

void PlayGame(int socket) {
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
	snake* player_snake = MakeSnake(player_no, head_y, head_x);


	//Variables for user input
	char key = UP;
	char key_buffer;
	char map_buffer[map_size];
	int bytes_sent, n;
	int  success = 1;

    memcpy(map_buffer, game_map, map_size);
    bytes_sent = 0;

    while(bytes_sent < map_size){         
        bytes_sent += send(socket, game_map, map_size, 0);
        if (bytes_sent < 0) error("ERROR writing to socket");
    }
	while(success){
        // Check if someone won
        if(someone_won)
            success = 0;
        printf("%d\n", game_map[0][0]);
        // Check if you are the winner
        if(player_snake->length-3 >= WINNER_LENGTH){
            someone_won = player_no;
            pthread_mutex_lock(&map_lock);
            game_map[0][0] = WINNER;
            pthread_mutex_unlock(&map_lock);
        } else if(game_map[0][0] != BORDER){
            pthread_mutex_lock(&map_lock);
            game_map[0][0] = someone_won;
            pthread_mutex_unlock(&map_lock);
        }

        //Copy map to buffer, and send to client
        memcpy(map_buffer, game_map, map_size);
        bytes_sent = 0;

        while(bytes_sent < map_size){         
            bytes_sent += send(socket, game_map, map_size, 0);
            if (bytes_sent < 0) error("ERROR writing to socket");
        }

        // Player key input
        bzero(&key_buffer, 1);
        n = read(socket, &key_buffer, 1);
        if (n <= 0)
            break;

        // If user key is a direction, then apply it
        key_buffer = toupper(key_buffer);   
        if (((key_buffer == UP) && !(player_snake->head.d == DOWN))
        || ((key_buffer == DOWN) && !(player_snake->head.d == UP))
        || ((key_buffer == LEFT) && !(player_snake->head.d == RIGHT)) 
        || ((key_buffer == RIGHT) && !(player_snake->head.d == LEFT)))
            key = key_buffer;

        switch(key){

            case UP:{
                if ((game_map[player_snake->head.y-1][player_snake->head.x] == 0) && 
                    !(game_map[player_snake->head.y-1][player_snake->head.x+1] == FRUIT)) {
                    MoveSnake(player_snake, UP);
                }
                else if((game_map[player_snake->head.y-1][player_snake->head.x] == FRUIT) || 
                    (game_map[player_snake->head.y-1][player_snake->head.x+1] == FRUIT)){
                    EatFruit(player_snake, UP, player_no);
                }
                else {
                    MoveSnake(player_snake, LEFT);
                    success = 0;
                }
                break;
            }

            case DOWN:{
                if ((game_map[player_snake->head.y+1][player_snake->head.x] == 0) && 
                    !(game_map[player_snake->head.y+1][player_snake->head.x+1] == FRUIT)) {
                    MoveSnake(player_snake, DOWN);
                }
                else if((game_map[player_snake->head.y+1][player_snake->head.x] == FRUIT) || 
                    (game_map[player_snake->head.y+1][player_snake->head.x+1] == FRUIT)){
                    EatFruit(player_snake, DOWN, player_no);
                }
                else {
                    MoveSnake(player_snake, DOWN);
                    success = 0;
                }
                break;
            }

            case LEFT:{
                if (game_map[player_snake->head.y][player_snake->head.x-1] == 0) {
                    MoveSnake(player_snake, LEFT);
                }
                else if(game_map[player_snake->head.y][player_snake->head.x-1] == FRUIT) {
                    EatFruit(player_snake, LEFT, player_no);

                }
                else{
                    MoveSnake(player_snake, LEFT);
                    success = 0;
                }
                break;
            }

            case RIGHT:{
                if (game_map[player_snake->head.y][player_snake->head.x+1] == 0) {
                    MoveSnake(player_snake, RIGHT);
                }
                else if (game_map[player_snake->head.y][player_snake->head.x+1] == FRUIT) {
                    EatFruit(player_snake, RIGHT, player_no);
                }
                else {
                    MoveSnake(player_snake, RIGHT);
                    success = 0;
                }
                break;
            }

            default: break;
        }   

    }
    // DeleteRoom(4);
    if (player_snake->length - 3 == WINNER_LENGTH) {
        fprintf(stderr, "Player %d had won!\n", player_no);
        // writeFile("nguoidung.txt", l);
        KillSnake(player_snake);
        game_map[HEIGHT+player_no][WIDTH+2] = 0;
        return 0;
    }
    else {
        fprintf(stderr, "Player %d had exited game!\n", player_no);
        KillSnake(player_snake);
        game_map[HEIGHT+player_no][WIDTH+2] = 0;
        return 0;
    }
}