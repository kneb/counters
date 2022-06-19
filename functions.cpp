#include "functions.h"
#include <sqlite3.h>
#include <time.h>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <limits.h>

void decMonth(struct tm *date){
  if (date->tm_mon > 0){
    date->tm_mon --;
  } else {
    date->tm_mon = 11;
    date->tm_year --;
  }
  mktime(date);
}

std::string getCurPath(){
  char buf[PATH_MAX];
  readlink("/proc/self/exe", buf, sizeof(buf)-1);
  std::filesystem::path p(buf);
  return p.parent_path();
}

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
  printf("  -d [mmYYYY] вывести/установить информацию за выбранный период\n");
  printf("              mm(01..12), YYYY - год\n");
  printf("  -t          показать тарифы\n");  
  printf("  -m          кол-во месяцев отображаемых за период\n");
  printf(" Используемые вместе с -c):\n");
  printf("  -B [%%d]     установить показания счетчика на начало\n");
  printf("              заданного периода\n");
  printf("  -S [%%d]     установить показания счетчика + заданное\n");
  printf("              количество в параметре\n");
  printf("  -E [%%d]     установить показания счетчика на конец\n");
  printf("              заданного периода\n");
  printf("  -T          установить тариф\n");
  printf(" Инфо:\n");
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
  printf("Показания счетчиков на %d %s %dг\n", date.tm_mday,
         global::monthName[date.tm_mon], date.tm_year+1900);
  int day = date.tm_mday;
  date.tm_hour = 0;   date.tm_min = 0; date.tm_sec = 0;
  date.tm_mday = 1;

  sqlite3 *db;
  int error;
  std::string filename = global::currentPath+"/db.sqlite3";
  error = sqlite3_open(filename.c_str(), &db);
  if (error == SQLITE_OK) {
    sqlite3_stmt *res;
    //char *tail;
    error = sqlite3_prepare_v2(db, "SELECT id, text, log.tarif, log.\"begin\", \
                      log.\"end\", log.\"data\" FROM counters LEFT JOIN log ON \
                      log.id_counter = counters.id;", -1, &res, NULL);
    if (error == SQLITE_OK) {
      int rec_count = 0;
      while (sqlite3_step(res) == SQLITE_ROW) {
        printf("Row (%d): ", rec_count);
        printf("%s\n", sqlite3_column_text(res, 1));
        rec_count++;
      }



    } else {
       printErr(sqlite3_errmsg(db));
    }
    sqlite3_finalize(res);
  } else {
    printErr(sqlite3_errmsg(db));
  }
  sqlite3_close(db);
}

void printTarifs(stCountersFormat *stCF){
  time_t now = time(NULL);
  struct tm date;
  if (stCF->dataSet == false)
    date = *localtime(&now);
  else
    date = stCF->data;
  printf("== Тарифы на %02d %s %dг ==\n", date.tm_mday,
         global::monthName[date.tm_mon], date.tm_year+1900);
  date.tm_hour = 0;   date.tm_min = 0; date.tm_sec = 0;
  date.tm_mday = 1;
  mktime(&date);

  sqlite3 *db;
  int error;
  std::string filename = global::currentPath+"/db.sqlite3";
  error = sqlite3_open(filename.c_str(), &db);
  if (error == SQLITE_OK) {
    sqlite3_stmt *res;
    //char *tail;
    error = sqlite3_prepare_v2(db, "SELECT id, text, log.tarif, log.\"begin\", \
                      log.\"end\", log.\"data\" FROM counters LEFT JOIN log ON \
                      log.id_counter = counters.id;", -1, &res, NULL);
    if (error == SQLITE_OK) {
      int rec_count = 0;
      while (sqlite3_step(res) == SQLITE_ROW) {
        printf("Row (%d): ", rec_count);
        printf("%s\n", sqlite3_column_text(res, 1));
        rec_count++;
      }



    } else {
       printErr(sqlite3_errmsg(db));
    }
    sqlite3_finalize(res);
  } else {
    printErr(sqlite3_errmsg(db));
  }
  sqlite3_close(db);
}