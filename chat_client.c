#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024

void msg_out_error( char *msg );

int main( int argc, char *argv[] )
{
	int sock;
	char sendBuf[BUFSIZE];
	char recvBuf[BUFSIZE];
	size_t send_len;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	int clnt_addr_size;

	if( argc != 3 )
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	sock = socket( PF_INET, SOCK_STREAM, 0 );
	
	if( sock == -1 )
		msg_out_error("socket() error");
	
	memset( &serv_addr, 0, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if( connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1 )
		msg_out_error("connect() error");
	
	printf("~~~~~1:1 chatting start!~~~~~\n");
	printf("~~~~~If you want to exit, enter '/quit'~~~~~\n");

	while(1)
	{
		//create buffer to input
		memset(sendBuf, 0, strlen(sendBuf));

		printf("Client : ");
		fgets(sendBuf, BUFSIZE, stdin);
	
		send_len = strlen(sendBuf);
		ssize_t comu_num = send( sock, sendBuf, send_len, 0);
		
		if( comu_num == -1 )
			msg_out_error("send() error");

		if( strcmp(sendBuf, "/quit\n") == 0 )
			break;

		memset(recvBuf, 0, sizeof(recvBuf));
		comu_num = recv(sock, recvBuf, BUFSIZE, 0);
		
		if(comu_num == -1 )
			msg_out_error("recv() error");

		if( strcmp(recvBuf, "/quit\n") == 0 )
			break;

		printf("from %s : %s", inet_ntoa(serv_addr.sin_addr),recvBuf);
		memset(recvBuf, 0, sizeof(recvBuf));
	}

	fputc('\n', stdout);
	printf("~~~~~1:1 chat has ended~~~~~\n");

	close(sock);

	return 0;
}

void msg_out_error(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
