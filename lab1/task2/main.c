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
#include <stdint.h>

#define BUFFER_SIZE 4096

enum errors {
    INVALID_INPUT,
    INVALID_MEMORY,
    OK,
    NOT_DECLARED,
    ALREADY_EXISTS,
    WRONG_PIN,
};

void print_error(enum errors err)
{
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

enum errors str_to_int(const char* x, int* res)
{
    char symbol;
    if (sscanf(x, "%d%c", res, &symbol) != 1)
    {
        return INVALID_INPUT;
    }

    if (*res >= INT_MAX || *res <= INT_MIN)
    {
        return INVALID_INPUT;
    }

    return OK;
}

enum errors str_to_uint(const char *str, unsigned int *out, int base)
{
    if (str == NULL || *str == '\0' || out == NULL || base < 2 || base > 36)
    {
        return INVALID_INPUT;
    }
    char *end_ptr;
    unsigned long value = strtoul(str, &end_ptr, base);
    if (end_ptr == str || *end_ptr != '\0' || value > UINT_MAX)
    {
        return INVALID_INPUT;
    }
    *out = (unsigned int) value;
    return OK;
}

enum errors xor2(FILE *file)
{
    if (file == NULL)
    {
        return INVALID_INPUT;
    }
    char one;
    char result = 0x00;
    one = 0x00;
    while (fread(&one, 1, 1, file))
    {
        char first_4_bite = one >> 4;

        char last_4_bite = one & 0x0F;
        result ^= first_4_bite;
        result ^= last_4_bite;
    }
    printf("xor2 result is: %x\n", result);
    return OK;
}

enum errors xor3(FILE *file)
{
    if (file == NULL)
    {
        return INVALID_INPUT;
    }
    char result = 0x00;
    char one = 0x00;
    while (fread(&one, 1, 1, file))
    {
        result ^= one;
    }
    printf("xor3 result is: %x\n", result);
    return OK;
}

enum errors xor4(FILE *file)
{
    if (file == NULL)
    {
        return INVALID_INPUT;
    }
    union
    {
        char bytes[2];
        uint16_t num;
    } result;

    result.num = 0;
    char bytes_buffer[2] = {0x00, 0x00};
    int bytes_read = 0;
    while ((bytes_read = fread(&bytes_buffer, sizeof(bytes_buffer), 1, file)) != 0)
    {
        if (bytes_read < sizeof(bytes_buffer))
        {
            for (size_t i = bytes_read; i < sizeof(bytes_buffer); i++)
            {
                bytes_buffer[i] = 0x00;
            }
        }
        for (int i = 0; i < sizeof(bytes_buffer); i++)
        {
            result.bytes[i] ^= bytes_buffer[i];
        }
        memset(bytes_buffer, 0x00, sizeof(bytes_buffer));
    }
    printf("xor4 result is: ");
    for (int i = 0; i < sizeof(bytes_buffer); i++)
    {
        printf("%x ", result.bytes[i]);
    }
    printf("\nNUMBER IS  %d(10) %x(hex)\n", result.num, result.num);
    return OK;
}

enum errors xor5(FILE *file)
{
    if (file == NULL)
    {
        return INVALID_INPUT;
    }
    union
    {
        char bytes[4];
        uint32_t num;
    } result;
    result.num = 0;
    char bytes_buffer[4] = {0x00, 0x00, 0x00, 0x00};
    int bytes_read = 0;
    while ((bytes_read = fread(&bytes_buffer, sizeof(bytes_buffer), 1, file)))
    {
        if (bytes_read < sizeof(bytes_buffer))
        {
            for (int i = bytes_read; i < sizeof(bytes_buffer); i++)
            {
                bytes_buffer[i] = 0x00;
            }
        }
        for (int i = 0; i < sizeof(bytes_buffer); i++)
        {
            result.bytes[i] ^= bytes_buffer[i];
        }
    }
    printf("xor5 result is: ");
    for (int i = 0; i < sizeof(bytes_buffer); i++)
    {
        printf("%x ", result.bytes[i]);
    }
    printf("\nNUMBER IS  %d(10) %x(hex)\n", result.num, result.num);
    return OK;
}

enum errors xor6(FILE *file)
{
    if (file == NULL)
    {
        return INVALID_INPUT;
    }
    union
    {
        char bytes[8];
        uint64_t num;
    } result;
    result.num = 0;
    char bytes_buffer[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int bytes_read = 0;
    while ((bytes_read = fread(&bytes_buffer, sizeof(bytes_buffer), 1, file)))
    {
        if (bytes_read < sizeof(bytes_buffer))
        {
            for (int i = bytes_read; i < sizeof(bytes_buffer); i++)
            {
                bytes_buffer[i] = 0x00;
            }
        }
        for (int i = 0; i < sizeof(bytes_buffer); i++)
        {
            result.bytes[i] ^= bytes_buffer[i];
        }
    }
    printf("xor6 result is: ");
    for (int i = 0; i < sizeof(bytes_buffer); i++)
    {
        printf("%x ", result.bytes[i]);
    }
    printf("\nNUMBER IS  %ld(10) %lx(hex)\n", result.num, result.num);
    return OK;
}

enum errors xor_action(char **file_paths, const int file_count, const int N)
{
    if (file_paths == NULL || N < 2 || N > 6)
    {
        return INVALID_INPUT;
    }

    for (int i = 0; i < file_count - 1; i++)
    {
        FILE *file = fopen(file_paths[i], "rb");
        if (file == NULL) {
            return INVALID_INPUT;
        }

        printf("Processing file: %s: \n", file_paths[i]);

        if (N == 2)
        {
            xor2(file);
        }
        else if (N == 3)
        {
            xor3(file);
        }
        else if (N == 4)
        {
            xor4(file);
        }
        else if (N == 5)
        {
            xor5(file);
        }
        else if (N == 6)
        {
            xor6(file);
        }
        else
        {
            printf("N must be in {2,3,4,5,6}\n");
            fclose(file);
            return INVALID_INPUT;
        }

        fclose(file);
    }

    return OK;
}

uint32_t swap_bytes(uint32_t value)
{
    // big to lit endian
    return ((value & 0xFF000000) >> 24) |
           ((value & 0x00FF0000) >> 8) |
           ((value & 0x0000FF00) << 8) |
           ((value & 0x000000FF) << 24);
}

enum errors mask_action(char **files, int size, const char *attr)
{
    if (files == NULL || attr == NULL)
    {
        return INVALID_INPUT;
    }

    unsigned int mask;
    if (str_to_uint(attr, &mask, 16) != OK)
    {
        return INVALID_INPUT;
    }

    for (int i = 0; i < size - 1; i++) {
        FILE *file = fopen(files[i], "rb");
        if (file == NULL)
        {
            printf("Ошибка открытия файла: %s\n", files[i]);
            return INVALID_MEMORY;
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (file_size % sizeof(uint32_t) != 0)
        {
            printf("File size is not a multiple of 4 bytes\n");
            fclose(file);
            return INVALID_INPUT;
        }

        int count = 0;

        uint32_t number;
        while (fread(&number, sizeof(uint32_t), 1, file) == 1)
        {
            number = swap_bytes(number);
            printf("Read number: 0x%x\n", number);
            if ((number & mask) == mask)
            {
                count++;
            }
        }

        fclose(file);
        printf("Числа, соответствующие маске %s (0x%x): %d\n", attr, mask, count);
    }

    return OK;
}

enum errors copy_file(const char *src, const char *dest)
{
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0)
    {
        return INVALID_INPUT;
    }

    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0)
    {
        return INVALID_MEMORY;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0)
    {
        write(dest_fd, buffer, bytes_read);
    }

    close(src_fd);
    close(dest_fd);
    return OK;
}

int find_string_in_file(const char *filename, const char *search_str)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Ошибка открытия файла %s\n", filename);
        return 0;
    }

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, search_str))
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int main(int argc, char* argv[]){

    if (argc < 2)
    {
        printf("Ошибка, ввод: пути_к_файлам флаг\n");
        return INVALID_INPUT;
    }

    if (strncmp(argv[argc - 1], "xor", 3) == 0)
    {
        const int N = argv[argc - 1][3] - '0';
        printf("N = %d\n", N);
        return xor_action(argv + 1, argc - 1, N);
    }
    else if (strcmp(argv[argc - 2], "mask") == 0)
    {
        return mask_action(argv + 1, argc - 2, argv[argc - 1]);
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

        for (int i = 1; i < argc - 1; i++)
        {
            char *filepath = argv[i];

            if (access(filepath, F_OK) != 0)
            {
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
        char *search_str = argv[argc - 1];
        int found_any = 0;
        for (int i = 1; i < argc - 2; i++)
        {
            FILE *list_file = fopen(argv[i], "r");
            if (!list_file)
            {
                printf("Ошибка открытия файла: %s\n", argv[i]);
                continue;
            }
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("Ошибка fork()");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                if (find_string_in_file(argv[i], search_str))
                {
                    printf("Найдено в файле: %s\n", argv[i]);
                    exit(EXIT_SUCCESS);
                }
                exit(EXIT_FAILURE);
            }
            fclose(list_file);
        }

        int status;
        while (wait(&status) > 0)
        {
            if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
            {
                found_any = 1;
            }
        }

        if (!found_any)
        {
            printf("Строка не найдена ни в одном из файлов.\n");
        }
    }
    else
    {
        printf("Такого флага не существует!\n");
    }
    return 0;
}