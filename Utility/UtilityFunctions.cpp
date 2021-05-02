#include <cmath>

#include "UtilityFunctions.h"

std::string to_clean_string(const double& n)
{
	std::string outp = std::to_string(n);

    outp.erase(0, outp.find_first_not_of('0'));
	if (outp.empty())
		outp = "0";
	else if (outp[0] == '.')
		outp = "0" + outp;

    outp.erase(outp.find_last_not_of('0') + 1);
	if (outp[outp.size() - 1] == '.')
		outp.erase(outp.size() - 1);

	if (outp[0] == '-' && outp[1] == '0')
		outp.erase(0, 1);

	return outp;
}

std::string to_clean_string(std::string n)
{
    double d = std::stod(n);

    return to_clean_string(d);
}

std::string str_lower(std::string name)
{
	for (std::string::iterator p = name.begin(); p < name.end(); p++) // Convert to lowercase
		*p = tolower(*p);

	return name;
}

std::string trim(std::string data)
{
	std::string whitespace = " \t\n";
	data.erase(0, data.find_first_not_of(whitespace));
	data.erase(data.find_last_not_of(whitespace) + 1);

	return data;
}

bool isInteger(const double& d)
{
	return d == std::floor(d) ? true : false;
}
