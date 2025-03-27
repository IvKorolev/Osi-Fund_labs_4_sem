#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char* argv[]){

    return 0;
}