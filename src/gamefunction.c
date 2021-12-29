#include "init.h"


char key = UP_KEY;
int game_result = ONGOING;

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
        printf("\n           Room Id: %s\n", arr[1]);
        printf("You are host of the room, let's start game!\n");
        printf("        Press [S] to start game\n");
        printf("        Press [Q] to quit game!\n");
        printf("====>");
        scanf("%s", command);
        memset(messageClient, 0, sizeof(messageClient));
        sprintf(messageClient, "15 %s %s", arr[1], command);
        ClientSendMessageToServer(sockfd);
        ClientReceiveMessageFromServer(sockfd);

            play = 1;
            return InGamePlay(sockfd);
            
    }
    
    int back = 0;
    do
    {
        back = Back("return", "continue stay here");
        if (back)
        {
            system("clear");
            memset(messageClient, 0, sizeof(messageClient));
            sprintf(messageClient, "10 %s", arr[1]);
            ClientSendMessageToServer(sockfd);
            ClientReceiveMessageFromServer(sockfd);
            return GameFunction(sockfd);
        }
    } while (back != 1);
}

void JoinRoom(int sockfd)
{
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
        // printf("\n Game will be started by host\n");
        // printf(" Press [Ctr + C] to quit game!\n");
        // printf(" Press any key to wait for more players...\n");
        
        //// Tu thoat
        // int back = 0;
        // do
        // {
        //     back = Back("return", "continue stay here");
        //     if (back)
        //     {
        //         system("clear");
        //         memset(messageClient, 0, sizeof(messageClient));
        //         sprintf(messageClient, "10 ");
        //         ClientSendMessageToServer(sockfd);
        //         return GameFunction(sockfd);
        //     }
        // } while (back != 1);

        // Waiting mess from server to do something after
        int recvBytes;
        // Cho
        while ((recvBytes = recv(sockfd, messageClient, MESSAGE_MAX, 0)) > 0)
        {
            messageClient[recvBytes] = 0;
            char **arr = NULL;
            int count = string_split(messageClient, ' ', &arr);
            if (strcmp(messageClient, "-1") == 0)
            {
                printf("\nHost out room so you must out the room\n");
                CountTime("Return play game screen", 5);
                system("clear");
                return GameFunction(sockfd);
            }
            else if(strcmp(messageClient, "14")){
                system("clear");
                ViewWelcomeScreen();
                printf("\n_____________Room Id: %s_____________\n", inputRoomID);
                for(int i = 1; i<=atoi(arr[1]); i++){
                    printf("%s. %s\n", arr[2*i], arr[2*i+1]);
                }
                printf("_____________________________________\n");
                printf("\n Game will be started by host\n");
                printf(" Press [Ctr + C] to quit game!\n");
            }

            else if (strcmp(messageClient, "15") == 0)
            {
                InGamePlay(sockfd);
                break;
            }
        }
        
    }
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
    ClientReceiveMessageFromServer(sockfd);
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

//Stevens, chapter 12, page 428: Create detatched thread
int make_thread(void* (*fn)(void *), void* arg){
    int             err;
    pthread_t       tid;
    pthread_attr_t  attr;

    err = pthread_attr_init(&attr);
    if(err != 0)
        return err;
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(err == 0)
        err = pthread_create(&tid, &attr, fn, arg);
    pthread_attr_destroy(&attr);
    return err;
}

void* write_to_server(void* arg){
    int sockfd = *(int *) arg;
    struct timespec ts;
    ts.tv_sec = REFRESH;
    ts.tv_nsec = ((int)(REFRESH * 1000) % 1000)  * 1000000;
    while(game_result == ONGOING){
        nanosleep(&ts, NULL);
        int n = write(sockfd, &key, 1);
        if(n < 0) 
            error("ERROR writing to socket.");
    }
    return 0;
}

void* update_screen(void* arg){    
    int  sockfd = *(int*) arg;
    int  bytes_read;
    int  game_map[HEIGHT+10][WIDTH+10];
    int  map_size = (HEIGHT+10) * (WIDTH+10) * sizeof(game_map[0][0]);
    char map_buffer[map_size];
    int  i, j, n;

    while(1){

        //Recieve updated map from server
        bytes_read = 0;
        bzero(map_buffer, map_size);
        while(bytes_read < map_size){
            n = read(sockfd, map_buffer + bytes_read, map_size - bytes_read);
            bytes_read += n;
        }
        memcpy(game_map, map_buffer, map_size);

        clear();
        box(win, 0, 0);
        refresh();
        wrefresh(win);
        //for each position in the array, check if it's a snake head or bodypart
        for(i = 1; i < HEIGHT-1; i++){
            for(j = 1; j < WIDTH-1; j++){
                int current = game_map[i][j];
                int colour = abs(current) % 10;
                attron(COLOR_PAIR(colour)); 
                if((current > 0) && (current != FRUIT) && current < 10000){               
                    mvprintw(i, j, "  ");
                    attroff(COLOR_PAIR(colour));
                }
                else if ((current < 0) && (current != FRUIT) && (current != WALL) && (current != WALL2)){
                    if(game_map[i-1][j] == -current)
                        mvprintw(i, j, "..");
                    else if(game_map[i+1][j] == -current)
                        mvprintw(i, j, "**");
                    else if(game_map[i][j-1] == -current)
                        mvprintw(i, j, " :");
                    else if(game_map[i][j+1] == -current)
                        mvprintw(i, j, ": ");
                    attroff(COLOR_PAIR(colour));
                }                
                else if (current == FRUIT){ 
                    attroff(COLOR_PAIR(colour));
                    mvprintw(i, j, "O");                    
                }
                else if (current == WALL){ 
                    attroff(COLOR_PAIR(colour));
                    mvprintw(i, j, "|");                    
                }
                else if (current == WALL2){ 
                    attroff(COLOR_PAIR(colour));
                    mvprintw(i, j, "_");                    
                }
                mvprintw(1, WIDTH+2, "   |Score|"); 
                for(int v = 1; v <= 10; v++){
                    if(game_map[HEIGHT+v][WIDTH+2] >= 10000){
                        if(v == 1){
                            mvprintw(v+1, WIDTH+2, "Red Snake: %d", game_map[HEIGHT+v][WIDTH+2] - 10003); 
                        }
                        else if(v == 2){
                            mvprintw(v+1, WIDTH+2, "Green Snake: %d", game_map[HEIGHT+v][WIDTH+2] - 10003); 
                        }
                        else if(v == 3){
                            mvprintw(v+1, WIDTH+2, "Yellow Snake: %d", game_map[HEIGHT+v][WIDTH+2] - 10003); 
                        }
                        else if(v == 4){
                            mvprintw(v+1, WIDTH+2, "Magenta Snake: %d", game_map[HEIGHT+v][WIDTH+2] - 10003); 
                        }
                        else if(v == 5){
                            mvprintw(v+1, WIDTH+2, "Cyan Snake: %d", game_map[HEIGHT+v][WIDTH+2] - 10003); 
                        }
                        else if(v == 6){
                            mvprintw(v+1, WIDTH+2, "B'Yellow Snake: %d", game_map[HEIGHT+v][WIDTH+2] - 10003); 
                        }
                        else if(v == 7){
                            mvprintw(v+1, WIDTH+2, "B'Magenta Snake: %d", game_map[HEIGHT+v][WIDTH+2] - 10003); 
                        }
                        else if(v == 8){
                            mvprintw(v+1, WIDTH+2, "B'Cyan Snake: %d", game_map[HEIGHT+v][WIDTH+2] - 10003); 
                        }
                        else if(v == 9){
                            mvprintw(v+1, WIDTH+2, "B'White Snake: %d", game_map[HEIGHT+v][WIDTH+2] - 10003); 
                        }
                    }
                }
                
            }
        }
        refresh();
    }

    return 0;
}
void InGamePlay(int sockfd){
    
    //Create Ncurses Window, with input, no echo and hidden cursor
    initscr();      
    cbreak();
    noecho();
    start_color();
    use_default_colors();    
    curs_set(0);

    //Set window to new ncurses window
    win = newwin(HEIGHT, WIDTH, 0, 0);
    //Snake colours
    init_pair(0, COLOR_WHITE, COLOR_BLUE);
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_YELLOW);
    init_pair(4, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(5, COLOR_WHITE, COLOR_CYAN);
    init_pair(6, COLOR_BLACK, COLOR_YELLOW);
    init_pair(7, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(8, COLOR_BLACK, COLOR_CYAN);
    init_pair(9, COLOR_BLACK, COLOR_WHITE);

    mvprintw((HEIGHT-20)/2 + 1, (WIDTH-58)/2,"     _______  __    _  _______  ___   _  _______   \n");
    mvprintw((HEIGHT-20)/2 + 2, (WIDTH-58)/2,"    |       ||  |  | ||   _   ||   | | ||       |  \n");
    mvprintw((HEIGHT-20)/2 + 3, (WIDTH-58)/2,"    |  _____||   |_| ||  |_|  ||   |_| ||    ___|  \n");
    mvprintw((HEIGHT-20)/2 + 4, (WIDTH-58)/2,"    | |_____ |       ||       ||      _||   |___   \n");
    mvprintw((HEIGHT-20)/2 + 5, (WIDTH-58)/2,"    |_____  ||  _    ||       ||     |_ |    ___|  \n");
    mvprintw((HEIGHT-20)/2 + 6, (WIDTH-58)/2,"     _____| || | |   ||   _   ||    _  ||   |___   \n");
    mvprintw((HEIGHT-20)/2 + 7, (WIDTH-58)/2,"    |_______||_|  |__||__| |__||___| |_||_______|  \n");
    mvprintw((HEIGHT-20)/2 + 10, (WIDTH-58)/2," Instructions:"); 
    mvprintw((HEIGHT-20)/2 + 12, (WIDTH-58)/2," - Use the keys [W], [A], [S], [D] to move your snake.");
    mvprintw((HEIGHT-20)/2 + 13, (WIDTH-58)/2," - Eat fruit to grow in length.");
    mvprintw((HEIGHT-20)/2 + 14, (WIDTH-58)/2," - Do not run in to other snakes, the game border, the game wall"); 
    mvprintw((HEIGHT-20)/2 + 15, (WIDTH-58)/2,"   or yourself.");
    mvprintw((HEIGHT-20)/2 + 16, (WIDTH-58)/2," - The first snake to reach length 10 wins!");
    mvprintw((HEIGHT-20)/2 + 17, (WIDTH-58)/2," - Press '.' to quit at any time.");
    mvprintw((HEIGHT-20)/2 + 19, (WIDTH-58)/2,"Press any key to start . . .");
    getch();

    make_thread(update_screen, &sockfd);
    make_thread(write_to_server, &sockfd);

    while(game_result == ONGOING){
        
        //Get player input with time out
        bzero(&key_buffer, 1);
        timeout(REFRESH * 1000);
        key_buffer = getch();
        key_buffer = toupper(key_buffer);
        if(key_buffer == '.'){
            game_result = INTERRUPTED;
            break;
        } else if((key_buffer == UP_KEY) || (key_buffer == DOWN_KEY) || (key_buffer == LEFT_KEY) || (key_buffer == RIGHT_KEY))
            key = key_buffer;
    }
}