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
        char username[30], password[30], new_password[30], buff[100];

        printf("Enter username: "); gets(username);
        if(strlen(username) == 0 || strcmp(username,"Bye")==0){
            printf("Program exit\n");
            sendBytes = send(sockfd, username, strlen(username),0);
            if(sendBytes <0 ){
                perror("Error send account");
                //break;
            }
            break;
        }
        //gui account den server de xu ly
        sendBytes = send(sockfd, username, strlen(username),0);
      
        // nhan thong bao tu server sau khi xu ly account
        recvBytes = recv(sockfd,buff, sizeof(buff),0);
        if(recvBytes < 0){
            perror("Error recv buff");
            break;
        }
        buff[recvBytes] = '\0';
        if(strcmp(buff,"Insert password: ") != 0){
            puts(buff);
            {
                char sign_in_in[100] = "Tai khoan \'";
                strcat(sign_in_in,username);
                strcat(sign_in_in,"\' dang Dang Nhap");
                if(strcmp(buff,sign_in_in) == 0){
                    char bye[100];
                    printf("\tNhap \"bye\" de dang xuat(other : continue): ");
                    gets(bye);
                    send(sockfd,bye,strlen(bye),0);
                    if(strlen(bye) == 0){
                        printf("Da thoat chuong trinh\n");
                        break;
                    }
                    if(strcmp(bye,"bye") == 0){
                        char message[100];
                        recvBytes = recv(sockfd,message,sizeof(message),0);
                        
                        if(recvBytes < 0){
                            perror("Error: loi recv (in bye)");
                            break;
                        }
                        message[recvBytes] ='\0';
                        puts(message);
                    }
                }
            }
        }else{
            char message[100];
            printf("%s",buff);
            while(1){               
                gets(password);
                if(strlen(password) == 0){
                    exit =true;
                    printf("Da thoat chuong trinh\n");
                    sendBytes = send(sockfd,password,strlen(password),0);
                    if(sendBytes < 0){
                        perror("Error: loi send (password)");
                        exit =true;
                        //break;
                    }
                    break;
                }
                sendBytes = send(sockfd,password,strlen(password),0);
                if(sendBytes < 0){
                    perror("Error: loi send (password)");
                    exit = true;
                    break;
                }
                recvBytes = recv(sockfd,message,sizeof(message),0);
                message[recvBytes] = '\0';
                if(strcmp(message,"OK") == 0){
                    puts(message);
                    printf("\tEnter new password: ");
                    gets(new_password);
                    if(strlen(new_password) == 0){
                        exit = true;
                        printf("Da thoat chuong trinh\n");
                        sendBytes = send(sockfd,new_password,strlen(new_password),0);
                        if(sendBytes < 0){
                            perror("Error: loi send (new password)");
                            exit =true;
                            //break;
                        }
                        break;
                    }
                    //gui new passwrod den server
                    sendBytes = send(sockfd,new_password,strlen(new_password),0);
                    if(sendBytes < 0){
                        perror("Error: loi send (new password)");
                        exit =true;
                        break;
                    }
                    {
                        char old_number[30], old_string[30];
                        recvBytes = recv(sockfd,old_number,sizeof(old_number),0);
                        if(recvBytes < 0){
                            perror("Error: loi recv (old_number)");
                            exit = true;
                            break;
                        }
                        old_number[recvBytes] = '\0';
                        puts(old_number);
                        if(strlen(old_number) == 0 || checkNumber(old_number) == 1){ // xau old_number : "" or "<number>" -> OK
                            recvBytes = recv(sockfd,old_string,sizeof(old_string),0);
                            if(recvBytes < 0){
                                perror("Error: loi recv (old_number)");
                                exit = true;
                                break;
                            }
                            old_string[recvBytes] = '\0';
                            puts(old_string);
                        }
                    }
                    break;
                }else{
                    if(strcmp(message,"Not OK") == 0){
                        char in_passw[100];
                        puts(message);
                        recvBytes = recv(sockfd,in_passw,sizeof(in_passw),0);
                        in_passw[recvBytes] = '\0';
                        printf("%s",in_passw);
                    }else{ //account is blocked
                        puts(message);
                        break;
                    }
                }
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
