#include "functions.h"

void printErr(const char* msg){
  fprintf(stderr, "\033[1;31mошибка: \033[0m%s\n", msg);
}

void printHelp(){
  printf("Использование: counters [ПАРАМЕТР]…\n");
  printf("По умолчанию выдает показания на текущий день по\n");
  printf("всем счетчикам.\n");
  printf("  -с [e|g|w]  вывести информацию по выбранному счетчику\n");
  printf("  -a          расширенная информация\n");
  printf("  -d [mmYY]   вывести информацию за выбранный период\n");
  printf("              mm(01..12), YY - год\n");
  printf("  -t          показать тарифы\n");
  printf("  -T          установить тарифы\n");
  printf("  -m          кол-во месяцев отображаемых за период\n");
  printf("  -h          показать справку\n");
  printf("  -v          показать версию\n");
}

void printVersion(){
printf("counters v1.0 (Журнал показаний счетчиков)\n");
printf("Copyright (C) 2022 Neb K.V.\n");
printf("E-mail: neb_kv@mail.ru\n");
printf("Автор: Неб Константин Викторович\n");
}