#include "functions.h"
#include "list.h"
#include <sqlite3.h>
#include <time.h>
#include <iostream>
#include <filesystem>

void printErr(const char* msg){
  fprintf(stderr, "\033[1;31mошибка: \033[0m%s\n", msg);
}

void printHelp(){
  printf("Использование: counters [ПАРАМЕТР]…\n");
  printf("По умолчанию выдает показания на текущий день по\n");
  printf("всем счетчикам.\n");
  printf("  -с [e|g|w]  вывести информацию по выбранному счетчику,\n");
  printf("              если не указан то вывести по всем\n");
  printf("  -l          расширенная информация\n");
  printf("  -d [mmYYYY] вывести информацию за выбранный период\n");
  printf("              mm(01..12), YYYY - год\n");
  printf("  -s []\n");
  printf("  -t          показать тарифы\n");
  printf("  -T          установить тариф (исп. вместе с -c)\n");
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

void printCounters(stCountersFormat *stCF){
  time_t now = time(NULL);
  struct tm date = *localtime(&now);
  date.tm_hour = 0;   date.tm_min = 0; date.tm_sec = 0;
  date.tm_mday = 1;
  printf("Показания счетчиков на %d %s %dг\n", date.tm_mday,
         mon_name[date.tm_mon], date.tm_year+1900);
 // printf("time: %.f\n", difftime(mktime(&y2k), mktime(&y2k1)));

  sqlite3 *db;
  int error;
  error = sqlite3_open("db.sqlite3", &db);
  if (error == 0) {
    sqlite3_stmt *res;
    const char *tail;
    error = sqlite3_prepare_v2(db, "SELECT id, text, tarif FROM counters;", -1, &res, &tail);
    if (error == 0) {

      int rec_count = 0;
      while (sqlite3_step(res) == SQLITE_ROW) {
        printf("Row (%d): ", rec_count);
        printf("%s\n", sqlite3_column_text(res, 1));
        rec_count++;
      }

    } else {
       printErr(sqlite3_errmsg(db));
       printf("%d\n", error);
    }
    sqlite3_finalize(res);

  } else {
    printErr(sqlite3_errmsg(db));
  }
  sqlite3_close(db);


}
