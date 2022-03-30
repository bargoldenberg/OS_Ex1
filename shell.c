#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "client.h"
#include <dirent.h>
#include <sys/wait.h>

#define SIZE 1024*1024
#define tmpfd 400
#define buffer 1024

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

int isCPY(char* str, int size){
    char copy [] = {'C','O','P','Y','\0'};
    char cmp [5];
    if(size<5){
        return 0;
    }
    for(int i=0;i<4;i++){
        cmp[i]=str[i];
    }
    cmp[4]='\0';
    if(strcmp(cmp,copy)==0){
        return 1;
    }else{
        return 0;
    }
}

int isDel(char* str,int size){
    char del [] = {'D','E','L','E','T','E','\0'};
    char cmp [7];
    if(size<7){
        return 0;
    }
    for(int i=0;i<6;i++){
        cmp[i]=str[i];
    }
    cmp[6] = '\0';
    for(int i=0;i<sizeof(cmp);i++){
        if(cmp[i]!=del[i]){
            return 0;
        }
    }
    printf("one");
    return 1;
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
            continue;
        }
        if(!strcmp(local,input)){
            printf("%s\n",exit);
            closesock(client);
            client = NULL;
            dup2(tmpfd,STDOUT_FILENO);
            continue;
        }
        if(!strcmp(exit,input)){
            if(client!=NULL){
                close(client->_sock);
                free(client);
                client = NULL;
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
            continue;           
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
            continue;
        }
        /*
        fread fwrite and fopen are not system calls they are library functions that use system calls.
        */
        if(isCPY(input,sizeof(input))==1){
            char* token;
            token = strtok(input," ");
            token = strtok(NULL," ");
            char* src = token;
            token = strtok(NULL," ");
            char* dest = token;

            FILE* fsrc = fopen(src, "r");
            FILE* fdest = fopen(dest,"w");
            char tmpbuffer[SIZE];
            size_t bytes=1;
            while (bytes!=0){
                bytes = fread(tmpbuffer, 1, sizeof(tmpbuffer), fsrc);
                fwrite(tmpbuffer, 1, bytes, fdest);
                printf("in here");
            }
            fclose(fsrc);
            fclose(fdest);
            continue;
        }
        /*
        Unlink is a system call.
        */
        if(isDel(input,sizeof(input))){
            printf("in\n");
            char* token;
            token = strtok(input," ");
            token = strtok(NULL," ");
            char* path = token;
            unlink(path);
            continue;
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
        //      * No system() is not a system call it is a function from the standard library, as we can see we basicly implemented system below. 
        //      * 
        //      */
        //     system(input);
        // }
        else{
            int pid = fork();
            if(pid<0){
                printf("could not fork");
                break;
            }
            if(pid==0){
                execl("/bin/sh", "sh", "-c", input, (char *) NULL);
                break;
            }else{
                wait(NULL);
            }
        }
    
    }
    return 0;
}