#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <stdio.h>
#include "global.h"

void printErr(const char* msg);
void printHelp();
void printVersion();
void printCounters(stCountersFormat *stCF);
std::string getCurPath();
void printTarifs(stCountersFormat *stCF);
void decMonth(struct tm *date);

#endif //_FUNCTIONS_H
