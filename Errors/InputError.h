#ifndef INPUTERROR_H
#define INPUTERROR_H

#include "Error.h"

class InputError : public Error
{
private:
	std::string _token;

public:
	InputError(const std::string& msg, const std::string& token);
	
	void prompt(QWidget* parent);
	std::string what();
};

#endif // !INPUTERROR_H