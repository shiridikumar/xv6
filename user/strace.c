#include "user.h"
char *combine(char *a, char *b, char *c)
{
    int i;
    for (i = 0; i < strlen(a); i++)
    {
        c[i] = a[i];
    }
    int j;
    for (j = 0; j < strlen(b); j++)
    {
        c[j + i] = b[j];
    }
    c[j+i]='\0';
    return c;
}
void main(int argc ,char *args[]){
    /*char arr[500];
    strcpy(arr,"");
    for(int i=1;i<argc;i++){
        combine(arr,args[i],arr);
        arr[strlen(arr)]=';';
        arr[strlen(arr)]='\0';
    }*/
    char **arr=(char **)malloc((argc)*sizeof(char *));
    for(int i=0;i<argc-1;i++){
        arr[i]=(char *)malloc(100*sizeof(char));
        arr[i]=args[i+1];
    }
    arr[argc-1]='\0';
    for(int i=0;i<argc;i++){
        printf("%s\n",arr[i]);
    }
    trace(5,argc,args+1);
    exec(arr[0],arr);
    //printf("",143);
    exit(0);
}