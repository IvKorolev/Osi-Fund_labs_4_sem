#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_PATH 4096

void list_files(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("Ошибка открытия папки\n");
        return;
    }

    struct dirent *entry;
    struct stat file_stat;

    printf("%s:\n", path);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == -1) {
            printf("Ошибка получения статистики\n");
            continue;
        }

        if (S_ISREG(file_stat.st_mode)) {
            printf("%-20s %lu\n", entry->d_name, file_stat.st_blocks);
        }
        //printf("%-20s %lu\n", entry->d_name, entry->d_ino);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <directory> [directory2 ...]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        list_files(argv[i]);
        if (i < argc - 1) {
            printf("\n");
        }
    }

    return 0;
}
