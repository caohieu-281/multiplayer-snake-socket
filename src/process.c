#include "init.h"

int string_split(const char *str, char c, char ***arr)
{
    printf("asdad");
    
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

void readUserFromFile()
{
    FILE *fin = fopen(FILE_DATA, "r");
    if (fin == NULL)
    {
        printf("Can't open file\n");
        return;
    }
    char readFile[MAX_LENGTH];
    char **arr;
    int id = -1;
    while (fgets(readFile, MAX_LENGTH, fin))
    {
        id++;
        int count = string_split(readFile, ' ', &arr);
        for (int i = 0; i < count; i++)
            if (arr[i][strlen(arr[i])-1] == '\n')
                arr[i][strlen(arr[i])-1] = 0;

        strcpy(listUsers[id].username, arr[0]);
        strcpy(listUsers[id].password, arr[1]);
    }
    numberUsers = id + 1;

    fclose(fin);
}

int checkSignIn(char *username, char *password)
{
    for (int i = 0; i < numberUsers; i++)
    {
        if (strcmp(username, listUsers[i].username) == 0)
        {
            if (strcmp(password, listUsers[i].password) == 0)
                // correct user
                return 1;
            else
                // wrong password
                return 0;
        }
    }
    // not find users
    return -1;
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

void sortScore()
{
    return;
}
