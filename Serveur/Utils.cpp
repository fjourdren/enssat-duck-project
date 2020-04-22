#include <string>

#include "Utils.h"

std::string convertToString(char* a, int size) { 
    std::string s = "";

    for (int i = 0; i < size; i++) {
        s = s + a[i];
    }

    return s;
} 