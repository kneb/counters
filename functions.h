#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <stdio.h>
#include "global.h"

void printErr(const char* msg);
void printHelp();
void printVersion();
void printCounters(stCountersFormat *stCF);
std::string getCurPath();

#endif //_FUNCTIONS_H
