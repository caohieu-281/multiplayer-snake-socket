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

void UserOutRoom(int roomID, int userID)
{
    
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

void sortScore()
{
    return;
}