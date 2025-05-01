#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // fork()
#include <signal.h>
#include <sys/wait.h> //waitpid()

pid_t monitorID = -1;
int monitorStatus=0;


typedef struct treasure
{
    char id[32];
    char username[64];
    double latitudine,longitudine;
    char clue[128];
    int val;
}treasure;


void handle_function(int signal)
{
    printf("Monitor %d received signal",getpid());
}

int start_monitor()
{

    if(monitorID>0)
    {
        printf("Monitor is already running\n");
        return -1;
    }
    
    monitorID = fork();
    
    if(monitorID<0)
    {
        perror("fork failed");
        return -1;
    }

    if(monitorID==0)
    {
        struct sigaction sa;
        sa.sa_handler=handle_function;
        if(sigaction(SIGUSR1,&sa,NULL)==-1)
        {
            printf("Sigaction error\n");
            return -1;
        }

        while(1)
        {
            pause();//astept semnale
        }
        exit(0);

    } else {
       
        monitorStatus=1;
        
    }

    return 0;
}

void list_hunts()
{
    if(monitorStatus == 0)
    {
        printf("Monitor is not running\n");
        return;
    }
}



void list_treasure()
{
    if(monitorStatus == 0)
    {
        printf("Monitor is not running\n");
        return;
    }

}


void view_treasure()
{
    if(monitorStatus == 0)
    {
        printf("Monitor is not running\n");
        return;
    }

}

void stop_monitor()
{
    if(monitorStatus ==0 || monitorID <0)
    {
        printf("Monitor is not running\n");
        return;
    }

    kill(monitorID,SIGUSR1);

    waitpid(monitorID,NULL,0);
    printf("Monitor %d stopped\n",monitorID);
    monitorID=-1;
    monitorStatus=0;
}


int main()
{
    char comanda[64];

    struct sigaction sa;


    while(1)
    {
        printf("\nChoose one of the following commands:\n");
        printf("start_monitor\n");
        printf("list_hunts\n");
        printf("list_treasure\n");
        printf("view_treasure\n");
        printf("stop_monitor\n");
        printf("exit\n");
        if(!fgets(comanda,sizeof(comanda),stdin)) break;

        if(strcmp(comanda,"start_monitor\n")==0)
        {
            start_monitor();
        }else if(strcmp(comanda,"list_hunts\n")==0)
        {
            list_hunts();
        }else if(strcmp(comanda,"list_treasure\n")==0)
        {
            list_treasure();
        }else if(strcmp(comanda,"view_treasure\n")==0)
        {
            view_treasure();
        }else if(strcmp(comanda,"stop_monitor\n")==0)
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