#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <stdio.h>
#include <sqlite3.h>
#include "global.h"

typedef struct{
    int begin;
    int end;
    float tarif;
} RowLog;

void printErr(const char* msg);
void printHelp();
void printVersion();
void printCounters(stCountersFormat *stCF);
void printTarifs(stCountersFormat *stCF);
void insUpdLog(stCountersFormat *stCF);
bool isExistRowLog(sqlite3 *db, int id, time_t unixdate);
RowLog getIndicationRow(sqlite3 *db, int id_counter, time_t unixdate);

#endif //_FUNCTIONS_H
