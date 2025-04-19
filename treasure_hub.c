#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // fork()

typedef struct treasure
{
    char id[32];
    char username[64];
    double latitudine,longitudine;
    char clue[128];
    int val;
}treasure;

void start_monitor()
{
    pid_t pid = fork();
    
    if(pid<0)
    {
        perror("fork failed");
        exit(-1);
    }

    if(pid==0)
    {

    }
}

void list_hunts()
{

}



void list_treasure()
{

}


void view_treasure()
{

}

void stop_monitor()
{

}


int main()
{
    char comanda[64];

    while(1)
    {
        if(!fgets(comanda,sizeof(comanda),stdin)) break;

        if(strcmp(comanda,"start_monitor")==0)
        {
            start_monitor();
        }else if(strcmp(comanda,"list_hunts")==0)
        {
            list_hunts();
        }else if(strcmp(comanda,"list_treasure")==0)
        {
            list_treasure();
        }else if(strcmp(comanda,"view_treasure")==0)
        {
            view_treasure();
        }else if(strcmp(comanda,"stop_monitor")==0)
        {
            stop_monitor();
        }else if(strcmp(comanda,"exit\n")==0)
        {
            break;
        }else{
            printf("Unknown command. \n");
        }
    }
}