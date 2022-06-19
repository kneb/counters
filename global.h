#ifndef _LIST_H
#define _LIST_H
#include <string>

enum class Status{
  error,
  help,
  version,
  counters,
  tarifs,
};

enum class Counter{
  all,
  electro,
  gas,
  water,
};

typedef struct{
  bool extended;
  int id_counter;
  int monthCount;
} stCountersFormat;



namespace global{
  extern std::string currentPath;
  extern const char *monthName[];
}

#endif //_LIST_H
