#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "linkedList.h"

typedef enum {true, false} bool;
int checkNumber(char* str);
int checkString_Number(char *str);
int splitString(char *str,char *old_number, char *old_string);

int main(int argc, char **argv){

    int sockfd, port, recvBytes;// bindBytes;
    struct sockaddr_in servaddr, clieaddr;
    socklen_t len_serv, len_clie;
    char file_name[] = "nguoidung.txt";
    ElementType user, check_user;


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

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");//htonl(INADDR_ANY);

    len_serv = sizeof(servaddr);
    len_clie = sizeof(clieaddr);
    

    bind(sockfd, (struct sockaddr*)&servaddr,len_serv);

    while(1){
        char account_[50], password_[30], new_password_[30];
        bool check = false;
        
        //nhan account tu client
        recvBytes = recvfrom(sockfd, account_,sizeof(account_),0,(struct sockaddr*)&clieaddr, &len_clie);
        account_[recvBytes] = '\0';
      
        
        if(recvBytes == 0){
            continue;
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
                        sendto(sockfd, ok_account,strlen(ok_account),0,(struct sockaddr*)&clieaddr,len_clie);
                        // nhan passw tu client
                        recvBytes = recvfrom(sockfd, password_,sizeof(password_),0,(struct sockaddr*)&clieaddr, &len_clie);
                        password_[recvBytes] = '\0';
                        if(recvBytes == 0){
                          
                            break;
                        }
                        if(strcmp(user.password,password_) == 0){ //kiem tra passw - OK
                            char message[] = "OK";
                            user.login_status = 1;
                            Update(users_list,user,Search(users_list,user.username));
                            sendto(sockfd, message,strlen(message),0,(struct sockaddr*)&clieaddr,len_clie);
                            //xu ly new password
                            recvBytes = recvfrom(sockfd,new_password_,sizeof(new_password_),0,(struct sockaddr*)&clieaddr,&len_clie);
                            new_password_[recvBytes] = '\0';
                            if(recvBytes == 0){
                                
                                break;
                            }
                            {
                                char old_number[30], old_string[30];
                                if(checkString_Number(new_password_) == 1){ //ok : string number
                                    if(strcmp(new_password_,"bye") == 0){
                                        char message[] = "Goodbye ";
                                        strcat(message,user.username);
                                        user.login_status = 0;
                                        sendto(sockfd,message,strlen(message),0,(struct sockaddr*)&clieaddr,len_clie);
                                    }else{
                                        strcpy(user.password,new_password_);
                                        Update(users_list,user,Search(users_list,user.username));

                                        splitString(new_password_,old_number,old_string);
                                        sendto(sockfd,old_number,strlen(old_number),0,(struct sockaddr*)&clieaddr,len_clie);
                                        sendto(sockfd,old_string,strlen(old_string),0,(struct sockaddr*)&clieaddr,len_clie);
                                    }
                                }else{
                                    char message[] = "Error";
                                    sendto(sockfd,message,strlen(message),0,(struct sockaddr*)&clieaddr,len_clie);
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
                                sendto(sockfd, message,strlen(message),0,(struct sockaddr*)&clieaddr,len_clie);                           
                                break;
                            }else{
                                char message[] = "Not OK";
                                sendto(sockfd, message,strlen(message),0,(struct sockaddr*)&clieaddr,len_clie);
                            }
                        }                      
                    }
                }else{ //account dang dang nhap
                    char message[] = "Username \'";
                    char sign_out_bye[100];
                    strcat(message,user.username);
                    strcat(message,"\' already Login");
                    sendto(sockfd,message,strlen(message),0,(struct sockaddr*)&clieaddr,len_clie);
                    recvBytes = recvfrom(sockfd,sign_out_bye,sizeof(sign_out_bye),0,(struct sockaddr*)&clieaddr,&len_clie);
                    sign_out_bye[recvBytes] = '\0';
                    if(recvBytes > 0){
                        if(strcmp(sign_out_bye,"bye") == 0){
                            char message[] = "Goodbye ";
                            strcat(message,user.username);
                            user.login_status = 0;
                            sendto(sockfd, message,strlen(message),0,(struct sockaddr*)&clieaddr,len_clie); 
                        }
                    }
                }    
            }else{
                char message[] = "Account not ready";
                sendto(sockfd, message,strlen(message),0,(struct sockaddr*)&clieaddr,len_clie);               
            }
        }else{
            char message[] = "Wrong account";
            sendto(sockfd, message,strlen(message),0,(struct sockaddr*)&clieaddr,len_clie);
        }

        
        rewind(f);
        node p = users_list;

        while (p != NULL)
        {
            fprintf(f,"%s %s %s %s\n",p->eltype.username,p->eltype.password,p->eltype.status,p->eltype.homepage);
            p = p->next;
        }
        

        fclose(f);
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