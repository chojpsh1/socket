#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 1024

void msg_out_error( char *msg );

int main( int argc, char **argv )
{
	int sock;
	struct sockaddr_in serv_addr;
	char msg[BUFSIZE];
	int str_len;

	if( argc != 3 ){
		printf("Usage : %s <IP> <port> \n", argv[0] );
		exit(1);
	}

	//create for server connection
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if( sock == -1 )
		msg_out_error("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;	//define protocol
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if( connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1 )
		msg_out_error("connect() error");

	while(1){
		fputs("Enter message to transmit (q to quit) :", stdout);
		fgets(msg, BUFSIZE, stdin);

		if( !strcmp(msg, "q\n") )
			break;
		write( sock, msg, strlen(msg) );

		//print message
		str_len = read(sock, msg, BUFSIZE-1);
		msg[str_len] = 0;
		printf("Transmitted message from server : %s\n", msg);
	}
	
	close(sock);
	return 0;
}

void msg_out_error(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
