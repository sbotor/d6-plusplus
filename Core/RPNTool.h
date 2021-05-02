#ifndef RPNTOOL_H
#define RPNTOOL_H

#include <string>
#include <memory>
#include <regex>

#include "..\Errors\InputError.h"
#include "..\Errors\ErrorClass.h"

class RPNTool : public ErrorClass
{
protected:
	std::string _expression;
	const std::regex numRegex, dieRegex;

public:
	RPNTool(const std::string& expr);
    ~RPNTool();

	virtual Error* operator()() = 0;
	virtual Error* operator()(const std::string& expr) = 0;

	virtual void clear() = 0;
	static bool isFunc(const std::string& name);
};

#endif // !RPNTOOL_H
