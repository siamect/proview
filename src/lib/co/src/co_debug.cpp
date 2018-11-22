#include "co_debug.h"

#include <stdarg.h>
#include <string.h>
#include <time.h>

int DEBUG = 0;

void print_time(FILE* stream, int fulldate)
{
  time_t t;
  struct tm* tm;
  char Date[11], Time[11];
  time(&t);
  tm = localtime(&t);
  if (fulldate) {
    strftime(Date, 11, "%Y-%m-%d", tm);
    fprintf(stream, "%s ", Date);
  }
  strftime(Time, 11, "%H:%M:%S", tm);
  fprintf(stream, "%s", Time);
}

void dbg_print(const char* file, int line, const char* fmt, ...)
{
  if (DEBUG) {
    // 1. print timestamp
    print_time(stderr);
    // 2. print filename only, without path
    const char* file2 = strrchr(file, '/');
    file2 = file2 ? (file2 + 1) : file;
    fprintf(stderr, " %s:%d: ", file2, line);
    // 3. print the actual debug message
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
  }
}