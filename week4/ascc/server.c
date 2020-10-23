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

int main(int argc, char **argv){

    int sockfd, port, recvBytes;// bindBytes;
    struct sockaddr_in servaddr, clieaddr;
    socklen_t len_serv, len_clie;
    char file_name[] = "nguoidung.txt";
    ElementType user, check_user;


    if(argc != 2){
        printf("Invalid grammar\n");
        printf("ex: ./server 9500\n");
        return 0;
    }
    
    if(checkNumber(argv[1]) == 0){
        printf("Parameter1 must be number\n");
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
        char username[50], password[30], new_password[30];
        bool check = false;
        
        //nhan account tu client
        recvBytes = recvfrom(sockfd, username,sizeof(username),0,(struct sockaddr*)&clieaddr, &len_clie);
        username[recvBytes] = '\0';
      
        
        if(recvBytes == 0){
            continue;
        }
       if (Search(users_list,username)!=-1) {
            check = true;
            user = Get(users_list,Search(users_list,username));
       }

        if(check == true){ 
            if(strcmp(user.status, "active")==0){ 

                if(user.login_status != 1){ 
                    int count = 3;  
                    while(1){ 
                        char ok_account[] = "Insert password: ";
                        sendto(sockfd, ok_account,strlen(ok_account),0,(struct sockaddr*)&clieaddr,len_clie);
                        // nhan passw tu client
                        recvBytes = recvfrom(sockfd, password,sizeof(password),0,(struct sockaddr*)&clieaddr, &len_clie);
                        password[recvBytes] = '\0';
                        if(recvBytes == 0){
                          
                            break;
                        }
                        if(strcmp(user.password,password) == 0){ 
                            char message[] = "OK";
                            user.login_status = 1;
                            Update(users_list,user,Search(users_list,user.username));
                            sendto(sockfd, message,strlen(message),0,(struct sockaddr*)&clieaddr,len_clie);
                            
                            recvBytes = recvfrom(sockfd,new_password,sizeof(new_password),0,(struct sockaddr*)&clieaddr,&len_clie);
                            new_password[recvBytes] = '\0';
                            if(recvBytes == 0){
                                
                                break;
                            }
                            {
                                char old_number[30], old_string[30];
                                if(checkString_Number(new_password) == 1){ //ok : string number
                                    if(strcmp(new_password,"bye") == 0){
                                        char message[] = "Goodbye ";
                                        strcat(message,user.username);
                                        user.login_status = 0;
                                        sendto(sockfd,message,strlen(message),0,(struct sockaddr*)&clieaddr,len_clie);
                                    }else{
                                        strcpy(user.password,new_password);
                                        Update(users_list,user,Search(users_list,user.username));

                                        splitString(new_password,old_number,old_string);
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
