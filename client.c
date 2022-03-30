#include <stdio.h>
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 5070
#define SERVER_IP_ADDRESS "127.0.0.1"

typedef struct _client {
	int _sock;
    int _port;
    char* _addr;
}Client;

Client* initClient(){
    Client* client = (Client*)malloc(sizeof(Client));
    client->_addr = SERVER_IP_ADDRESS;
    client->_port = SERVER_PORT;
    client->_sock = socket(AF_INET, SOCK_STREAM, 0);

     if(client->_sock == -1)
    {
        printf("Could not create socket : %d",errno );
    }

    // "sockaddr_in" is the "derived" from sockaddr structure
    // used for IPv4 communication. For IPv6, use sockaddr_in6
    //
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
	int rval = inet_pton(AF_INET, (const char*)SERVER_IP_ADDRESS, &serverAddress.sin_addr);
	if (rval <= 0)
	{
		printf("inet_pton() failed");
		
	}

     // Make a connection to the server with socket SendingSocket.

     if (connect(client->_sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1)
     {
	   printf("connect() failed with error code : %d" ,errno);
     }

     printf("connected to server\n");
    return client;
}

int sendmessage(Client* client,char* message, size_t size){
    int bytesSent = send(client->_sock, message, size, 0);

     if (-1 == bytesSent)
     {
	printf("send() failed with error code : %d" ,errno);
     }
     else if (0 == bytesSent)
     {
	printf("peer has closed the TCP connection prior to send().\n");
     }
     else if (size > bytesSent)
     {
	printf("sent only %lu bytes from the required %d.\n", size, bytesSent);
     }
     else 
     {
	printf("message was successfully sent .\n");
     }
	return bytesSent;

}
void closesock(Client* client){
    close(client->_sock);
}
