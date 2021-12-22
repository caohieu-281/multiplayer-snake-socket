typedef struct _User{
    char usename[MAX];
    char password[MAX];
    struct _User *pNext;
} User;

typedef struct List{
    User *pHead;
    User *pTail;
} List;

void InitList(List *l){
    l->pHead = l->pTail = NULL;
}

int string_split (const char *str, char c, char ***arr)
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

    *arr = (char**) malloc(sizeof(char*) * count);
    // if (*arr == NULL)
    //     exit(1);

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
        {
            (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
            // if ((*arr)[i] == NULL)
            //     exit(1);

            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }
    (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
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

void readFile(char *file_name, char){
    FILE *fin = fopen(file_name, "r");

    if (fin == NULL) {
        printf("Can't open file\n");
        return;
    }
    char *token;
    char tmp[MAX];
    char tmp2[MAX];
    char usename[MAX];
    char password[MAX];
    int status;
    int win_times;
    const char space[2] = " ";

    while(!feof(fin)){
        fgets(tmp2, 256, fin);
        token = strtok(tmp2, space);
        strcpy(usename, token);
        token = strtok(NULL, space);
        strcpy(password, token);
        token = strtok(NULL, space);
        strcpy(tmp, token);
        if(strlen(tmp) > 1){
            status = (tmp[0] - '0')*10 + (tmp[1] - '0');
        }
        else status = tmp[0] - '0';
        token = strtok(NULL, space);
        strcpy(tmp, token);
        // printf("%s---\n",tmp);
        if(tmp[strlen(tmp)-1] == '\n')  tmp[strlen(tmp)-1] = '\0';
        if(strlen(tmp) > 1){
            win_times = (tmp[0] - '0')*10 + (tmp[1] - '0');
        }
        else win_times = tmp[0] - '0';
        User *p = makeUser(usename, password, status, win_times);
        addUser(l, p);
    }
    fclose(fin);
}

void sortListStatus(List *clas){
    List stu1, stu2;
    if(clas->pHead == clas->pTail) return;
    InitList(&stu1);  InitList(&stu2);
    User *p;
    User *tag;
    tag = clas->pHead;
    clas->pHead = clas->pHead->pNext;
    tag->pNext = NULL;
    while(clas->pHead != NULL){
        p = clas->pHead;
        clas->pHead = clas->pHead->pNext;
        p->pNext = NULL;
        if(p->status >= tag->status) {
            addUser(&stu1, p);
        }
        else addUser(&stu2, p);
    }
    sortListStatus(&stu1);
    sortListStatus(&stu2);
    if(stu1.pHead != NULL){
        clas->pHead = stu1.pHead;
        stu1.pTail->pNext = tag;
    }
    else{
        clas->pHead = tag;
    }
    tag->pNext = stu2.pHead;
    if(stu2.pHead != NULL){
        clas->pTail = stu2.pTail;
    }
    else{
        clas->pTail = tag;
    }
}
