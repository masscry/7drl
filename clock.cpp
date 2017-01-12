#include <time.h>

#ifndef _WIN32

timespec start;

void clockStart(){
  clock_gettime(CLOCK_MONOTONIC, &start);
}

double clockNow(){
  timespec temp;
  clock_gettime(CLOCK_MONOTONIC, &temp);
  if (temp.tv_nsec < start.tv_nsec){
    return ((double)(temp.tv_sec - start.tv_sec - 1)) + ((double)(temp.tv_nsec - start.tv_nsec + 1e9) * 1.0e-9);
  } else {
    return (temp.tv_sec - start.tv_sec) + (temp.tv_nsec - start.tv_nsec)*1.0e-9;
  }
}

#else // _WIN32

/**
 * @todo add WIN32 implementation
 */
void clockStart(){
  ;
}

/**
 * @todo add WIN32 implementation
 */
double clockNow(){
  return 0.0;
}

#endif // _WIN32
