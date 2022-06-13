#include <stdio.h>
#include "functions.h"
#include "list.h"

int main(int argc, char *argv[]){
  Status stat;
  if (argc > 1) {
    stat = Status::error;
    int i = 1;
    while (i < argc){
      if (argv[i][0] == '-'){
        if (argv[i][1] == 'h'){ //--Выводим справку
          stat = Status::help;
          printHelp();
          break;
        } else if (argv[i][1] == 'v'){ //--Выводим версию
          stat = Status::version;
          printVersion();
          break;
        }
      }
      i++;
    }

  } else {
    stat = Status::counters;
    printf("Показания\n");
  }
  if (stat == Status::error){
    printErr("Не указана операция (используйте -h для справки)");
    return 1;
  }
/*  
    sqlite3 *db;
    int error;
  error = sqlite3_open("test.db", &db);
  if (error == 0) {
    std::cout << "Connect is ok." << std::endl;
    sqlite3_stmt *res;
    const char *tail;
    error = sqlite3_prepare_v2(db, "SELECT * FROM users", 1000, &res, &tail);
    if (error == 0) {
      std::cout << "Display result from table1" << std::endl;
      int rec_count = 0;
      while (sqlite3_step(res) == SQLITE_ROW) {
        std::cout << "Row (" << rec_count << "):" << sqlite3_column_text(res, 0) << " ";
        std::cout << sqlite3_column_text(res, 1) << " " ;
        std::cout << sqlite3_column_text(res, 2) << std::endl;

        rec_count++;
      }

    } else {
      std::cout << "Can't select data: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(res);
    
  } else {
    std::cout << "Err: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_close(db);
  printerr("Невозможно открыть файл db.sqlite3");
*/  

  return 0;
}