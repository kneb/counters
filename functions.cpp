#include "functions.h"
#include "global.h"
#include <cstddef>
#include <cstdio>
#include <time.h>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <string.h>

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
  printf("\nИспользуемые только вместе с -c):\n");
  printf("  -B [%%d]     установить показания счетчика на начало\n");
  printf("              заданного периода\n");
  printf("  -S [%%d]     установить показания счетчика + заданное\n");
  printf("              количество в параметре\n");
  printf("  -E [%%d]     установить показания счетчика на конец\n");
  printf("              заданного периода\n");
  printf("  -T [%%f]     установить тариф\n");
  printf("\nИнформация:\n");
  printf("  -h          показать справку\n");
  printf("  -v          показать версию\n");
}

void printVersion(){
  printf("counters v1.0 (Журнал показаний счетчиков)\n");
  printf("Copyright (C) 2022 Neb K.V.\n");
  printf("E-mail: neb_kv@mail.ru\n");
  printf("Автор: Неб Константин Викторович\n");
}

int getSizeFormat(int w, const char* str){
  int len = strlen(str);
  return w-len/2+len;
}

void printCounters(stCountersFormat *stCF){
  time_t now = time(NULL);
  struct tm *date = localtime(&now);
  if (stCF->dataSet == true) {
    *date = stCF->data;
  }
  int day = date->tm_mday;
  date->tm_hour = 0;   date->tm_min = 0; date->tm_sec = 0;
  date->tm_mday = 1;

  time_t dateEnd = mktime(date);
  time_t dateBegin = global::decMonth(date, stCF->monthCount-1);

  sqlite3 *db;
  int error;
  std::string filename = global::currentPath+"/db.sqlite3";
  error = sqlite3_open(filename.c_str(), &db);
  if (error == SQLITE_OK) {
    sqlite3_stmt *res;
    int id_counter_min = (stCF->id_counter == 0)?1:stCF->id_counter;
    int id_counter_max = (stCF->id_counter == 0)?3:stCF->id_counter;      
    error = sqlite3_prepare_v2(db, "SELECT id, text, measure, log.tarif, \
                        log.\"begin\", log.\"end\", log.\"data\" FROM counters \
                        LEFT JOIN log ON log.id_counter = counters.id WHERE \
                        log.id_counter >= ?1 AND log.id_counter <= ?2 AND \
                        log.\"data\"<=?3 AND log.\"data\">=?4 ORDER BY log.\"data\" \
                        DESC, log.id_counter ASC;", -1, &res, NULL);
    sqlite3_bind_int(res, 1, id_counter_min);
    sqlite3_bind_int(res, 2, id_counter_max);
    sqlite3_bind_int(res, 3, dateEnd);
    sqlite3_bind_int(res, 4, dateBegin);    
    if (error == SQLITE_OK) {
      time_t old_date = 0;
      while (sqlite3_step(res) == SQLITE_ROW) {  
        time_t cur_date = sqlite3_column_int(res, 6);
        if (old_date != cur_date){ // Следующий месяц
          old_date = cur_date;
          date = localtime(&cur_date);
          if (stCF->dataSet == true || stCF->monthCount > 1){
            printf("--= Показания счетчиков за %s %dг =--\n",
              global::monthName_i[date->tm_mon], date->tm_year+1900);
          } else {
            printf("--= Показания счетчиков на %d %s %dг =--\n", day,
              global::monthName_v[date->tm_mon], date->tm_year+1900);
          }
        }
        int indb = sqlite3_column_int(res, 4);
        int inde = sqlite3_column_int(res, 5);
        int ind = inde - sqlite3_column_int(res, 4);
        double tarif = sqlite3_column_double(res, 3);
        if (stCF->dataSet == false && stCF->monthCount == 1){ // расчет показаний на текущий день
          ind = round((double)ind * day / global::getDayMonth(date->tm_mon, 
          date->tm_year+1900));
          inde = indb + ind;
        }
        // std::ios state(nullptr);
        // state.copyfmt(std::cout);
        // std::cout << std::setw(15) << sqlite3_column_text(res, 1);
        // std::cout.copyfmt(state);
        printf("%*s: %d (%d %s)\n", getSizeFormat(15, (const char*)sqlite3_column_text(res, 1)), sqlite3_column_text(res, 1), inde, ind, sqlite3_column_text(res, 2));          
        if (stCF->extended == true){ // Расширенная версия
          printf("\t%d %s х тариф: %.2f руб/%2$s = %.2f руб.\n", ind, sqlite3_column_text(res, 2), tarif, ind*tarif);
          printf("\t\tначальное: %d\tконечное: %d\n", indb, inde);
        } 
        
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
         global::monthName_v[date.tm_mon], date.tm_year+1900);
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

void insUpdLog(stCountersFormat *stCF){
  if (stCF->id_counter == 0){
    printErr("Пропущен обязательный параметр -c");
    return;
  }
  time_t now = time(NULL);
  struct tm date;
  if (stCF->dataSet == false)
    date = *localtime(&now);
  else
    date = stCF->data;
  date.tm_hour = 0;   date.tm_min = 0; date.tm_sec = 0;
  date.tm_mday = 1;
  time_t unixdate = mktime(&date);
  sqlite3 *db;
  int error;
  std::string filename = global::currentPath+"/db.sqlite3";
  error = sqlite3_open(filename.c_str(), &db);
  if (error == SQLITE_OK) {
    sqlite3_stmt *res;
    bool isNewRecord = !isExistRowLog(db, stCF->id_counter, unixdate);
    char *errmsg;
    RowLog row = {0,0,0.0};

      RowLog prev_row = getIndicationRow(db, stCF->id_counter, global::decMonth(&date));
      if (stCF->tarif > 0){
        row.tarif = stCF->tarif;
      } else {
        row.tarif = prev_row.tarif;
      }
      if (stCF->setIndication == Indication::begin){//введены только начальные показания
        row.begin = stCF->beginInd;
        row.end = stCF->beginInd;
      } else if (stCF->setIndication == Indication::end){//введены только конечные показания
        row.begin = prev_row.end;
        row.end = stCF->endInd;
      } else if (stCF->setIndication == Indication::amount){//введено только кол-во
        row.begin = prev_row.end;
        row.end = row.begin + stCF->amount;
      } else if (stCF->setIndication == Indication::all){
        row.begin = stCF->beginInd;
        row.end = (stCF->amount > 0) ? row.begin + stCF->amount :
                  stCF->endInd;
      }    
    char *sql = NULL;
    if (isNewRecord == true){ //если записи не существует  
      asprintf(&sql, "INSERT INTO log VALUES (?1, ?2, ?3, ?4, ?5);");
      sqlite3_prepare_v2(db, sql, -1, &res, NULL);
      sqlite3_bind_int(res, 1, stCF->id_counter);
      sqlite3_bind_int(res, 2, row.begin);
      sqlite3_bind_int(res, 3, row.end);
      sqlite3_bind_double(res, 4, row.tarif);
      sqlite3_bind_int(res, 5, unixdate);

    } else { //обновляем данные
      asprintf(&sql, "UPDATE log SET \"begin\"=?1, \"end\"=?2, tarif=?3 WHERE \
                      id_counter=?4 AND \"data\"=?5;");
      sqlite3_prepare_v2(db, sql, -1, &res, NULL);
      sqlite3_bind_int(res, 4, stCF->id_counter);
      sqlite3_bind_int(res, 1, row.begin);
      sqlite3_bind_int(res, 2, row.end);
      sqlite3_bind_double(res, 3, row.tarif);
      sqlite3_bind_int(res, 5, unixdate);
      
    }
    sqlite3_step(res);

    if (error == SQLITE_OK) {
      printf("Данные обновлены! :)\n");
    } else {
       printErr(sqlite3_errmsg(db));
       printf("%s\n", errmsg);
    }
    sqlite3_finalize(res);
  } else {
    printErr(sqlite3_errmsg(db));
  }
  sqlite3_close(db);
}

bool isExistRowLog(sqlite3 *db, int id_counter, time_t unixdate){
  sqlite3_stmt *res;
  char *sql = NULL;
  asprintf(&sql, "SELECT COUNT(id_counter) FROM log WHERE id_counter=%d AND \"data\"=%ld",
             id_counter, unixdate);
    int error = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    bool exist = false;
    if (error == SQLITE_OK) {
      if (sqlite3_step(res) == SQLITE_ROW) {
        int count = sqlite3_column_int(res, 0);
        if (count == 1) exist = true;        
      }
    } else {
       printErr(sqlite3_errmsg(db));
    }
    sqlite3_finalize(res);
    return exist;
}

RowLog getIndicationRow(sqlite3 *db, int id_counter, time_t unixdate){
    sqlite3_stmt *res;
    char *sql = NULL;
    asprintf(&sql, "SELECT \"begin\",\"end\",tarif FROM log WHERE id_counter=%d AND \"data\"=%ld",
             id_counter, unixdate);
    int error = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    RowLog result = {0, 0, 0.0};
    if (error == SQLITE_OK) {
      if (sqlite3_step(res) == SQLITE_ROW) {
        result.begin = sqlite3_column_int(res, 0);
        result.end = sqlite3_column_int(res, 1);
        result.tarif = sqlite3_column_double(res, 2);
      }
    } else {
       printErr(sqlite3_errmsg(db));
    }
    sqlite3_finalize(res);
    return result;
}
