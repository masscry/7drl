#include <time.h>


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
