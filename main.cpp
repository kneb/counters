#include <stdio.h>
#include "functions.h"
#include "global.h"
#include <iostream>
#include <filesystem>
#include <unistd.h>

int main(int argc, char *argv[]){
  Status stat;
  stCountersFormat stCF = {false, 0, 0, 0, false};
  stCF.data.tm_hour = 0;   stCF.data.tm_min = 0; stCF.data.tm_sec = 0;
  stCF.data.tm_mday = 1;
  global::currentPath = getCurPath();

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
        } else if (argv[i][1] == 'c'){ //--Выбрать счетчик
          i++;
          if (i < argc){
            if (argv[i][0] == '1' || argv[i][0] == 'e')
              stCF.id_counter = 1;
            else if (argv[i][0] == '2' || argv[i][0] == 'g')
              stCF.id_counter = 2;
            else if (argv[i][0] == '3' || argv[i][0] == 'w')
              stCF.id_counter = 3;
            else stat = Status::error;
          } else stat = Status::error;
        } else if (argv[i][1] == 'd'){ //--За какой месяц информация
          i++;
          if (i < argc){
            int yy = 0;
            sscanf(argv[i], "%02d%04d", &stCF.data.tm_mon, &yy);
            stCF.data.tm_mon --;
            stCF.data.tm_year = yy-1900;
            stCF.dataSet = true;
          } else stat = Status::error;
        } else if (argv[i][1] == 't'){ //--Выводим тарифы
          stat = Status::tarifs;
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
  } else if (stat == Status::tarifs){
    printTarifs(&stCF);
  }


  return 0;
}
