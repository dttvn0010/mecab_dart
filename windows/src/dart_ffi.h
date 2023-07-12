
#ifndef MECAB_DART_FFI_H_
#define MECAB_DART_FFI_H_

#include <vector>
#include <string>

extern "C"  //__attribute__((visibility("default"))) __attribute__((used))
{    
    void* initMecab(const char* opt, const char* dicdir);
    const char* parse(void* mecab, const char* input);
    void destroyMecab(void* mecab);
    int native_add(int x, int y);
}

#endif
