#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <string>
#include <vector>

#include "RPNTool.h"

class Evaluator : public RPNTool
{
private:
	std::vector<double> numbers;
	double result;
	const std::regex funcRegex;

public:
	Evaluator(const std::string& expr = "");

	Error* operator()();
	Error* operator()(const std::string& expr);

	void clear();
	double getResult() const;
	std::string getResultStr() const;
	Error* evaluate();
	Error* evaluate(const std::string& expr);
};

#endif // !EVALUATOR_H
