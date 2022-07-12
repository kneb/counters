#ifndef _LIST_H
#define _LIST_H
#include <string>
#include <time.h>

enum class Status{
  error,
  help,
  version,
  counters,
  tarifs,
  updateLog,
};

enum class Indication{
  unset,
  begin,
  end,
  amount,
  all,
};

typedef struct{
  bool extended;
  int id_counter;
  int monthCount;
  struct tm data;
  bool dataSet;
  float tarif;
  int beginInd;
  int endInd;
  int amount;
  Indication setIndication;
} stCountersFormat;

namespace global{
  extern std::string currentPath;
  extern const char *monthName_i[];
  extern const char *monthName_v[];
  time_t decMonth(struct tm *date, int count=1);
  std::string getCurPath();
  int getDayMonth(int month, int year);
}

#endif //_LIST_H
