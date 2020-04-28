#ifndef DEF_UTILS
#define DEF_UTILS

#include <string>
#include <vector>

std::string convertToString(char* a, int size);
bool endWith(std::string fullString, std::string ending);
bool checkIfFileExists(std::string file);
std::vector<std::string> split(std::string message, char delimiter); // découpe d'un string avec un délimiteur (pour la désérialisation)  

#endif