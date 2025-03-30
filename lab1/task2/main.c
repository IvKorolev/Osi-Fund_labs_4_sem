#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <libgen.h>



#define BUFFER_SIZE 4096

enum errors {
    INVALID_INPUT,
    INVALID_MEMORY,
    OK,
    NOT_DECLARED,
    ALREADY_EXISTS,
    WRONG_PIN,
};

void print_error(enum errors err) {
    switch (err) {
    case INVALID_INPUT:
        printf("Ошибка: Некорректный ввод.\n");
        break;
    case INVALID_MEMORY:
        printf("Ошибка: Недостаточно памяти или проблемы с файлом.\n");
        break;
    case NOT_DECLARED:
        printf("Ошибка: Пользователь не найден.\n");
        break;
    case ALREADY_EXISTS:
        printf("Ошибка: Такой логин уже зарегистрирован.\n");
        break;
    case WRONG_PIN:
        printf("Ошибка: Неверный PIN-код.\n");
        break;
    default:
        break;
    }
}

enum errors str_to_int(const char* x, int* res){
    char symbol;
    if (sscanf(x, "%d%c", res, &symbol) != 1){
        return INVALID_INPUT;
    }

    if (*res >= INT_MAX || *res <= INT_MIN){
        return INVALID_INPUT;
    }

    return OK;
}


enum errors process_xor2(char* path)
{

    return OK;
}

enum errors process_xor3(char* path)
{

    return OK;
}

enum errors process_xor4(char* path)
{

    return OK;
}

enum errors process_xor5(char* path)
{

    return OK;
}

enum errors process_xor6(char* path)
{

    return OK;
}

enum errors copy_file(const char *src, const char *dest) {
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) {
        return INVALID_INPUT;
    }

    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        return INVALID_MEMORY;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        write(dest_fd, buffer, bytes_read);
    }

    close(src_fd);
    close(dest_fd);
    return OK;
}

int main(int argc, char* argv[]){

    if (argc < 2)
    {
        printf("Ошибка, ввод: пути_к_файлу флаг\n");
        return INVALID_INPUT;
    }

    if (strncmp(argv[argc - 1], "xor", 3) == 0)
    {
        enum errors result;
        switch (argv[argc - 1][3])
        {
            case '2':
                result = process_xor2(argv[argc - 1]);
                if (result != OK) printf("Ошибка сложения по модулю\n");
                break;
            case '3':
                result = process_xor3(argv[argc - 1]);
                if (result != OK) printf("Ошибка сложения по модулю\n");
                break;
            case '4':
                result = process_xor4(argv[argc - 1]);
                if (result != OK) printf("Ошибка сложения по модулю\n");
                break;
            case '5':
                result = process_xor5(argv[argc - 1]);
                if (result != OK) printf("Ошибка сложения по модулю\n");
                break;
            case '6':
                result = process_xor6(argv[argc - 1]);
                if (result != OK) printf("Ошибка сложения по модулю\n");
                break;
            default:
                printf("xor вводить от 2 до 6!\n");
                break;
        }
    }
    else if (strcmp(argv[argc - 2], "mask") == 0)
    {

    }
    else if (strncmp(argv[argc - 1], "copy", 4) == 0)
    {
        int num_copy;
        enum errors res_copy;

        if (str_to_int(argv[argc - 1] + 4, &num_copy) != OK || num_copy > 10)
        {
            printf("Ошибка. Количество копий не может превышать 10!\n");
            return INVALID_INPUT;
        }

        for (int i = 1; i < argc - 1; i++) {
            char *filepath = argv[i];

            if (access(filepath, F_OK) != 0) {
                fprintf(stderr, "Ошибка: файл '%s' не существует\n", filepath);
                continue;
            }

            char *filename = strrchr(filepath, '/');
            filename = (filename) ? filename + 1 : filepath;

            char dirpath[256];
            strncpy(dirpath, filepath, sizeof(dirpath));
            dirpath[sizeof(dirpath) - 1] = '\0';
            dirname(dirpath);

            for (int j = 1; j <= num_copy; j++)
            {
                pid_t pid = fork();

                if (pid < 0)
                {
                    perror("Ошибка fork");
                    exit(EXIT_FAILURE);
                }

                if (pid == 0)
                {
                    char new_filename[256];
                    char *dir = dirname(dirpath);
                    if (strcmp(dir, ".") == 0)
                    {
                        snprintf(new_filename, sizeof(new_filename), "%s_copy%d", filename, j);
                    }
                    else
                    {
                        snprintf(new_filename, sizeof(new_filename), "%s/%s_copy%d", dir, filename, j);
                    }

                    copy_file(filepath, new_filename);
                    printf("Создана копия: %s\n", new_filename);

                    exit(EXIT_SUCCESS);
                }
            }
        }
        while (wait(NULL) > 0);
    }
    else if (strcmp(argv[argc - 2], "find") == 0)
    {
        int count;
        enum errors res;
        for (int i = 1; i < argc - 1; ++i)
        {
            //res = process_find();
            if (res != OK)
            {
                printf("Ошибка с поиском подстроки в файле %s\n", argv[i]);
            }
        }
    }
    else
    {
        printf("Такого флага не существует!\n");
    }
    return 0;
}