#include "config.h"
#include <cstdlib>
#include <cstdio>
#include <unordered_map>
#include <string>

typedef std::unordered_map<std::string, std::string> configMap_t;

static configMap_t configMap;

void configInit(){
  FILE* input = fopen("config.txt", "r");
  if (input != 0){
    while((!feof(input))&&(!ferror(input))){
      char key[256];
      char value[256];
      if (fscanf(input, "%255s %255s", key, value) == 2){
        configMap[key] = value;
      }
    }
    fclose(input);
  }
}

void configCleanup(){
  ;
}

const char* configKey(const char* key){
  if (configMap.find(key) != configMap.end()){
    return configMap[key].c_str();
  }
  return 0;
}


