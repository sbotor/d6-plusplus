#ifndef CONVERTER_H
#define CONVERTER_H

#include <stack>
#include <deque>
#include <string>
#include <vector>

//#include "MathFunc.h"
#include "RPNTool.h"
#include "..\Dice\Roller.h"

class Converter : public RPNTool
{
private:
	std::string data, prettyExpression, converted;
	std::deque<std::string> opStack;
	std::stack<int> argC;
	std::vector<std::string> output, details;
	enum class TokenType { start, num, oper, r_par, l_par, func, neg, com = start };
	TokenType prevToken;
	const std::regex funcRegex;

	void normalise();
	void popOut();
	void popArg();
	void addOperator(const char& c);
	void addFunc(const std::string& name);
	std::string addDie(const std::string& d);
	void popParen();
	void outputToStr();
	void addPretty(const std::string& token);
	bool expected(const TokenType& token);

public:
	Converter(const std::string& expr = "");
	
	Error* operator()();
	Error* operator()(const std::string& expr);
	friend std::ostream& operator<<(std::ostream& os, const Converter& c);

	void clear();
	int prec(const char& c);
	bool isLeftAssoc(const char& c);
	std::string getData() const;
	std::string getPrettyExpr() const;
	std::string getConverted() const;
	std::string getDetails() const;
	Error* convert();
	Error* convert(const std::string& expr);
};

#endif // !CONVERTER_H
