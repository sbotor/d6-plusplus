#ifndef FUNCERROR_H
#define FUNCERROR_H

#include "Error.h"

class FuncError : public Error
{
private:
	std::string _funcName, _argNum, _expectedNum;

public:
	FuncError(const std::string& msg, const std::string& funcName, const std::string& argNum, const std::string& expectedNum);

	void prompt(QWidget* parent);
	std::string what();
};

#endif // !FUNCERROR_H