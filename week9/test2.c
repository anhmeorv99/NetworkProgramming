#include<stdio.h>
#include<json-c/json.h>
#include <string.h>
typedef struct  
{
    char a[30];
}he;
typedef struct  
{
    int b;
}he1;
typedef struct  {
    he h;
    he1 g;
}hello;



void main(){
  hello hee;
  char c[] = "asdasd";
  strcpy(hee.h.a,c);
  hee.g.b = 1;
  printf("%s %d", hee.h.a, hee.g.b);
}
