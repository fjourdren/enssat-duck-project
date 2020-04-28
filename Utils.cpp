#include <string>
#include <fstream>
#include <vector>

#include "Utils.h"

std::string convertToString(char* a, int size) { 
    std::string s = "";

    for (int i = 0; i < size; i++) {
        s = s + a[i];
    }

    return s;
}


bool endWith(std::string fullString, std::string ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}


bool checkIfFileExists(std::string file) {
    std::ifstream ifexfile(file.c_str());
    if (ifexfile)
        return true;
    else
        return false;
}


// découpe d'un string avec un délimiteur (pour la désérialisation)
std::vector<std::string> split(std::string message, char delimiter) {
    std::vector<std::string> vout;
    unsigned int start = 0, end = 0;

    while(end < message.length()) {
        end = message.find(delimiter, start);
        vout.push_back(message.substr(start, end - start));
        start = end + 1;
    }

    vout.push_back(message.substr(start));

    return vout;
}