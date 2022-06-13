#include "functions.h"

void printerr(const char* msg){
    fprintf(stderr, "\033[1;31mошибка: \033[0m%s\n", msg);
}