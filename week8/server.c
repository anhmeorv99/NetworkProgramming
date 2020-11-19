//Các file header
//For socket()
#include <sys/types.h>
#include <sys/socket.h>
//For inet_addr()
#include <netinet/in.h>
#include <arpa/inet.h>
// For ..
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#include "linkedList.h"


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


int main(int argc, char* argv[])
{

    char file_name[] = "nguoidung.txt";
    ElementType user, check_user;
    int recvBytes;



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
    int port = atoi(argv[1]);

    // Tao server socket
    int serverSocket =  socket(PF_INET,SOCK_STREAM,0);
    if (serverSocket == -1)
    {
        perror("CREATE SOCKET");
        exit(0);
    }
    // Set reuse option
    int i = 1;
    int check_bind = setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(int));
    if (check_bind == -1)
    {
	perror("Set reuse");
    }
    // Binding to a address
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
        // Neu chay trong mang thi phai dung :
        // serverAddress.sin_addr.s_addr = inet_addr(address);
	socklen_t len = sizeof(struct sockaddr_in);
	check_bind = bind(serverSocket,(struct sockaddr*)&serverAddress,len);
    printf("%d\n",serverSocket);
	if (check_bind == -1)
        {
            perror("BIND");
            exit(0);
        }
        //Gio ta cho server lang nghe ket noi
        check_bind = listen(serverSocket,3);
	if (check_bind == -1)
	{
		perror("Listen");
		exit(1);
	}
        struct sockaddr_in clientAddress;
        struct timeval timeout;
        len = sizeof(struct sockaddr_in);
        fd_set readfds;
        fd_set masterfds; // tập readfds để check các socket, 1 tập để lưu lại nhưng thay đổi của tập readfds.
        FD_ZERO(&masterfds);
        FD_ZERO(&readfds);
        FD_SET(serverSocket,&masterfds); // add serverSock vào tập masterfds.
        int max_fd = serverSocket;
        
        timeout.tv_sec = 90;
        timeout.tv_usec = 0;
        int n_select;
        do
    {
        memcpy(&readfds,&masterfds,sizeof(masterfds));
        n_select = select(max_fd + 1, &readfds,NULL,NULL,NULL);
        if (n_select < 0)
        {
                perror("SELECT");
                exit(0);
        }
        else
        if (n_select == 0)
        {
            printf("Time out\n");
            sleep(3);
        }
        else
        {
            
              for (i = 0; i <= max_fd;i++)
                {   
                    
                    if (FD_ISSET(i,&readfds))
                    {
                        printf("i-isset = %d\n",i);
                    	int close_fd = 0;
                        if (i == serverSocket)
                        {                          
                          int newCon = accept(serverSocket,(struct sockaddr*)&clientAddress,&len);
                          printf("New connection - newcon = %d\n",newCon);

                          FD_SET(newCon,&masterfds);
                          if (newCon > max_fd ) max_fd = newCon;
                        }
                        else
                        {
        // while(1){
        
    

        char account_[50], password_[30], new_password_[30];
        int check = 0;
        
        //nhan account tu client
        recvBytes = recv(i, account_,sizeof(account_),0);
        account_[recvBytes] = '\0';
        if(recvBytes == -1 || recvBytes == 0){
             printf("In socket %d\n",i);
             perror("RECEIVE");
            close_fd = 1;
            // break;
            continue;
        }
       if (Search(users_list,account_)!=-1) {
            check = 1;
            user = Get(users_list,Search(users_list,account_));
       }

        if(check == 1){ //ton tai account
            if(strcmp(user.status, "active")==0){ // account dang hoat dong

                if(user.login_status != 1){ //account chua dang nhap
                    int count = 3;  //so lan co the nhap passw sai
                    while(1){ 
                        char ok_account[] = "Insert password: ";
                        send(i, ok_account,strlen(ok_account),0);
                        // nhan passw tu client
                        recvBytes = recv(i, password_,sizeof(password_),0);
                        password_[recvBytes] = '\0';
                        if(recvBytes == -1 || recvBytes == 0){
                            printf("In socket %d\n",i);
                            perror("RECEIVE");
                            close_fd = 1;
                            break;
                        }
                        if(strcmp(user.password,password_) == 0){ //kiem tra passw - OK
                            char message[] = "OK";
                            user.login_status = 1;
                            Update(users_list,user,Search(users_list,user.username));
                            send(i, message,strlen(message),0);
                            //xu ly new password
                            recvBytes = recv(i,new_password_,sizeof(new_password_),0);
                            new_password_[recvBytes] = '\0';
                            if(recvBytes == -1 || recvBytes == 0){
                                printf("In socket %d\n",i);
                                perror("RECEIVE");
                                close_fd = 1;
                                break;
                            }
                            {
                                char old_number[30], old_string[30];
                                if(checkString_Number(new_password_) == 1){ //ok : string number
                                    if(strcmp(new_password_,"bye") == 0){
                                        char message[] = "Goodbye ";
                                        strcat(message,user.username);
                                        user.login_status = 0;
                                        send(i,message,strlen(message),0);
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
                                        send(i,old_number,strlen(old_number),0);
                                        send(i,old_string,strlen(old_string),0);
                                        
                                    }
                                }else{
                                    char message[] = "Error";
                                    send(i,message,strlen(message),0);
                                }
                            }
                            //------
                            close_fd = 1;

                            break;
                        }else{
                            count--;
                            if(count == 0){
                                char message[] = "Account is blocked";
                                strcpy(user.status,"blocked");  
                                Update(users_list,user,Search(users_list,user.username));                         
                                send(i, message,strlen(message),0); 
                                close_fd = 1;
                          
                                break;
                            }else{
                                char message[] = "Not OK";
                                send(i, message,strlen(message),0);
                            }
                        }                      
                    }
                }else{ //account dang dang nhap
                    char message[] = "Username \'";
                    char sign_out_bye[100];
                    strcat(message,user.username);
                    strcat(message,"\' already Login");
                    send(i,message,strlen(message),0);
                    recvBytes = recv(i,sign_out_bye,sizeof(sign_out_bye),0);
                    if (recvBytes == 0){
                        close_fd = 1;

                        // break;
                    }
                    sign_out_bye[recvBytes] = '\0';
                    if(recvBytes > 0){
                        if(strcmp(sign_out_bye,"bye") == 0){
                            char message[] = "Goodbye ";
                            strcat(message,user.username);
                            user.login_status = 0;
                            send(i, message,strlen(message),0); 
                        }
                    }
                }    
            }else{
                char message[] = "Account not ready";
                send(i, message,strlen(message),0);               
            }
        }else{
            char message[] = "Wrong account";
            send(i, message,strlen(message),0);
        }

        if (close_fd == 1) 
                        {
                        	FD_CLR(i,&masterfds);
                        	close(i);
                        }

    }
                        }
                        
                    }
                }
    // }
        
    }
    while(1);
    close(serverSocket);
    return 0;
}

