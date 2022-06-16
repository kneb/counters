#include <stdio.h>
#include "functions.h"
#include "list.h"
#include <unistd.h>

int main(int argc, char *argv[]){
  Status stat;
  stCountersFormat stCF = {false, 0, 0};
  if (argc > 1) { //--Если аргументов больше 1
    stat = Status::counters;
    int i = 1;
    while (i < argc){ //--Разбираем аргументы
      if (argv[i][0] == '-'){
        if (argv[i][1] == 'h'){ //--Выводим справку
          stat = Status::help;      
          break;
        } else if (argv[i][1] == 'v'){ //--Выводим версию
          stat = Status::version;       
          break;
        } else if (argv[i][1] == 'l'){ //--Расширенная информация по счетчикам
          stCF.extended = true;
        } else {
          stat = Status::error;
        }

      } else {
        stat = Status::error;
        break;
      }
      i++;
    }

  } else { //--Запустили без аргументов
    stat = Status::counters;
  }

  if (stat == Status::error){
    printErr("Аргументы заданы неправильно (используйте -h для справки)");
    return 1;
  } else if (stat == Status::help){
    printHelp();
  } else if (stat == Status::version){
    printVersion();
  } else if (stat == Status::counters){
    printCounters(&stCF);
  }


  return 0;
}
