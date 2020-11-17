#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct 
{
    char username[50];
    char password[50];
    char status[10];
    int login_status;
    char homepage[20];
}ElementType;

struct LinkedList{
    ElementType eltype;
    struct LinkedList *next;
 };
 
typedef struct LinkedList *node; //Từ giờ dùng kiểu dữ liệu LinkedList có thể thay bằng node cho ngắn gọn
 
node CreateNode(ElementType eltype){
    node temp; // declare a node
    eltype.login_status = 0;
    temp = (node)malloc(sizeof(struct LinkedList)); // Cấp phát vùng nhớ dùng malloc()
    temp->next = NULL;// Cho next trỏ tới NULL
    temp->eltype = eltype; // Gán giá trị cho Node
    return temp;//Trả về node mới đã có giá trị
}
 
node AddTail(node head, ElementType eltype){
    node temp,p;// Khai báo 2 node tạm temp và p
    temp = CreateNode(eltype);//Gọi hàm createNode để khởi tạo node temp có next trỏ tới NULL và giá trị là value
    if(head == NULL){
        head = temp;     //Nếu linked list đang trống thì Node temp là head luôn
    }
    else{
        p  = head;// Khởi tạo p trỏ tới head
        while(p->next != NULL){
            p = p->next;//Duyệt danh sách liên kết đến cuối. Node cuối là node có next = NULL
        }
        p->next = temp;//Gán next của thằng cuối = temp. Khi đó temp sẽ là thằng cuối(temp->next = NULL mà)
    }
    return head;
}
 
 
node AddHead(node head, ElementType eltype){
    node temp = CreateNode(eltype); // Khởi tạo node temp với data = value
    if(head == NULL){
        head = temp; // //Nếu linked list đang trống thì Node temp là head luôn
    }else{
        temp->next = head; // Trỏ next của temp = head hiện tại
        head = temp; // Đổi head hiện tại = temp(Vì temp bây giờ là head mới mà)
    }
    return head;
}
 
node AddAt(node head, ElementType eltype, int position){
    if(position == 0 || head == NULL){
        head = AddHead(head, eltype); // Nếu vị trí chèn là 0, tức là thêm vào đầu
    }else{
        // Bắt đầu tìm vị trí cần chèn. Ta sẽ dùng k để đếm cho vị trí
        int k = 1;
        node p = head;
        while(p != NULL && k != position){
            p = p->next;
            ++k;
        }
 
        if(k != position){
            // Nếu duyệt hết danh sách lk rồi mà vẫn chưa đến vị trí cần chèn, ta sẽ mặc định chèn cuối
            // Nếu bạn không muốn chèn, hãy thông báo vị trí chèn không hợp lệ
            head = AddTail(head, eltype);
            // printf("Vi tri chen vuot qua vi tri cuoi cung!\n");
        }else{
            node temp = CreateNode(eltype);
            temp->next = p->next;
            p->next = temp;
        }
    }
    return head;
}
 
node DelHead(node head){
    if(head == NULL){
        printf("\nCha co gi de xoa het!");
    }else{
        head = head->next;
    }
    return head;
}
 
node DelTail(node head){
    if (head == NULL || head->next == NULL){
         return DelHead(head);
    }
    node p = head;
    while(p->next->next != NULL){
        p = p->next;
    }
    p->next = p->next->next; // Cho next bằng NULL
    // Hoặc viết p->next = NULL cũng được
    return head;
}
 
node DelAt(node head, int position){
    if(position == 0 || head == NULL || head->next == NULL){
        head = DelHead(head); // Nếu vị trí chèn là 0, tức là thêm vào đầu
    }else{
        // Bắt đầu tìm vị trí cần chèn. Ta sẽ dùng k để đếm cho vị trí
        int k = 1;
        node p = head;
        while(p->next->next != NULL && k != position){
            p = p->next;
            ++k;
        }
 
        if(k != position){
            // Nếu duyệt hết danh sách lk rồi mà vẫn chưa đến vị trí cần chèn, ta sẽ mặc định xóa cuối
            // Nếu bạn không muốn xóa, hãy thông báo vị trí xóa không hợp lệ
            head = DelTail(head);
            // printf("Vi tri xoa vuot qua vi tri cuoi cung!\n");
        }else{
            p->next = p->next->next;
        }
    }
    return head;
}
 
ElementType Get(node head, int index){
    int k = 0;
    node p = head;
    while(p != NULL && k != index){
        k++;
        p = p->next;
    }
    return p->eltype;
}

void printList(node head){
    node p = head;
    while (p != NULL)
    {
        printf("%s\t%s\t%s\t%s\n",p->eltype.username,p->eltype.password,p->eltype.status,p->eltype.homepage);
        p = p->next;
    }
    
}
 
int Search(node head, char *eltype){
    int position = 0;
    for(node p = head; p != NULL; p = p->next){
        if(strcmp(p->eltype.username,eltype)==0){
            return position;
        }
        ++position;
    }
    return -1;
}

void Update(node head, ElementType eltype, int index){
    int k = 0;
    node p = head;
    while(p != NULL && k != index){
        k++;
        p = p->next;
    }
    strcpy(p->eltype.username,eltype.username);
    strcpy(p->eltype.password,eltype.password);
    strcpy(p->eltype.status,eltype.status);
    p->eltype.login_status = eltype.login_status;

}
 
node DelByVal(node head, char *eltype){
    int position = Search(head, eltype);
    while(position != -1){
        DelAt(head, position);
        position = Search(head, eltype);
    }
    return head;
}
 
 
node InitHead(){
    node head;
    head = NULL;
    return head;
}
 
int Length(node head){
    int length = 0;
    for(node p = head; p != NULL; p = p->next){
        ++length;
    }
    return length;
}
 
node Input(){
    node head = InitHead();
    int n;
    ElementType eltype;
    do{
        printf("\nNhap so luong phan tu n = ");
        scanf("%d", &n);
    }while(n <= 0);
 
    for(int i = 0; i < n; ++i){
        printf("\nNhap gia tri can them: ");
        scanf("%s %s %s", eltype.username, eltype.password, eltype.status);
        head = AddTail(head, eltype);
    }
    return head;
}