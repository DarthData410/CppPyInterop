#include <iostream>
#include <string>

using namespace std;

class format{
public:
    string gray = "\e[0;100m";
    string graytxt = "\033[90m";
    string red =  "\e[41m";
    string redtxt = "\033[31m";
    string green = "\e[0;102m";
    string greentxt = "\033[32m";
    string gold = "\e[0;103m";
    string goldtxt = "\033[93m";
    string blue = "\e[0;104m";
    string dblue = "\e[44m";
    string bluetxt = "\033[34m";
    string purple = "\e[0;105m";
    string turq = "\e[0;106m";
    string white = "\e[0;107m";
    string none = "\e[0;0m";
};