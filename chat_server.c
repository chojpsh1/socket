/* socket programming 1:1 chat */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define BUFSIZE 1024

void msg_out_error(char* msg);
int main( int argc, char **argv )
{
	int serv_sock;
	int clnt_sock;
	char sendBuf[BUFSIZE];
	char recvBuf[BUFSIZE];
	int send_len;
	int recv_len;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	
	int clnt_addr_size;

	serv_sock = socket( PF_INET, SOCK_STREAM, 0 );	//create server socket

	if( argc != 2 ){
		printf("Usage : %s <port>\n", argv[0] );
		exit(1);
	}
	if( serv_sock == -1 )	//fail to create socket.
		msg_out_error("socket() error");

	memset( &serv_addr, 0, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;	//define kind of protocol
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if( bind( serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) == -1)
		msg_out_error("bind() error" );

	if( listen( serv_sock, 5 ) == -1 )
		msg_out_error("listen() error");
	
	printf("~~~~~1:1 chatting server is open. Waiting for client access..~~~~~");
	
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	
	if( clnt_sock == -1 )
		msg_out_error("accept() error");
	
	printf("~~~~~1:1 chatting start!~~~~~\n");
	printf("~~~~~If you want to exit, enter '/quit'~~~~~\n");

	printf("Connected from %s:%d...\n", inet_ntoa(clnt_addr.sin_addr), 
		ntohs(clnt_addr.sin_port));

	//reception
	while(1)
	{
		memset(recvBuf, 0, sizeof(recvBuf));
		
		//receive data
		ssize_t Received_num = recv(clnt_sock, recvBuf, BUFSIZE, 0);
		if( Received_num == -1 )
			msg_out_error("recv() error");

		//print received data
		printf("from %s : %s", inet_ntoa(clnt_addr.sin_addr), recvBuf);
		recvBuf[strlen(recvBuf)-1] = '\0';
		
		if(strcmp(recvBuf, "/quit") == 0)
			break;
		memset(recvBuf, 0, sizeof(recvBuf));

		memset(sendBuf, 0, sizeof(sendBuf));
		//input message
		printf("Server : ");
		fgets(sendBuf, BUFSIZE, stdin);

		//send input buffer
		if( send(clnt_sock, sendBuf, strlen(sendBuf),0) == -1 )
			msg_out_error("send() error");

		if(strcmp(sendBuf,"/quit\n") == 0 )
			break;
	}
	fputc('\n', stdout);
	printf("~~~~~1:1 chat has ended~~~~~\n");

	close(clnt_sock);
	close(serv_sock);

	return 0;
}

void msg_out_error(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
