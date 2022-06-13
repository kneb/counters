#include <stdio.h>
#include <sqlite3.h>

enum class status{
    help,
    version,
};

void printerr(const char* msg);

int main(int argc, char *argv[]){
    
    if (argc > 1) {
        int i = 1;

    }

    printf("%d\n", argc);
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

void printerr(const char* msg){
    fprintf(stderr, "\033[1;31mошибка: \033[0m%s\n", msg);
}
