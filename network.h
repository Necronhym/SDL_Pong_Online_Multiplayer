#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
class NetworkServer
	{
		public:
		int sockfd, newsockfd, portno;
		socklen_t clilen;
		char buffer[256];
		struct sockaddr_in serv_addr, cli_addr;
		void OpenSocket( int Port )
			{
				sockfd = socket( AF_INET, SOCK_STREAM, 0 );
				bzero( &serv_addr, sizeof( serv_addr ) );
				portno = Port;
				serv_addr.sin_family = AF_INET;
				serv_addr.sin_port = htons(portno);
				serv_addr.sin_addr.s_addr = INADDR_ANY;
				bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) );	
				listen( sockfd, 5 );
				clilen = sizeof( cli_addr );
				newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen);
			}
		void Recive()
			{
				bzero( buffer, 256 );
				read( newsockfd, buffer, 256 );
		}
		void Send(char send[256])
			{
				write( newsockfd, send, 256 );
			}
		void CloseSocket()
			{
				close(newsockfd);
				close(sockfd);
			}
	};
class NetworkClient
	{
		public:
		int sockfd, portno;
		struct sockaddr_in serv_addr;
		struct hostent *server;
		char buffer[256];
		void OpenSocket( char* Host ,int Port )
			{
				portno = Port;
    				sockfd = socket(AF_INET, SOCK_STREAM, 0);
				server = gethostbyname( Host );
				bzero((char *) &serv_addr, sizeof(serv_addr));
				serv_addr.sin_family = AF_INET;
				bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
				serv_addr.sin_port = htons(portno);
				connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
			}
		void Recive()
			{
				bzero( buffer, 256 );
				read( sockfd, buffer, 256 );
			}
		void Send( char send[256])
			{
				write( sockfd, send, 256 );
			}
		void CloseSocket()
			{
				close(sockfd);
			}
};
