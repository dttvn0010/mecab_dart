//  MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation

//  Flutter Dart binding by Tung Duong - 2020

#include "mecab.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern "C" __attribute__((visibility("default"))) __attribute__((used))
void* initMecab(const char* opt, const char* dicdir) {
    std::string rcfile = std::string(dicdir) + "/mecabrc";
    FILE *f = fopen(rcfile.data(), "rt");
    if(!f) return NULL;

    std::vector<char*> params;
    
    params.push_back(strdup("mecab"));

    std::string rcfileString = "--rcfile=" + rcfile;
    char* rcfileArg = (char*) malloc(rcfileString.length() + 1);
    strcpy(rcfileArg, rcfileString.data());
    params.push_back(rcfileArg);

    std::string dicdirString = "--dicdir=" + std::string(dicdir);
    char* dicdirArg = (char*) malloc(dicdirString.length() + 1);
    strcpy(dicdirArg, dicdirString.data());
    params.push_back(dicdirArg);

    if(*opt) {
        params.push_back(strdup(opt));
    }

    mecab_t* mecab = mecab_new(params.size(), params.data());

    for(size_t i = 0; i < params.size(); i++) {
        free(params[i]);
    }

    return mecab;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
const char* parse(void* mecab, const char* input) { 
    if(!mecab) return ""; 
    return mecab_sparse_tostr((mecab_t*)mecab, input);
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
void destroyMecab(void* mecab) {
    if(mecab) {
        mecab_destroy((mecab_t*) mecab);  
    }
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
int32_t native_add(int32_t x, int32_t y) {
    return x + y;
}
