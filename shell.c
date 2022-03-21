#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "client.h"
#define buffer 1024
#define tmpfd 400
#define stdfd 1


int isEcho(char* str,int size){
    if(size<4){
        return 0;
    }
    char echo[5] = {'E','C','H','O','\0'};

    char check[5];
    for(int i=0;i<4;i++){
        check[i]=str[i];
    }
    check[4]='\0';
    if(strcmp(echo,check)==0){
        return 1;
    }else{
        return 0;
    }
}

int main(){
    int flag =1;
    Client* client = NULL;
    char input[buffer];
    char tcp[] = {'T','C','P',' ','P','O','R','T','\0'};
    char local[] = {'L','O','C','A','L','\0'};
    char exit [] = {'E','X','I','T','\0'}; 
    const char green[] ="\033[0;32m"; 
    const char reset[] = "\033[0m";
    const char blue[] = "\033[0;34m";
    while(flag){
        char buff[buffer];
        getcwd(buff,buffer);
        printf(green);
        printf("user@user");
        printf(reset);
        printf(":");
        printf(blue);
        printf("~%s",buff);
        printf(reset);
        printf("$ ");
        scanf("%[^\n]%*c",input);
        /*
        Change file discriptor to socket so printf will send to socket.
        */
        if(!strcmp(tcp,input)){
            client = initClient();
            dup2(stdfd,tmpfd);
            dup2(client->_sock,stdfd);
        }
        if(!strcmp(local,input)){
            //printf("Local command -->\n");
            //sendmessage(client,exit,sizeof(exit)+1);
            //printf("Exit Sent <--\n");
            printf("EXIT");
            sleep(0.5);
            closesock(client);
            free(client);
            client = NULL;
            dup2(tmpfd,stdfd);
        }
        if(!strcmp(exit,input)){
            closesock(client);
            if(client){
                free(client);
            }
            break;
        }
        if(isEcho(input,sizeof(input))){
            char message[sizeof(input)-4];
            int k=0;
            for(int i=5;i<sizeof(input);i++){
                message[k++] = input[i];
            }
            printf("%s\n",message);           
        }    
    }
    return 0;
}