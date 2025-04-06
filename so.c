#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main() {
    dir* folder;
    struct dirent* fisier;

    folder = opendir("treasure_hunt");
    if (folder == NULL) {
        perror("nu pot deschide directorul treasure_hunt");
        return 1;
    }

    printf("lista de fisiere:\n");

    while ((fisier = readdir(folder)) != NULL) {
        if (strcmp(fisier->d_name, ".") != 0 && strcmp(fisier->d_name, "..") != 0) {
            printf("%s\n", fisier->d_name);
        }
    }

    closedir(folder);
    return 0;
}
