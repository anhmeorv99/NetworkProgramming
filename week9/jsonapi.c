#include<stdio.h>
#include<json-c/json.h>
#include <string.h>
#include <stdlib.h>


typedef struct user {
    struct json_object *id;
    struct json_object *name;
    struct json_object *username;
    struct json_object *password;
    struct json_object *status;
    struct json_object *created_at;
    struct json_object *is_admin;
} User;

typedef struct friends {
    struct json_object *id;
    struct json_object *friend;
    struct json_object *confirm;
    struct json_object *user;
} Friend;

typedef struct room {
	struct json_object *id;
	struct json_object *room_status;
 	struct json_object *name;
  	struct json_object *admin_room; 
  	struct json_object *member;
} Room;

typedef struct message {
	struct json_object *id;
	struct json_object *from_user;
 	struct json_object *message;
  	struct json_object *created_at; 
  	struct json_object *room;
	struct json_object *member;
} Message;



typedef enum {
	false, true
} bool;

typedef enum{
	DB_NONE,
	DB_LIST_FRIEND,
	DB_CHAT_PRIVATE,
	DB_CHAT_PUBLIC
}Signal_db;


void convert_object_to_struct_user(){
	User item;
	FILE *fp;
	char buffer[1024];
	size_t length_eltype;
	size_t i;	
	struct json_object *parsed_json;
	struct json_object *elementType;
	fp = fopen("data.json","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	length_eltype = json_object_array_length(parsed_json);
	printf("Found %lu item\n",length_eltype);

	for(i=0;i<length_eltype;i++) {
		elementType = json_object_array_get_idx(parsed_json, i);
		json_object_object_get_ex(elementType, "id", &item.id);
		json_object_object_get_ex(elementType, "name", &item.name);
		json_object_object_get_ex(elementType, "username", &item.username);
		json_object_object_get_ex(elementType, "password", &item.password);
		json_object_object_get_ex(elementType, "status", &item.status);
		json_object_object_get_ex(elementType, "created_at", &item.created_at);
		json_object_object_get_ex(elementType, "is_admin", &item.is_admin);

        printf("id = %d, name = %s, username = %s, password = %s, status = %d, created_at = %s, is_admin = %d \n",
			json_object_get_int(item.id),
			json_object_get_string(item.name),
			json_object_get_string(item.username),
			json_object_get_string(item.password),
			json_object_get_boolean(item.status),
			json_object_get_string(item.created_at),
			json_object_get_boolean(item.is_admin));
	}	
	

}

void convert_object_to_struct_friend(){
	Friend item;
	FILE *fp;
	char buffer[1024];
	size_t length_eltype;
	size_t i;	
	struct json_object *parsed_json;
	struct json_object *elementType;
	fp = fopen("data.json","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	length_eltype = json_object_array_length(parsed_json);
	printf("Found %lu item\n",length_eltype);

	for(i=0;i<length_eltype;i++) {
		elementType = json_object_array_get_idx(parsed_json, i);
		json_object_object_get_ex(elementType, "id", &item.id);
		json_object_object_get_ex(elementType, "friend", &item.friend);
		json_object_object_get_ex(elementType, "confirm", &item.confirm);
		json_object_object_get_ex(elementType, "user", &item.user);

        printf("id = %d, friend = %d, confirm = %d, user = %d \n",
			json_object_get_int(item.id),
			json_object_get_int(item.friend),
			json_object_get_boolean(item.confirm),
			json_object_get_int(item.user));
	}	
	

}

void convert_object_to_struct_room(){
	Room item;
	FILE *fp;
	char buffer[1024];
	size_t length_eltype, length_member;
	size_t i,j;	
	struct json_object *parsed_json;
	struct json_object *elementType;
	struct json_object *id_member;
	fp = fopen("data.json","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	length_eltype = json_object_array_length(parsed_json);
	printf("Found %lu item\n",length_eltype);

	for(i=0;i<length_eltype;i++) {
		elementType = json_object_array_get_idx(parsed_json, i);
		json_object_object_get_ex(elementType, "id", &item.id);
		json_object_object_get_ex(elementType, "room_status", &item.room_status);
		json_object_object_get_ex(elementType, "name", &item.name);
		json_object_object_get_ex(elementType, "admin_room", &item.admin_room);
		json_object_object_get_ex(elementType, "member", &item.member);
        printf("id = %d, room_status = %s, name = %s, admin_room = %d, ",
			json_object_get_int(item.id),
			json_object_get_string(item.room_status),
			json_object_get_string(item.name),
			json_object_get_int(item.admin_room));
		
		printf("member = ");
		length_member = json_object_array_length(item.member);
		for(j=0;j<length_member;j++) {
			id_member = json_object_array_get_idx(item.member, j);
			printf("%d, ",json_object_get_int(id_member));
	}	
		printf("\n");

		
		
	}	
}

void convert_object_to_struct_message(){
	Message item;
	FILE *fp;
	char buffer[1024];
	size_t length_eltype, length_member;
	size_t i,j;	
	struct json_object *parsed_json;
	struct json_object *elementType;
	struct json_object *id_member;
	fp = fopen("data.json","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	length_eltype = json_object_array_length(parsed_json);
	printf("Found %lu item\n",length_eltype);

	for(i=0;i<length_eltype;i++) {
		elementType = json_object_array_get_idx(parsed_json, i);
		json_object_object_get_ex(elementType, "id", &item.id);
		json_object_object_get_ex(elementType, "from_user", &item.from_user);
		json_object_object_get_ex(elementType, "message", &item.message);
		json_object_object_get_ex(elementType, "created_at", &item.created_at);
		json_object_object_get_ex(elementType, "room", &item.room);
		json_object_object_get_ex(elementType, "member", &item.member);
        printf("id = %d, from_user = %d, message = %s, created_at = %s, room = %d, ",
			json_object_get_int(item.id),
			json_object_get_int(item.from_user),
			json_object_get_string(item.message),
			json_object_get_string(item.created_at),
			json_object_get_int(item.room));
		
		printf("member = ");
		length_member = json_object_array_length(item.member);
		for(j=0;j<length_member;j++) {
			id_member = json_object_array_get_idx(item.member, j);
			printf("%d, ",json_object_get_int(id_member));
	}	
		printf("\n");

		
		
	}	
}


const char *convert_object_to_json_user(User elememt){
	   struct json_object *Eltype = json_object_new_object();
	    json_object_object_add(Eltype,"id",elememt.id);
		json_object_object_add(Eltype,"name",elememt.name);
	    json_object_object_add(Eltype,"username",elememt.username);
	    json_object_object_add(Eltype,"password",elememt.password);
	    json_object_object_add(Eltype,"status",elememt.status);
	    json_object_object_add(Eltype,"created_at",elememt.created_at);
	    json_object_object_add(Eltype,"is_admin",elememt.is_admin);
	return json_object_to_json_string(Eltype);
}

const char *convert_object_to_json_friend(Friend elememt){
	   struct json_object *Eltype = json_object_new_object();
	    json_object_object_add(Eltype,"id",elememt.id);
		json_object_object_add(Eltype,"friend",elememt.friend);
	    json_object_object_add(Eltype,"confirm",elememt.confirm);
	    json_object_object_add(Eltype,"user",elememt.user);
	return json_object_to_json_string(Eltype);
}

const char *convert_object_to_json_room(Room elememt){
	   struct json_object *Eltype = json_object_new_object();
	    json_object_object_add(Eltype,"id",elememt.id);
		json_object_object_add(Eltype,"room_status",elememt.room_status);
	    json_object_object_add(Eltype,"name",elememt.name);
	    json_object_object_add(Eltype,"admin_room",elememt.admin_room);
		json_object_object_add(Eltype,"member",elememt.member);
	return json_object_to_json_string(Eltype);
}

const char *convert_object_to_json_message(Message elememt){
	   struct json_object *Eltype = json_object_new_object();
	    json_object_object_add(Eltype,"id",elememt.id);
		json_object_object_add(Eltype,"from_user",elememt.from_user);
	    json_object_object_add(Eltype,"message",elememt.message);
	    json_object_object_add(Eltype,"created_at",elememt.created_at);
		json_object_object_add(Eltype,"room",elememt.room);
		json_object_object_add(Eltype,"member",elememt.member);
	return json_object_to_json_string(Eltype);
}


typedef struct {
	int ID; //khoa chinh
	bool confirm;
	char username[30]; //username cua friend
	char name[30];
}friend_db;

friend_db getFriend(Friend friend, User profile){
	friend_db Eltype;

	Eltype.ID = json_object_get_int(friend.user);
	strcpy(Eltype.name,json_object_get_string(profile.name));
	Eltype.confirm = json_object_get_boolean(friend.confirm);
	strcpy(Eltype.username, json_object_get_string(profile.username));
	return Eltype;
}

typedef struct {
	int ID; 
	char username[30];
	char name[30];
}member_db;

member_db getMember(User profile){
	member_db Eltype;
	Eltype.ID = json_object_get_int(profile.id);
	strcpy(Eltype.name,json_object_get_string(profile.name));
	strcpy(Eltype.username, json_object_get_string(profile.username));
	return Eltype;
}

typedef struct {
	int ID;
	int id_username;
	char from_username[30];
	char from_name[30];
	char message[500];
	char create_at[50];
}message_db;

message_db getMessage(Message message, User profile){
	message_db Eltype;
	Eltype.ID = json_object_get_int(message.id);
	strcpy(Eltype.from_username, json_object_get_string(profile.username));
	strcpy(Eltype.from_name, json_object_get_string(profile.name));
	strcpy(Eltype.message, json_object_get_string(message.message));
	strcpy(Eltype.create_at, json_object_get_string(message.created_at));
}

typedef struct {
	int ID;
	char to_username[30];
	char to_name[30];
	message_db msg_private[250];
	int length_message;
}Chat_Private_;

Chat_Private_ getChatPrivate(message_db message, User profile, int index){
	Chat_Private_ Eltype;
	Eltype.ID = message.ID;
	strcpy(Eltype.to_username, json_object_get_string(profile.username));
	strcpy(Eltype.to_name, json_object_get_string(profile.name));
	
	Eltype.length_message = index;
	Eltype.msg_private[index] = message;

	return Eltype;
}

typedef struct {
	int ID_group;
	char name[30];
	int id_admin;
	member_db members[12];
	int length_member;
	message_db msg_public[500];
	int length_msg_public;
}group_db;

group_db getGroupDB(message_db message, member_db member, Room room, int index_member, int index_message){
	group_db Eltype;
	Eltype.ID_group = json_object_get_int(room.id);
	strcpy(Eltype.name,json_object_get_string(room.name));
	Eltype.id_admin = json_object_get_int(room.admin_room);
	Eltype.members[index_member] = member;
	Eltype.length_member = index_member;
	Eltype.msg_public[index_message] = message;
	Eltype.length_msg_public = index_message;

	return Eltype;
}

typedef struct {
	int ID_user;
	char name[30];
	char username[30];
	char password[30];
	bool is_admin;
	bool login_status;
}user_db;

user_db getUserDB(User user){
	user_db Eltype;
	Eltype.ID_user = json_object_get_int(user.id);
	strcpy(Eltype.name, json_object_get_string(user.name));
	strcpy(Eltype.username, json_object_get_string(user.username));
	strcpy(Eltype.password, json_object_get_string(user.password));
	Eltype.is_admin = json_object_get_boolean(user.is_admin);
	Eltype.login_status = json_object_get_boolean(user.status);

	return Eltype;

}

typedef struct {
	Signal_db signal;
	user_db user;
	friend_db list_friend[50];
	int length_list_friend;
	Chat_Private_ chat_private[50];
	int length_chat_private;
	group_db group[20];
	int length_group;
}Data_base;

Data_base getDatabase(user_db user, friend_db *friend, int len_friend, Chat_Private_ *chat_private, 
						int len_chat_private, group_db *group, int len_group){
		
		Data_base Eltype;
		int i;
		Eltype.signal = DB_NONE;
		Eltype.user = user;
		for (i=0;i < len_friend;i++){
			Eltype.list_friend[i] = friend[i];
		}
		for (i=0;i < len_chat_private;i++){
			Eltype.chat_private[i] = chat_private[i];
		}
		for (i=0;i < len_group;i++){
			Eltype.group[i] = group[i];
		}

		Eltype.length_list_friend = len_friend;
		Eltype.length_chat_private = len_chat_private;
		Eltype.length_group = len_group;

		return Eltype;
}


int main(int argc, char **argv) {
	
	// convert_object_to_struct_user();
	// convert_object_to_struct_room();
	// convert_object_to_struct_message();
	// convert_object_to_struct_friend();
	User user;
	user.id = json_object_new_int(1);
	user.is_admin = json_object_new_boolean(FALSE);
	user.status = json_object_new_boolean(FALSE);
	user.created_at = json_object_new_string("12121212121");
	user.name = json_object_new_string("Tuan Anh");
	user.username = json_object_new_string("anh.nt");
	user.password = json_object_new_string("1234455");

	const char *a = convert_object_to_json_user(user);
	printf("%s", a);
	// user_db test = getUserDB(user);
	// printf("%s", test.username);


	// convert_object_to_struct_friend();
	return 1;
}