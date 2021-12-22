#include "init.h"

void *connection_handler(void *client_socket){
	int socket = *(int*) client_socket;
	int read_len;
	char client_message[100];
	int send_status;
	while( (read_len=recv(socket,client_message, 100,0))>0)
	{
		//end of string marker
		client_message[read_len] = '\0';
		printf("String receive from client: %s\n", client_message);
        for(int i=0;i<read_len;i++)
            client_message[i] = toupper(client_message[i]);
		printf("%s", client_message);
		if(strcmp(client_message,"Q")==0)	break;
		//Send the message back to client
		send_status=send(socket , client_message , strlen(client_message),0);	
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
    
    int server_socket = ServerCreateSocket(atoi(argv[1]));
    int no_threads=0;
    pthread_t threads[MAX_PLAYER];
    while (no_threads < MAX_PLAYER){
		printf("Listening...\n");
		int client_socket = accept(server_socket, NULL, NULL);
		puts("Connection accepted");
		if( pthread_create( &threads[no_threads], NULL ,  connection_handler , &client_socket) < 0){
			perror("Could not create thread");
			return 1;
		}
		if (client_socket < 0) { 
			printf("server acccept failed...\n"); 
			exit(0); 
			} 
		else
			printf("Server acccept the client...\n");
		puts("Handler assigned");
		no_threads++;
	}
	int k=0;
    for (k=0;k<MAX_PLAYER;k++){
		pthread_join(threads[k],NULL);
	}

    //int send_status;
    //send_status=send(client_socket, server_message, sizeof(server_message), 0);
    close(server_socket);
    
    return 0;
}