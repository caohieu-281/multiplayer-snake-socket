#include "init.h"

void ViewWelcomeScreen()
{
    system("clear");
    printf("     _______  __    _  _______  ___   _  _______   \n");
    printf("    |       ||  |  | ||   _   ||   | | ||       |  \n");
    printf("    |  _____||   |_| ||  |_|  ||   |_| ||    ___|  \n");
    printf("    | |_____ |       ||       ||      _||   |___   \n");
    printf("    |_____  ||  _    ||       ||     |_ |    ___|  \n");
    printf("     _____| || | |   ||   _   ||    _  ||   |___   \n");
    printf("    |_______||_|  |__||__| |__||___| |_||_______|  \n");
}

void ViewLoginScreen()
{
    printf("\n---------------Welcome to snake game---------------\n");
    printf(" _________________________________________________ \n");
    printf("|                 => [1]. Sign In                 |\n");
    printf("|                 => [2]. Sign Up                 |\n");
    printf("|                 => [3]. Quit game               |\n");
    printf("|_________________________________________________|\n");
    printf("Your Choose: ");
}

void ViewFunctionInGameScreen()
{
    printf(" _________________________________________________\n");
    printf("|             => [1]. Create Room                 |\n");
    printf("|             => [2]. Join waiting-room           |\n");
    printf("|             => [3]. Change password             |\n");
    printf("|             => [4]. Show profile                |\n");
    printf("|             => [5]. Show leaderboard            |\n");
    printf("|             => [6]. Log out                     |\n");
    printf("|_________________________________________________|\n");
    printf("Your Choose: ");
}
