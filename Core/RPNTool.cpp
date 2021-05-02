#include "RPNTool.h"
#include "MathFunc.h"

RPNTool::RPNTool(const std::string& expr) : ErrorClass(nullptr), _expression(expr),
    numRegex(R"(\d+(\.\d+)*)"), dieRegex(R"(\d*[dD]\d+!?([kd]([hl])?\d+)?)") {}

RPNTool::~RPNTool()
{
    if (errP)
    {
        delete errP;
        errP = nullptr;
    }
}

bool RPNTool::isFunc(const std::string& name)
{
	if (MathFunc::exists(name))
		return true;
	else
		return false;
}
