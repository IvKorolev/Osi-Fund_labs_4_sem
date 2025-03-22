#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

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
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == -1) {
            printf("Ошибка получения статистики\n");
            continue;
        }

        if (S_ISREG(file_stat.st_mode)) {
            printf("%-20s %lu\n", entry->d_name, file_stat.st_blocks);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory> [directory2 ...]\n", argv[0]);
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
