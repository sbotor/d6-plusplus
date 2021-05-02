#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H

#include <string>

std::string to_clean_string(const double& n);

std::string to_clean_string(std::string n);

std::string str_lower(std::string name);

std::string trim(std::string data);

bool isInteger(const double& d);

#endif // !UTILITYFUNCTIONS_H
