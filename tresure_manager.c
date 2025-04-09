#include "tresure_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
 
    if(argc <3 )
    {
        printf("Not enough arguments\n");
        exit(-1);
    }

    char *action = argv[1];
    char *hunt_id = argv[2];
    char *tresure_id;

    if(argc > 3)
    {
        tresure_id = argv[3];
    }
    else
    {
        tresure_id=NULL;
    }

    if(strcmp(action,"add")==0)
    {
        add(hunt_id);
    }
    else if (strcmp(action,"list")==0)
    {
        list(hunt_id);
    }
    else if (strcmp(action,"view")==0)
    {
        view(hunt_id,tresure_id);
    }
    else if (strcmp(action,"remove_tresure")==0)
    {
        remove_tresure(hunt_id,tresure_id);
    }
    else if (strcmp(action,"remove_hunt")==0)
    {
        remove_hunt(hunt_id);
    }
    else
    {
        printf("Invalid operation\n");
    }

    
    
    
    
    return 0;
}