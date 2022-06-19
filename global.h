#ifndef _LIST_H
#define _LIST_H
#include <string>
#include <time.h>

enum class Status{
  error,
  help,
  version,
  counters,
  tarifsRead,
  tarifWrite,
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
} stCountersFormat;

namespace global{
  extern std::string currentPath;
  extern const char *monthName[];
}

#endif //_LIST_H
