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

       