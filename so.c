#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "tresure_manager.h"
#include <time.h>
#include <fcntl.h>    // Pentru open(), O_WRONLY, O_APPEND, O_CREAT
#include <unistd.h>   // Pentru close(), write(), mkdir()
//#include <sys/types.h> // Pentru mkdir()
#include <sys/stat.h>  // Pentru mkdir()

void hunt_path(char *buff, size_t size,  char *hunt_id) {
    snprintf(buff, size, "%s/", hunt_id);
}

void file_path(char *buff, size_t size, char *hunt_id, char *filename) {
    char dir[256];
    hunt_path(dir, sizeof(dir), hunt_id);
    snprintf(buff, size, "%s/%s", dir, filename);
}

void log_action(char *hunt_id, char *action)
{
    char log_path[256];
    file_path(log_path, sizeof(log_path),hunt_id, "logged_hunt");

    int fd= open(log_path, O_WRONLY | O_APPEND| O_CREAT, 0666);
    
    if(fd <0)
    return;

    time_t now = time(NULL);
    char buff[256];
    snprintf(buff, sizeof(buff), "[%s] %s\n", ctime(&now), action);
    write(fd, buff, strlen(buff));
    close(fd);

    char symlink_path[256];
    snprintf(symlink_path, sizeof(symlink_path), "logged_hunt-%s", hunt_id);
    symlink(log_path, symlink_path);
}

void add(char *hunt_id)
{
    char dir[256];

    hunt_path(dir,sizeof(dir),hunt_id);

    if (opendir(dir) == NULL) 
    {
        if (mkdir(dir, 0755) != 0)
        {   
        
            printf("error creating hunt directory\n");
            return;
        }
    }

    treasure t;
    printf("Enter treasure ID: "); scanf("%s", t.id);
    printf("Enter username: "); scanf("%s", t.username);
    printf("Enter latitude: "); scanf("%lf", &t.latitude);
    printf("Enter longitude: "); scanf("%lf", &t.longitude);
    printf("Enter clue: "); scanf("%s", t.clue);
    printf("Enter value: "); scanf("%d", &t.val);

    char file[256];
    file_path(file,sizeof(file),hunt_id,hunt_id);

    int fd = open(file, O_WRONLY | O_APPEND | O_CREAT, 0666);

    if(fd == -1)
    {
        printf("error");
        return;
    }

    if(write(fd, &t, sizeof(treasure)) == -1)
    {
        printf("error");
        close(fd);
        return;
    }

    close(fd);
    log_action(hunt_id,"Added treasure");

}
void list(char *hunt_id)
{
    char file[256];
    file_path(file, sizeof(file), hunt_id, hunt_id);
    printf("%s",file);
    struct stat s;

    if(stat(file,&s) == -1)
    {
        printf("stat failed\n");
        return;
    }

    printf("Hunt: %s\n", hunt_id);
    printf("Size: %ld bytes\n", s.st_size);
    printf("Last modified: %s", ctime(&s.st_mtime));


    int fd= open(file,O_RDONLY);

    if(fd<0)
    {
        printf("cannot open treasure file");
        return;


    }

    treasure t;

    while(read(fd,&t,sizeof(treasure)))
    {
        printf("- ID: %s | User: %s | (%.2f, %.2f) | Value: %d\n", t.id, t.username, t.latitude, t.longitude, t.val);
    }
    close(fd);
    log_action(hunt_id, "Listed treasures");
}
void view(char *hunt_id, char *treasure_id)
{
    char file[256];
    file_path(file,sizeof(file),hunt_id,hunt_id);

    int fd = open(file, O_RDONLY);

    if(fd <0)
    {
        printf("cannot open treasure file \n");
        return;
    }

    treasure t;
    int ok=0;

    while(read(fd,&t,sizeof(treasure)))
    {
        if(strcmp(t.id,treasure_id)==0)
        {
            printf("ID: %s\nUser: %s\nLat: %.2f\nLon: %.2f\nClue: %s\nValue: %d\n", t.id, t.username, t.latitude, t.longitude, t.clue, t.val);
            ok=1;
            break;
        }
    }

    if(!ok)
    printf("treasure not found\n");

    close(fd);
    log_action(hunt_id,"View treasure");
}

void remove_treasure(char *hunt_id, char *treasure_id)
{

}

void remove_hunt(char *hunt_id)
{

}

