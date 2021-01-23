#include <stdlib.h>

typedef enum {
	true,false
} bool;

typedef enum{
	DB_NONE,
	DB_LIST_FRIEND,
	DB_CHAT_PRIVATE,
	DB_CHAT_PUBLIC
}Signal_db;

typedef struct {
	int ID;
	bool status;
	char friends[30];
}List_Friend;

typedef struct {
	int username_ID;
	char username[30];
	char name[30];
}Member;

typedef struct {
	int ID;
	char from_username[30];
	char message[200];
	char create_at[50];
}Message;

typedef struct {
	char to_username[30];
	Message msg_private[250];
	int length_message;
}Chat_Private_;

typedef struct {
	Message msg_public;
	Member to_members[10];
	int length_to_member;
}Chat_Public;

typedef struct {
	int ID_group;
	char name[30];
	char admin[30];
	Member members[12];
	int length_member;
	Chat_Public chat_public[500];
	int length_chat_public;
}Group;

typedef struct {
	int ID_user;
	char name[30];
	char username[30];
	char password[30];
	bool is_admin;
	bool login_status;
}User_DB;

typedef struct {
	//Signal_db signal;
	User_DB user;
	List_Friend list_friend[50];
	int length_list_friend;
	Chat_Private_ chat_private[50];
	int length_chat_private;
	Group group[20];
	int length_group;
}Data_base;

typedef struct List_DB_{
	Data_base *DB;
	struct List_DB_ *next;
}List_DB;

List_DB *new_list_db(Data_base *db){
	List_DB *new_db = (List_DB*)malloc(sizeof(List_DB));
	new_db->DB = db;
	new_db->next = NULL;
	return new_db;
}

List_DB *insert_at_end_db(List_DB *root_db, Data_base *db){
	List_DB *new_db = new_list_db(db);
	if(root_db == NULL){
		return new_db;
	}
	List_DB *cur_db = root_db;
	while(cur_db->next != NULL){
		cur_db = cur_db->next;
	} 
	cur_db->next = new_db;
	return root_db;
}

/*
void free_list_db(List_DB *root_db){
	if(root_db == NULL)
		return;
	else{
		List_DB *del = root_db;
		//free(del->DB);
		free(del);
		free_list_db(root_db->next);
	}
}*/