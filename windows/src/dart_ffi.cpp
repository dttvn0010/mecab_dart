//  MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation

//  Flutter Dart binding by Tung Duong - 2020

#include "mecab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dart_ffi.h"

std::vector<char*> split(std::string st) {
    std::vector<char*> result;
    size_t start = 0;
    while(start < st.length()) {
        while(st[start] == ' ') start ++;
        size_t end = start + 1;
        while(end < st.length() && st[end] != ' ') end ++;
        char* token = (char*) malloc(end - start + 1);
        strncpy(token, st.data() + start, end - start);
        token[end - start] = 0;
	    result.push_back(token);
        start = end + 1;
    }
    return result;
}

void* initMecab(const char* opt, const char* dicdir) {
    std::string rcfile = std::string(dicdir) + "/mecabrc";
    FILE *f = fopen(rcfile.data(), "rt");
    if(!f) return NULL;

    std::string options = (std::string) "mecab --rcfile=" + (std::string) rcfile 
                                         + " --dicdir=" + dicdir;
    if(*opt) {
        options += " " + std::string(opt);
    }                       

    std::vector<char*> params = split(options);
    mecab_t* mecab = mecab_new(params.size(), params.data());

    for(size_t i = 0; i < params.size(); i++) {
        free(params[i]);
    }

    return mecab;
}

const char* parse(void* mecab, const char* input) { 
    if(!mecab) return ""; 
    return mecab_sparse_tostr((mecab_t*)mecab, input);
}

void destroyMecab(void* mecab) {
    if(mecab) {
        mecab_destroy((mecab_t*) mecab);  
    }
}

int native_add(int x, int y) {
    return x + y;
}
