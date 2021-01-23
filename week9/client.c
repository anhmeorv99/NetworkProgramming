#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef enum {true,false} bool;

void menu();
int checkNumber(char *str);
struct myStruct {
        int a,b;
    }myStruct1;

int main(int argc, char **argv){
    int sockfd, port, sendBytes, recvBytes;
    struct sockaddr_in servaddr;
    socklen_t len_serv;
    bool exit = false;
    
    if(argc != 3){
        printf("Invalid grammar!\n");
        printf("Ex: ./client 127.0.0.1 9500\n");
        return 0;
    }
    
   
    if(checkNumber(argv[2]) == 0){
        printf("parameter2 must be number!\n");
        return 0;
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0 ){
        perror("Create socket error\n");
        return 0;
    }
    port = atoi(argv[2]);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);   
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 

    len_serv = sizeof(servaddr);
    do{

         char buff[1000],message[1000];
         void* myPtr;

        printf("Enter message: "); gets(message);
        if(strlen(message) == 0 || strcmp(message,"Bye")==0){
            printf("Program exit\n");
            sendBytes = send(sockfd, message, strlen(message),0);
            if(sendBytes <0 ){
                perror("Error send message");
                //break;
            }
            break;
        }
        //gui account den server de xu ly
        sendBytes = send(sockfd, buff, strlen(buff),0);
        recvBytes =  recv(sockfd,(int*)myPtr,sizeof(myStruct1),0);
        if(recvBytes < 0){
            perror("Error recv buff");
            break;
        }

        printf("int = %ls \n", (int*)myPtr);
        
    } while(exit != true);
 
    
    close(sockfd);
    return 0;
}


int checkNumber(char* str){
    int i;
    if(strlen(str) == 0) return 0;
    for(i = 0; i< strlen(str); i++){
        if(!(str[i] >= '0' && str[i] <= '9'))
            return 0;
    }
    return 1;
}
