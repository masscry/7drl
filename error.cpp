#include "error.h"
#include <execinfo.h>
#include <cstdio>
#include <cstdlib>

static FILE* errFile = 0;

static void errorInit(){
  if (errFile == 0){
    errFile = fopen("error.log", "w");
    if (errFile == 0){
      errFile = stderr;
    }
  }
}

static void errorCleanup(){
  if ((errFile != 0)&&(errFile != stderr)){
    fclose(errFile);
  }
  errFile = 0;
}

void errorExpect(bool cond, const char* condt,  const char* file, int line){
  if (!cond){
    void* stk[10];
    int nptr = backtrace(stk, 10);

    errorInit();
    fprintf(errFile, "Expected %s\n", condt);
    fflush(errFile);
    backtrace_symbols_fd(stk, nptr, fileno(errFile));
    errorCleanup();

    exit(-1);
  }
}
