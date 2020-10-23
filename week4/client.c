#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef enum {true,false} bool;


int checkNumber(char *str);

int main(int argc, char **argv){
    int sockfd, port, sendBytes, recvBytes;
    struct sockaddr_in servaddr;
    socklen_t len_serv;
    bool exit = false;
    
    if(argc != 3){
        printf("Error: cu phap khong hop le!\n");
        printf("CU phap: ./client <ip address> <port-number>\n");
        return 0;
    }
    
   
    if(checkNumber(argv[2]) == 0){
        printf("Error: parameter2 (port) phai la so!\n");
        return 0;
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0 ){
        perror("Error: loi tao socket\n");
        return 0;
    }
    port = atoi(argv[2]);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);   
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    len_serv = sizeof(servaddr);
    do{
        char account[50], password[30], new_password[30], buff[100];

        printf("\tEnter account: "); gets(account);
        if(strlen(account) == 0){
            printf("Da thoat chuong trinh\n");
            sendBytes = sendto(sockfd, account, strlen(account),0,(struct sockaddr*)&servaddr,len_serv);
            if(sendBytes <0 ){
                perror("Error: loi sendto (account)");
                //break;
            }
            break;
        }
        //gui account den server de xu ly
        sendBytes = sendto(sockfd, account, strlen(account),0,(struct sockaddr*)&servaddr,len_serv);
        if(sendBytes <0 ){
            perror("Error: loi sendto (account)");
            break;
        }
        // nhan thong bao tu server sau khi xu ly account
        recvBytes = recvfrom(sockfd,buff, sizeof(buff),0,(struct sockaddr*)&servaddr,&len_serv);
        if(recvBytes < 0){
            perror("Error: loi recvfrom buff");
            break;
        }
        buff[recvBytes] = '\0';
        if(strcmp(buff,"Insert password: ") != 0){
            puts(buff);
           
        }else{
            char message[100];
            printf("%s",buff);
            while(1){               
                gets(password);
                if(strlen(password) == 0){
                    exit =true;
                    printf("Da thoat chuong trinh\n");
                    sendBytes = sendto(sockfd,password,strlen(password),0,(struct sockaddr*)&servaddr,len_serv);
                    if(sendBytes < 0){
                        perror("Error: loi sendto (password)");
                        exit =true;
                        //break;
                    }
                    break;
                }
                sendBytes = sendto(sockfd,password,strlen(password),0,(struct sockaddr*)&servaddr,len_serv);
                if(sendBytes < 0){
                    perror("Error: loi sendto (password)");
                    exit = true;
                    break;
                }
                recvBytes = recvfrom(sockfd,message,sizeof(message),0,(struct sockaddr*)&servaddr,&len_serv);
                message[recvBytes] = '\0';
                printf("%s\n",message);
                
                
            }
        }
        printf("-------------------------------\n");
    }while(exit != true);
 
    
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


