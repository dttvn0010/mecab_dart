#include "mecab.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

// Function to split options string into arguments, respecting quotes
std::vector<char*> parseOptions(const std::string& options) {
    std::vector<char*> result;
    std::string token;
    bool inQuotes = false;

    for (char ch : options) {
        if (ch == '"') {
            inQuotes = !inQuotes;
        } else if (ch == ' ' && !inQuotes) {
            if (!token.empty()) {
                char* arg = (char*)malloc(token.size() + 1);
                strcpy(arg, token.c_str());
                result.push_back(arg);
                token.clear();
            }
        } else {
            token += ch;
        }
    }
    if (!token.empty()) {
        char* arg = (char*)malloc(token.size() + 1);
        strcpy(arg, token.c_str());
        result.push_back(arg);
    }

    return result;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
void* initMecab(const char* opt, const char* dicdir) {
    std::string rcfile = std::string(dicdir) + "/mecabrc";
    FILE* f = fopen(rcfile.c_str(), "rt");
    if (!f) return NULL;
    fclose(f);

    std::string options = "mecab --rcfile=\"" + rcfile + "\" --dicdir=\"" + std::string(dicdir) + "\"";
    if (*opt) {
        options += " " + std::string(opt);
    }

    std::vector<char*> params = parseOptions(options);
    mecab_t* mecab = mecab_new(params.size(), params.data());

    for (size_t i = 0; i < params.size(); ++i) {
        free(params[i]);
    }

    return mecab;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
const char* parse(void* mecab, const char* input) {
    if (!mecab) return "";
    return mecab_sparse_tostr((mecab_t*)mecab, input);
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
void destroyMecab(void* mecab) {
    if (mecab) {
        mecab_destroy((mecab_t*)mecab);
    }
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
int32_t native_add(int32_t x, int32_t y) {
    return x + y;
}
