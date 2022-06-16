#ifndef _LIST_H
#define _LIST_H

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
  int month;
} stCountersFormat;

extern const char *mon_name[];

#endif //_LIST_H
