#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>

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

enum errors clear_input_buffer() {
    int extra_char = getchar();
    if (extra_char != '\n' && extra_char != EOF) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return INVALID_INPUT;
    }
    return OK;
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

enum errors check_if_exists(const char* username, int* stored_pin) {
    FILE* file = fopen("all_users.txt", "r");
    if (!file) return INVALID_MEMORY;

    char file_username[7];
    int pin;
    int sessions;

    while (fscanf(file, "%6s %d %d", file_username, &pin, &sessions) == 3) {
        if (strcmp(file_username, username) == 0) {
            *stored_pin = pin;
            fclose(file);
            return ALREADY_EXISTS;
        }
    }

    fclose(file);
    return OK;
}

enum errors find_number_of_sessions(const char* username, int* result) {
    FILE* file = fopen("all_users.txt", "r");
    if (!file) return INVALID_MEMORY;

    char file_username[7];
    int pin;
    int sessions;

    while (fscanf(file, "%6s %d %d", file_username, &pin, &sessions) == 3) {
        if (strcmp(file_username, username) == 0) {
            *result = sessions;
            fclose(file);
            return OK;
        }
    }

    fclose(file);
    return INVALID_INPUT;
}

enum errors registration() {
    char username[7];
    char pin_str[7];
    int PIN;

    printf("Введите логин (до 6 латинских символов или цифр): ");
    scanf("%6s", username);
    enum errors check_user = clear_input_buffer();
    if (check_user != OK) {
        return INVALID_INPUT;
    }

    for (int i = 0; username[i]; i++) {
        if (!isalnum(username[i])) {
            return INVALID_INPUT;
        }
    }

    int stored_pin;
    if (check_if_exists(username, &stored_pin) == ALREADY_EXISTS) {
        return ALREADY_EXISTS;
    }

    printf("Введите PIN (от 0 до 100000): ");
    scanf("%6s", pin_str);
    enum errors check_password = clear_input_buffer();
    if (check_password != OK) {
        return INVALID_INPUT;
    }

    if (str_to_int(pin_str, &PIN) != OK) {
        return INVALID_INPUT;
    }

    FILE* file = fopen("all_users.txt", "a");
    if (!file) {
        return INVALID_MEMORY;
    }

    fprintf(file, "%s %d -1\n", username, PIN);
    fclose(file);

    return OK;
}

enum errors login(char* logged_user) {
    char username[7];
    char pin_str[7];
    int input_pin, stored_pin;

    printf("Введите логин: ");
    scanf("%6s", username);
    enum errors check_user = clear_input_buffer();
    if (check_user != OK) {
        return INVALID_INPUT;
    }

    if (check_if_exists(username, &stored_pin) != ALREADY_EXISTS) {
        return NOT_DECLARED;
    }

    printf("Введите PIN: ");
    scanf("%6s", pin_str);
    enum errors check_password = clear_input_buffer();
    if (check_password != OK) {
        return INVALID_INPUT;
    }

    if (str_to_int(pin_str, &input_pin) != OK || input_pin != stored_pin) {
        return WRONG_PIN;
    }

    strcpy(logged_user, username);
    printf("Вход выполнен успешно!\n");
    return OK;
}

void print_menu() {
    printf("\nДоступные команды:\n");
    printf("Time - текущее время\n");
    printf("Date - текущая дата\n");
    printf("Howmuch <time> flag - время с момента time (-s, -m, -h, -y)\n");
    printf("Sanctions username <number> - ограничение команд\n");
    printf("Logout - выход\n");
}

int is_valid_time_format(const char *time_str) {
    struct tm tm = {0};
    return strptime(time_str, "%d:%m:%Y", &tm) != NULL;
}

int is_valid_flag(const char *flag) {
    return (strcmp(flag, "-s") == 0 || strcmp(flag, "-m") == 0 ||
            strcmp(flag, "-h") == 0 || strcmp(flag, "-y") == 0);
}

void process_howmuch_command(const char *command) {
    char *cmd_copy = strdup(command);
    char *token = strtok(cmd_copy, " ");

    if (!token || strcmp(token, "Howmuch") != 0) {
        free(cmd_copy);
        return;
    }

    char *time_str = strtok(NULL, " ");
    char *flag = strtok(NULL, " ");

    if (!time_str || !flag) {
        printf("Ошибка: неправильный формат команды.\n");
        free(cmd_copy);
        return;
    }

    if (!is_valid_time_format(time_str)) {
        printf("Ошибка: некорректный формат даты. Используйте dd:mm:yyyy.\n");
        free(cmd_copy);
        return;
    }

    if (!is_valid_flag(flag)) {
        printf("Ошибка: некорректный флаг. Доступны -s, -m, -h, -y.\n");
        free(cmd_copy);
        return;
    }

    struct tm input_time = {0};
    strptime(time_str, "%d:%m:%Y", &input_time);
    time_t input_epoch = mktime(&input_time);
    time_t current_epoch = time(NULL);

    if (input_epoch == -1) {
        printf("Ошибка: не удалось обработать дату.\n");
        free(cmd_copy);
        return;
    }

    double diff = difftime(current_epoch, input_epoch);

    if (strcmp(flag, "-s") == 0) {
        printf("Прошло %.0f секунд\n", diff);
    }
    else if (strcmp(flag, "-m") == 0) {
        printf("Прошло %.0f минут\n", diff / 60);
    }
    else if (strcmp(flag, "-h") == 0) {
        printf("Прошло %.0f часов\n", diff / 3600);
    }
    else if (strcmp(flag, "-y") == 0) {
        printf("Прошло %.2f лет\n", diff / (3600 * 24 * 365.25));
    }

    free(cmd_copy);
}

enum errors process_sanctions(const char* command) {
    int stored_pin, limit;
    char *cmd_copy = strdup(command);
    char *token = strtok(cmd_copy, " ");

    if (!token || strcmp(token, "Sanctions") != 0) {
        free(cmd_copy);
        return INVALID_MEMORY;
    }

    char *username = strtok(NULL, " ");
    char *number = strtok(NULL, " ");

    if (!username || !number) {
        free(cmd_copy);
        return INVALID_INPUT;
    }

    if (check_if_exists(username, &stored_pin) != ALREADY_EXISTS) {
        free(cmd_copy);
        return INVALID_INPUT;
    }

    if (str_to_int(number, &limit) != OK || limit <= 0) {
        free(cmd_copy);
        return INVALID_INPUT;
    }

    FILE* file = fopen("all_users.txt", "r");
    if (!file) {
        free(cmd_copy);
        return INVALID_MEMORY;
    }

    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(file);
        free(cmd_copy);
        return INVALID_MEMORY;
    }

    char file_username[7];
    int pin;
    int sessions;

    while (fscanf(file, "%6s %d %d", file_username, &pin, &sessions) == 3) {
        if (strcmp(file_username, username) == 0) {
            fprintf(temp, "%s %d %d\n", file_username, pin, limit);
        }
        else {
            fprintf(temp, "%s %d %d\n", file_username, pin, sessions);
        }
    }

    fclose(file);
    fclose(temp);

    remove("all_users.txt");
    rename("temp.txt", "all_users.txt");

    free(cmd_copy);
    return OK;
}

void process_command(const char* command, const char* user) {
    if (strcmp(command, "Time") == 0) {
        time_t now = time(NULL);
        struct tm *timeinfo = localtime(&now);
        char buffer[9];
        strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
        printf("Текущее время: %s\n", buffer);
    }
    else if (strcmp(command, "Date") == 0) {
        time_t now = time(NULL);
        struct tm *timeinfo = localtime(&now);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%d:%m:%Y", timeinfo);
        printf("Текущая дата: %s\n", buffer);
    }
    else if (strncmp(command, "Howmuch", 7) == 0) {
        process_howmuch_command(command);
    }
    else if (strncmp(command, "Sanctions", 9) == 0) {
        printf("Для подтверждения своих действий введите 12345\n");
        long int number;
        scanf("%ld", &number);
        clear_input_buffer();
        if (number == 12345) {
            enum errors result = process_sanctions(command);
            if (result == OK) {
                printf("Ограничение установлено.\n");
            }
            else {
                printf("Ошибка при установке ограничения.\n");
            }
        }
        else printf("Не подтверждено\n");
    }
    else if (strcmp(command, "Logout") == 0) {
        printf("Выход...\n");
    }
    else {
        printf("Неизвестная команда!\n");
    }
}

int main() {

    char logged_user[7] = "";

    while (1) {
        printf("\n1. Вход\n2. Регистрация\n3. Выход\nВыберите: ");
        int choice;
        scanf("%d", &choice);
        clear_input_buffer();

        if (choice == 1) {
            enum errors result = login(logged_user);
            if (result == OK) {
                break;
            }
            else {
                print_error(result);
            }
        }
        else if (choice == 2) {
            enum errors result = registration();
            if (result == OK) {
                printf("Регистрация успешна!\n");
            }
            else {
                print_error(result);
            }
        }
        else if (choice == 3) {
            printf("Выход из программы.\n");
            return 0;
        }
        else {
            printf("Неверный ввод!\n");
        }
    }

    print_menu();

    int sessions;
    enum errors result = find_number_of_sessions(logged_user, &sessions);
    if (result != OK) {
        print_error(result);
    }

    char command[100];
    while (1) {
        if (sessions == 0) {
            printf("Количество команд для данной сессии закончилось\n");
            break;
        }
        printf("\nВведите команду: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "Logout") == 0) {
            printf("Выход из аккаунта.\n");
            break;
        }

        if (sessions != -1) sessions -= 1;

        process_command(command, logged_user);
    }

    return 0;
}