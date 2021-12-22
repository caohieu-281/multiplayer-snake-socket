#include "init.h"

void *connection_handler(void *client_socket)
{
	int socket = *(int *)client_socket;
	int send_status;
	int read_len;
	char **arr = NULL;
	int count;
	while ((read_len = recv(socket, messageServer, MESSAGE_MAX, 0)) > 0)
	{
		printf("String receive from client: %s\n", messageServer);

		count = string_split(messageServer, ' ', &arr);
		if (strcmp(arr[0], "1") == 0)
		{
			int check = checkSignIn(arr[1], arr[2]);
			if (check == 1)
			{
				strcpy(messageServer, "1");
				ServerSendToClient(socket);
			}
			else
			{
				strcpy(messageServer, "0");
				ServerSendToClient(socket);
			}
		}

		// Send the message back to client
		//  send_status=send(socket , message , strlen(message),0);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	readUserFromFile();
	int server_socket = ServerCreateSocket(atoi(argv[1]));
	int no_threads = 0;
	pthread_t threads[MAX_PLAYER];
	while (no_threads < MAX_PLAYER)
	{
		printf("Listening...\n");
		int client_socket = accept(server_socket, NULL, NULL);
		puts("Connection accepted");
		if (pthread_create(&threads[no_threads], NULL, connection_handler, &client_socket) < 0)
		{
			perror("Could not create thread");
			return 1;
		}
		if (client_socket < 0)
		{
			printf("server acccept failed...\n");
			exit(0);
		}
		else
			printf("Server acccept the client...\n");
		puts("Handler assigned");
		no_threads++;
	}
	int k = 0;
	for (k = 0; k < MAX_PLAYER; k++)
	{
		pthread_join(threads[k], NULL);
	}

	// int send_status;
	// send_status=send(client_socket, server_message, sizeof(server_message), 0);
	close(server_socket);

	return 0;
}