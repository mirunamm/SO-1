#include <stdio.h>

typedef struct tresure 
{
    char id[16];
    char username[32];
    double latitude;
    double lonfitude;
    char clue[64];
    int val;

}tresure;

void add(char *hunt_id);
void list(char *hunt_id);
void view(char *hunt_id, char *tresure_id);
void remove_tresure(char *hunt_id, char *tresure_id);
void remove_hunt(char *hunt_id);
