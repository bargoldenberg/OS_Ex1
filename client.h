#pragma once
#include <stdlib.h>


/*
    Client struct.
*/
struct _client;
// typedef struct _client Client;

typedef struct _client {
	int _sock;
    int _port;
    char* _addr;
}Client;

Client* initClient();

int sendmessage(Client* client,char* message,size_t size);

void closesock(Client* client);