#include "init.h"


int ClientCreateSocket(int port)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
        exit(0);
    }

    // Specify an address for the socket
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect the client to the server socket
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("ERROR connect to server");
        exit(0);
    }

    return sockfd;
}

void ClientSendMessageToServer(int sockfd)
{
    int sendBytes = send(sockfd, messageClient, strlen(messageClient), 0);
    if (sendBytes < 0) {
        perror("Not send message to server");
        exit(0);
    }
}

void ClientReceiveMessageFromServer(int sockfd)
{
    int rcvBytes = recv(sockfd, &messageClient, strlen(messageClient), 0);
    messageClient[rcvBytes] = 0;
    if (rcvBytes == 0) {
        perror("The server terminated prematurely");
        exit(0);
    }
}

int ServerCreateSocket(int port)
{
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        perror("Socket initialisation failed");
        exit(0);
    }
    else
        printf("Server socket created successfully\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // bind the socket to the specified IP addr and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if (listen(server_socket, 3) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    return server_socket;
}

void ServerSendToClient(int socket)
{
    int send_status = send(socket , messageServer , strlen(messageServer), 0);
    if(send_status < 0)
    {
        perror("Not send message to user\n");
    }
}

void ServerReceiveMessageFromClient(int socket)
{
    int read_len = recv(socket, messageServer, MESSAGE_MAX, 0)
    if(read_len < 0)
    {
        perror("Not receive message from user\n");
    }
    else messageServer[read_len] = 0;
}