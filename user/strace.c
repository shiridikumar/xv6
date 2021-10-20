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

    char **arr=(char **)malloc((argc-1)*sizeof(char *));
    for(int i=0;i<argc-1;i++){
        arr[i]=(char *)malloc(100*sizeof(char));
        arr[i]=args[i+2];
    }


    arr[argc-2]='\0';
    trace(atoi(args[1]),argc-1,args+1);
    exec(arr[0],arr);
    
    exit(0);
}