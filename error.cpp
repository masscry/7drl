#include "error.h"
#include <execinfo.h>
#include <cstdio>
#include <cstdlib>

static FILE* errFile = 0;

void errorInit(){
  if (errFile == 0){
    errFile = fopen("error.log", "w");
  }
}

void errorCleanup(){
  if (errFile != 0){
    fclose(errFile);
    errFile = 0;
  }
}

void errorExpect(bool cond, const char* condt,  const char* file, int line){
  if (!cond){
    if (errFile == 0){
      errorInit();
    }
    void* stk[10];
    int nptr = backtrace(stk, 10);

    fprintf(errFile, "Expected %s\n", condt);
    fflush(errFile);
    backtrace_symbols_fd(stk, nptr, fileno(errFile));
    errorCleanup();
    exit(-1);
  }
}
