#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(int argc , char *argv[])
{
	int socket_desc ;
	// , client_socket , c_name , read_size;
	int client_socket;
	int c_name = sizeof(struct sockaddr_in) ;
	int read;
	struct sockaddr_in server , client;
	char client_message[2000];

	// opening socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1){
		printf("Could not create socket");
	}
	puts("Socket created");

	// creating server struct
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	
	// binding
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
		perror("binding failed!");
		return 1;
	}
	// listening . . . . .
	listen(socket_desc , 3);
	puts("listening . . . . .");
	//accept connection from an incoming client
	client_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c_name);
	if (client_socket < 0)
	{
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");

	while( (read = recv(client_socket , client_message , 2000 , 0)) > 0 ){

		write(client_socket , client_message , strlen(client_message));
	}
	printf("password is :  %s",client_message);
	if(read == 0){
		puts("Client disconnected");
		fflush(stdout);
	}else if(read == -1){
		perror("recv failed");
	}

	
	return 0;
}