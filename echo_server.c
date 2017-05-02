#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 1024

void msg_out_error(char* msg);

int main(int argc, char **argv)
{
	int serv_sock;
	int clnt_sock;
	char msg[BUFSIZE];
	int str_len;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	
	if( argc != 2 ){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);	//create server socket
	
	if( serv_sock == -1 )	//fali to create socket, it returns -1
		msg_out_error("socket() error");
	
	memset( &serv_addr, 0, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;	//define kind of protocol
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	/*set IP address. INADDR_ANY is automatically founding IP address of server and setting function*/
	serv_addr.sin_port = htons(atoi(argv[1]));

	//allocate address at socekt
	if( bind( serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) == -1 )
		msg_out_error("bind() error");
	//state of connection request waiting
	if( listen( serv_sock, 5 ) == -1 )
		msg_out_error("listen() error");
	
	clnt_addr_size = sizeof(clnt_addr);

	//accepting connection request
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if( clnt_sock == -1 )
		msg_out_error("accept() error");
	
	//data receipt and transmission
	while( (str_len = read(clnt_sock, msg, BUFSIZE)) != 0 ){ //until EOF
		write( clnt_sock, msg, str_len );
		write( 1, msg, str_len );
	}
	close(clnt_sock);	//end of connection

	return 0;
}

void msg_out_error(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
