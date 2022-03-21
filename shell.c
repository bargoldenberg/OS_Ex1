#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "client.h"
#include <dirent.h>


#define buffer 1024
#define tmpfd 400


int isCD(char* str,int size){
    if(size<2){
        return 0;
    }
    char cd[] = {'C','D','\0'};
    char check[3];
    for(int i=0;i<2;i++){
        check[i]=str[i];
    }
    check[2]='\0';
    if(strcmp(cd,check)==0){
        return 1;
    }else{
        return 0;
    }
}

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
    char dir[] = {'D','I','R','\0'};
    char exit [] = {'E','X','I','T','\0'}; 

    // const char green[] ="\033[0;32m"; 
    const char blue[] ="\e[0;36m"; // Fake green one !!!!!
    const char reset[] = "\033[0m";
    // const char blue[] = "\033[0;34m" 
    const char purple[] = "\033[0;35m";  // Fake blue one !!!!!

    while(flag){
        char buff[buffer];
        getcwd(buff,buffer);
        printf(blue);
        printf("user@user");
        printf(reset);
        printf(":");
        printf(purple);
        printf("~%s",buff);
        printf(reset);
        printf("$ ");
        scanf("%[^\n]%*c",input);
        /*
        Change file discriptor to socket so printf will send to socket.
        */
        if(!strcmp(tcp,input)){
            client = initClient();
            dup2(STDOUT_FILENO,tmpfd);
            dup2(client->_sock,STDOUT_FILENO);
        }
        if(!strcmp(local,input)){
            printf("%s\n",exit);
            closesock(client);
            client = NULL;
            dup2(tmpfd,STDOUT_FILENO);
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
        if(!strcmp(dir,input)){
            DIR *folder;
            struct dirent *entry;
            int files = 0;

            folder = opendir(".");
            if(folder == NULL){
                perror("Unable to read directory");
            }
            while( (entry=readdir(folder)) ){
                files++;
                printf("File %3d: %s\n",files,entry->d_name);
            }
            closedir(folder);
        }
        if(isCD(input,sizeof(input))){
            char message[sizeof(input)-1]={0};
            int k=0;
            // message[k++] = '';
            for(int i=3;i<sizeof(input);i++){
                message[k++] = input[i];
            }
            int err = chdir(message);
            if(err == -1){
                printf("Directory Not Exists.\n");
            }
            /**
             * Chdir is a system call (system function).
             * Chdir is used for changing the current directory (similar to the shell command 'cd').
             */
        } 
        // else {
        //     /**
        //      * NEED TO ADDD ANSWER!!!
        //      * 
        //      */
        //     system(input);
        // }
        else{
            // char * token = strtok(input, " ");

        }
    
    }
    return 0;
}