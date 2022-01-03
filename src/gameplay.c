#include "init.h"

char key_buffer;
char key = UP_KEY;
int game_result = ONGOING;
WINDOW* win;

//Stevens, chapter 12, page 428: Create detatched thread
int make_thread(void *(*fn)(void *), void *arg)
{
    int err;
    pthread_t tid;
    pthread_attr_t attr;

    err = pthread_attr_init(&attr);
    if (err != 0)
        return err;
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err == 0)
        err = pthread_create(&tid, &attr, fn, arg);
    pthread_attr_destroy(&attr);
    return err;
}

void* write_to_server(void *arg)
{
    int sockfd = *(int *)arg;
    struct timespec ts;
    ts.tv_sec = REFRESH;
    ts.tv_nsec = ((int)(REFRESH * 1000) % 1000) * 1000000;
    while (game_result == ONGOING)
    {
        nanosleep(&ts, NULL);
        int n = send(sockfd, &key, 1, 0);
        if (n < 0)
            error("ERROR writing to socket.");
    }
    return 0;
}

void* update_screen(void *arg)
{
    int sockfd = *(int *)arg;
    int bytes_read;
    int game_map[HEIGHT + 10][WIDTH + 10];
    int map_size = (HEIGHT + 10) * (WIDTH + 10) * sizeof(game_map[0][0]);
    char map_buffer[map_size];
    int i, j, n;
    char message[MAX_LENGTH];

    while (game_result == ONGOING)
    {

        //Recieve updated map from server
        bytes_read = 0;
        bzero(map_buffer, map_size);
     
        while (bytes_read < map_size)
        {
            n = recv(sockfd, map_buffer + bytes_read, map_size - bytes_read, 0);
            bytes_read += n;
        }

        memcpy(game_map, map_buffer, map_size);

        if (game_map[0][0] != BORDER) break; 

        clear();
        box(win, 0, 0);
        refresh();
        wrefresh(win);
        // for each position in the array, check if it's a snake head or bodypart
        for (i = 1; i < HEIGHT - 1; i++)
        {
            for (j = 1; j < WIDTH - 1; j++)
            {
                int current = game_map[i][j];
                int colour = abs(current) % 10;
                attron(COLOR_PAIR(colour));
                if ((current > 0) && (current != FRUIT) && current < 10000)
                {
                    mvprintw(i, j, "  ");
                    attroff(COLOR_PAIR(colour));
                }
                else if ((current < 0) && (current != FRUIT) && (current != WALL) && (current != WALL2))
                {
                    if (game_map[i - 1][j] == -current)
                        mvprintw(i, j, "..");
                    else if (game_map[i + 1][j] == -current)
                        mvprintw(i, j, "**");
                    else if (game_map[i][j - 1] == -current)
                        mvprintw(i, j, " :");
                    else if (game_map[i][j + 1] == -current)
                        mvprintw(i, j, ": ");
                    attroff(COLOR_PAIR(colour));
                }
                else if (current == FRUIT)
                {
                    attroff(COLOR_PAIR(colour));
                    mvprintw(i, j, "O");
                }
                else if (current == WALL)
                {
                    attroff(COLOR_PAIR(colour));
                    mvprintw(i, j, "|");
                }
                else if (current == WALL2)
                {
                    attroff(COLOR_PAIR(colour));
                    mvprintw(i, j, "_");
                }
                mvprintw(1, WIDTH + 2, "   |Score|");
                for (int v = 1; v <= 10; v++)
                {
                    if (game_map[HEIGHT + v][WIDTH + 2] >= 10000)
                    {
                        if (v == 1)
                        {
                            mvprintw(v + 1, WIDTH + 2, "Red Snake: %d", game_map[HEIGHT + v][WIDTH + 2] - 10003);
                        }
                        else if (v == 2)
                        {
                            mvprintw(v + 1, WIDTH + 2, "Green Snake: %d", game_map[HEIGHT + v][WIDTH + 2] - 10003);
                        }
                        else if (v == 3)
                        {
                            mvprintw(v + 1, WIDTH + 2, "Yellow Snake: %d", game_map[HEIGHT + v][WIDTH + 2] - 10003);
                        }
                        else if (v == 4)
                        {
                            mvprintw(v + 1, WIDTH + 2, "Magenta Snake: %d", game_map[HEIGHT + v][WIDTH + 2] - 10003);
                        }
                        else if (v == 5)
                        {
                            mvprintw(v + 1, WIDTH + 2, "Cyan Snake: %d", game_map[HEIGHT + v][WIDTH + 2] - 10003);
                        }
                        else if (v == 6)
                        {
                            mvprintw(v + 1, WIDTH + 2, "B'Yellow Snake: %d", game_map[HEIGHT + v][WIDTH + 2] - 10003);
                        }
                        else if (v == 7)
                        {
                            mvprintw(v + 1, WIDTH + 2, "B'Magenta Snake: %d", game_map[HEIGHT + v][WIDTH + 2] - 10003);
                        }
                        else if (v == 8)
                        {
                            mvprintw(v + 1, WIDTH + 2, "B'Cyan Snake: %d", game_map[HEIGHT + v][WIDTH + 2] - 10003);
                        }
                        else if (v == 9)
                        {
                            mvprintw(v + 1, WIDTH + 2, "B'White Snake: %d", game_map[HEIGHT + v][WIDTH + 2] - 10003);
                        }
                    }
                }
            }
        }
        refresh();
    }

    game_result = game_map[0][0];
    return 0;
}

void InGamePlay(int sockfd)
{

    //Create Ncurses Window, with input, no echo and hidden cursor
    initscr();
    cbreak();
    noecho();
    start_color();
    use_default_colors();
    curs_set(0);

    // //Set window to new ncurses window
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

    mvprintw((HEIGHT - 20) / 2 + 1, (WIDTH - 58) / 2, "     _______  __    _  _______  ___   _  _______   \n");
    mvprintw((HEIGHT - 20) / 2 + 2, (WIDTH - 58) / 2, "    |       ||  |  | ||   _   ||   | | ||       |  \n");
    mvprintw((HEIGHT - 20) / 2 + 3, (WIDTH - 58) / 2, "    |  _____||   |_| ||  |_|  ||   |_| ||    ___|  \n");
    mvprintw((HEIGHT - 20) / 2 + 4, (WIDTH - 58) / 2, "    | |_____ |       ||       ||      _||   |___   \n");
    mvprintw((HEIGHT - 20) / 2 + 5, (WIDTH - 58) / 2, "    |_____  ||  _    ||       ||     |_ |    ___|  \n");
    mvprintw((HEIGHT - 20) / 2 + 6, (WIDTH - 58) / 2, "     _____| || | |   ||   _   ||    _  ||   |___   \n");
    mvprintw((HEIGHT - 20) / 2 + 7, (WIDTH - 58) / 2, "    |_______||_|  |__||__| |__||___| |_||_______|  \n");
    mvprintw((HEIGHT - 20) / 2 + 10, (WIDTH - 58) / 2, " Instructions:");
    mvprintw((HEIGHT - 20) / 2 + 12, (WIDTH - 58) / 2, " - Use the keys [W], [A], [S], [D] to move your snake.");
    mvprintw((HEIGHT - 20) / 2 + 13, (WIDTH - 58) / 2, " - Eat fruit to grow in length.");
    mvprintw((HEIGHT - 20) / 2 + 14, (WIDTH - 58) / 2, " - Do not run in to other snakes, the game border, the game wall");
    mvprintw((HEIGHT - 20) / 2 + 15, (WIDTH - 58) / 2, "   or yourself.");
    mvprintw((HEIGHT - 20) / 2 + 16, (WIDTH - 58) / 2, " - The first snake to reach length 10 wins!");
    mvprintw((HEIGHT - 20) / 2 + 17, (WIDTH - 58) / 2, " - Press '.' to quit at any time.");
    mvprintw((HEIGHT - 20) / 2 + 19, (WIDTH - 58) / 2, "Press any key to start . . .");
    getch();

    make_thread(update_screen, &sockfd);
    make_thread(write_to_server, &sockfd);

    while (game_result == ONGOING)
    {
        //Get player input with time out
        bzero(&key_buffer, 1);
        timeout(REFRESH * 1000);
        key_buffer = getch();
        key_buffer = toupper(key_buffer);
        if (key_buffer == '.')
        {
            game_result = INTERRUPTED;
            break;
        }
        else if ((key_buffer == UP_KEY) || (key_buffer == DOWN_KEY) || (key_buffer == LEFT_KEY) || (key_buffer == RIGHT_KEY))
            key = key_buffer;
    }
    //Show the user who won
    WINDOW* announcement = newwin(7, 35, (HEIGHT - 7)/2, (WIDTH - 35)/2);
    box(announcement, 0, 0);
    mvwaddstr(announcement, 2, (35-21)/2, "Game Over - You WIN!");
    if (game_result == WINNER){
        mvwaddstr(announcement, 2, (35-21)/2, "Game Over - You WIN!");
        mvwaddstr(announcement, 4, (35-21)/2, "Press any key to quit.");
        wbkgd(announcement,COLOR_PAIR(2));
    } else{
        mvwaddstr(announcement, 2, (35-21)/2, "Game Over - you lose!");
        if(game_result > 0)
            mvwprintw(announcement, 3, (35-13)/2, "Player %d won.", game_result);
        mvwaddstr(announcement, 4, (35-21)/2, "Press any key to quit.");
        wbkgd(announcement,COLOR_PAIR(1));
    }
    mvwin(announcement, (HEIGHT - 7)/2, (WIDTH - 35)/2);
    wnoutrefresh(announcement);
    wrefresh(announcement);
    sleep(2);
    wgetch(announcement);
    delwin(announcement);
    wclear(win);

    echo(); 
    curs_set(1);  
    endwin();
    // pthread_exit(NULL);
    // memset(messageClient, 0, sizeof(messageClient));
    // sprintf(messageClient, "17");
    // ClientSendMessageToServer(sockfd);
    // return GameFunction(sockfd);
}