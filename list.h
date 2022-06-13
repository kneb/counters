#include <sqlite3.h>

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