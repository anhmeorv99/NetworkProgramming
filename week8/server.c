#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "linkedList.h"

typedef enum {true, false} bool;
int checkNumber(char* str);
int checkString_Number(char *str);
int splitString(char *str,char *old_number, char *old_string);
void sig_chld(int signo);

int main(int argc, char **argv){

    pid_t pid;
    int sockfd, port, recvBytes,new_socket; // bindBytes;
    struct sockaddr_in servaddr, clieaddr;
    socklen_t len_serv, len_clie;
    char file_name[] = "nguoidung.txt";
    ElementType user, check_user;
    int addrlen = sizeof(clieaddr);


    if(argc != 2){
        printf("Error: sai cu phap\n");
        printf("Cu phap: ./server <port-number>\n");
        return 0;
    }
    
    if(checkNumber(argv[1]) == 0){
        printf("Error: parameter1 (port) phai la so\n");
        return 0;
    }
    
    FILE *f;  
   node users_list = NULL;
    if((f = fopen(file_name,"r+")) == NULL) {
            printf("No such file or directory !!!");
            return 0;
        }

   
        while ( fscanf(f,"%s %s %s %s",user.username,user.password,user.status,user.homepage)!=EOF)
        {
                            users_list = AddTail(users_list,user);

        }
    //printList(root);
    port = atoi(argv[1]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");//htonl(INADDR_ANY);

    len_serv = sizeof(servaddr);
   

    bind(sockfd, (struct sockaddr*)&servaddr,len_serv);

    if (listen(sockfd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	signal(SIGCHLD, sig_chld);
    while (1)
    {
       
        if ((new_socket = accept(sockfd, (struct sockaddr *)&clieaddr, (socklen_t*)&addrlen))<0) 
	    { 
    		perror("accept"); 
	    	exit(EXIT_FAILURE); 
	    } 

        if( (pid = fork()) == 0) {// process in child
                close(sockfd); // child closes listening socket
                // doit(new_socket);// process the request
                
        
    

        while(1){
        
    

        char account_[50], password_[30], new_password_[30];
        bool check = false;
        
        //nhan account tu client
        recvBytes = recv(new_socket, account_,sizeof(account_),0);
        account_[recvBytes] = '\0';
      
        
        if(recvBytes == 0){
            
            close(sockfd);
            //continue;
        }
       if (Search(users_list,account_)!=-1) {
            check = true;
            user = Get(users_list,Search(users_list,account_));
       }

        if(check == true){ //ton tai account
            if(strcmp(user.status, "active")==0){ // account dang hoat dong

                if(user.login_status != 1){ //account chua dang nhap
                    int count = 3;  //so lan co the nhap passw sai
                    while(1){ 
                        char ok_account[] = "Insert password: ";
                        send(new_socket, ok_account,strlen(ok_account),0);
                        // nhan passw tu client
                        recvBytes = recv(new_socket, password_,sizeof(password_),0);
                        password_[recvBytes] = '\0';
                        if(recvBytes == 0){
                            close(sockfd);
                            break;
                        }
                        if(strcmp(user.password,password_) == 0){ //kiem tra passw - OK
                            char message[] = "OK";
                            user.login_status = 1;
                            Update(users_list,user,Search(users_list,user.username));
                            send(new_socket, message,strlen(message),0);
                            //xu ly new password
                            recvBytes = recv(new_socket,new_password_,sizeof(new_password_),0);
                            new_password_[recvBytes] = '\0';
                            if(recvBytes == 0){
                                close(sockfd);
                                break;
                            }
                            {
                                char old_number[30], old_string[30];
                                if(checkString_Number(new_password_) == 1){ //ok : string number
                                    if(strcmp(new_password_,"bye") == 0){
                                        char message[] = "Goodbye ";
                                        strcat(message,user.username);
                                        user.login_status = 0;
                                        send(new_socket,message,strlen(message),0);
                                    }else{
                                        strcpy(user.password,new_password_);
                                        Update(users_list,user,Search(users_list,user.username));

                                        rewind(f);
                                        node p = users_list;

                                        while (p != NULL)
                                            {
                                                fprintf(f,"%s %s %s %s\n",p->eltype.username,p->eltype.password,p->eltype.status,p->eltype.homepage);
                                                p = p->next;
                                            }
        

                                        fclose(f);
                                        
                                        splitString(new_password_,old_number,old_string);
                                        send(new_socket,old_number,strlen(old_number),0);
                                        send(new_socket,old_string,strlen(old_string),0);
                                        
                                    }
                                }else{
                                    char message[] = "Error";
                                    send(new_socket,message,strlen(message),0);
                                }
                            }
                            //------
                            
                            break;
                        }else{
                            count--;
                            if(count == 0){
                                char message[] = "Account is blocked";
                                strcpy(user.status,"blocked");  
                                Update(users_list,user,Search(users_list,user.username));                         
                                send(new_socket, message,strlen(message),0);                           
                                break;
                            }else{
                                char message[] = "Not OK";
                                send(new_socket, message,strlen(message),0);
                            }
                        }                      
                    }
                }else{ //account dang dang nhap
                    char message[] = "Username \'";
                    char sign_out_bye[100];
                    strcat(message,user.username);
                    strcat(message,"\' already Login");
                    send(new_socket,message,strlen(message),0);
                    recvBytes = recv(new_socket,sign_out_bye,sizeof(sign_out_bye),0);
                    if (recvBytes == 0){
                        close(sockfd);
                        break;
                    }
                    sign_out_bye[recvBytes] = '\0';
                    if(recvBytes > 0){
                        if(strcmp(sign_out_bye,"bye") == 0){
                            char message[] = "Goodbye ";
                            strcat(message,user.username);
                            user.login_status = 0;
                            send(new_socket, message,strlen(message),0); 
                        }
                    }
                }    
            }else{
                char message[] = "Account not ready";
                send(new_socket, message,strlen(message),0);               
            }
        }else{
            char message[] = "Wrong account";
            send(new_socket, message,strlen(message),0);
        }

    }

        close(new_socket);// done with this client
        exit(0);// child terminates

        }
       
    }

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

int checkString_Number(char *str){
    int i;
    if(strlen(str) == 0) return 0;
    for(i =0; i< strlen(str); i++){
        if(!((str[i] >= '0' && str[i] <= '9') ||
            (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')))
            return 0;
    }
    return 1;
}

int splitString(char *str, char *old_number, char *old_string){
    int i, j = 0, k = 0;
    if(strlen(str) == 0){
        old_number = "";
        old_string = "";
        return 0;
    }

    if(checkString_Number(str) == 0) return 0;

    for(i = 0; i < strlen(str); i++){
        if(str[i] >= '0' && str[i] <= '9'){
            old_number[j] = str[i];
            j++;
        }else{
            old_string[k] = str[i];
            k++;
        }
    }
    old_string[k] = '\0';
    old_number[j] = '\0';
    return 1;
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    pid = waitpid(-1, &stat, WNOHANG );
    printf("child %d terminated\n", pid);
}