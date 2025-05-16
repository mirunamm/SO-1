#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // fork()
#include <signal.h>
#include <sys/wait.h> //waitpid()
#include <dirent.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include "treasure_manager.h"

pid_t monitorID = -1;
int monitorStatus=0;
int pfd[2];//pipe

void menu()
{
    printf("\nChoose one of the following commands:\n");
        printf("start_monitor\n");
        printf("list_hunts\n");
        printf("list_treasure\n");
        printf("view_treasure\n");
        printf("stop_monitor\n");
        printf("exit\n");
}

int numberOfHunts(char *hunt)
{
    char file[256];
    int k=0;
    snprintf(file, sizeof(file), "%s/%s",hunt,hunt);
    struct stat s;

    if(stat(file,&s) == -1)
    {
        printf("stat failed\n");
        return 0;
    }

    int fd= open(file,O_RDONLY);

    if(fd<0)
    {
        printf("cannot open treasure file");
        return 0;


    }
    treasure t;

    while(read(fd,&t,sizeof(treasure)))
    {
        k++;
    }
    close(fd);
    return k;
}

void list1()
{

    DIR* directory=opendir(".");
    struct dirent *di;

    if(directory == NULL)
    {
        printf("opendir failed\n");
        return;
    }
   
    char path[300];

    while((di=readdir(directory)))
    {
        

        if(strcmp(di->d_name,".")==0 || strcmp(di->d_name,"..")==0)
        continue;
        snprintf(path, sizeof(path), "%s/%s",".",di->d_name);

        struct stat verif;
        if(stat(path, &verif) == 0 && S_ISDIR(verif.st_mode)) 
        {

        if(strcmp(di->d_name,".git")==0)
        continue;
        printf("Hunt name: %s\n",di->d_name);
        printf("Number of treasure: %d\n",numberOfHunts(di->d_name));
        }
    }

    closedir(directory);
    
}

void handle_function(int signal)
{
    if(signal==SIGUSR1)
    {
        system("clear");
        kill(getppid(), SIGSTOP);
        list1();
        kill(getppid(), SIGCONT);
        menu();

        
    }
    else if(signal==SIGUSR2)
    {
        system("clear");
        kill(getppid(),SIGSTOP);
        char hunt[32];
        printf("Enter hunt name: \n");
        fgets(hunt,sizeof(hunt),stdin);
        char path[100];
        sprintf(path,"./test list %s",hunt);
        system(path);
        kill(getppid(),SIGCONT);
        menu();


    }
    else if(signal==SIGINT)
    {
        system("clear");
        kill(getppid(),SIGSTOP);
        char hunt[30],id[30];
        printf("Enter hunt name: \n");
        fgets(hunt,sizeof(hunt),stdin);
        hunt[strcspn(hunt,"\n")]='\0';
        printf("Enter treasure id: \n");
        fgets(id,sizeof(id),stdin);
        id[strcspn(id,"\n")]='\0';
        char path[100];
        sprintf(path,"./test view %s %s",hunt,id);
        system(path);
        kill(getppid(),SIGCONT);
        menu();

    }else if(signal==SIGTERM)
    {
        system("clear");
        _exit(0);
    }
}

int start_monitor()
{

    if(monitorID>0)
    {
        printf("Monitor is already running\n");
        return -1;
    }
    
    if(pipe(pfd) < 0)
    {
        printf("pipe error\n");
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
        struct sigaction sa = {0};
        sa.sa_handler=handle_function;

        if(sigaction(SIGUSR1,&sa,NULL)==-1)
        {
            printf("Sigaction error\n");
            return -1;
        }

        if(sigaction(SIGUSR2,&sa,NULL)==-1)
        {
            printf("Sigaction error\n");
            return -1;
        }


        if(sigaction(SIGINT,&sa,NULL)==-1)
        {
            printf("Sigaction error\n");
            return -1;
        }


        if(sigaction(SIGTERM,&sa,NULL)==-1)
        {
            printf("Sigaction error\n");
            return -1;
        }

        close(pfd[0]);

        while(1)
        {
            pause();//astept semnale
        }
        exit(0);

    } else {
       
        monitorStatus=1;
        close(pfd[1]);
        return 0;
        
    }

}

void list_hunts()
{
    if(monitorStatus == 0)
    {
        printf("Monitor is not running\n");
        return;
    }
    
    kill(monitorID,SIGUSR1);
}   


void list_treasure()
{
    if(monitorStatus == 0)
    {
        printf("Monitor is not running\n");
        return;
    }

    kill(monitorID,SIGUSR2);
    

}


void view_treasure()
{
    if(monitorStatus == 0)
    {
        printf("Monitor is not running\n");
        return;
    }
    kill(monitorID,SIGINT);

}

void stop_monitor()
{
    if(monitorStatus ==0 || monitorID <0)
    {
        printf("Monitor is not running\n");
        return;
    }

    kill(monitorID,SIGTERM);

    int status=0;
    waitpid(monitorID,&status,0);
    printf("Monitor %d stopped with status %d\n",monitorID,status);
    monitorID=-1;
    monitorStatus=0;
}


int main()
{
    char comanda[64];

    while(1)
    {
        menu();
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
            if (monitorStatus==1) {
                printf("Monitor still running. Use stop_monitor first.\n");
            } else {
                break;
            }
        }else{
            printf("Unknown command. \n");
        }
    }
}