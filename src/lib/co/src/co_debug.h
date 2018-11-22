#ifndef CO_DEBUG
#define CO_DEBUG

#include <stdio.h>

void print_time(FILE* stream, int fulldate = 0);
void dbg_print(const char* file, int line, const char* fmt, ...);

#define debug_print(fmt, args...) dbg_print(__FILE__, __LINE__, fmt, ##args);

#endif