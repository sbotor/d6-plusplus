#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <iostream>

#include "..\Core\Evaluator.h"
#include "..\Core\Converter.h"

class History
{
private:
	std::string original_expr, details, result;

public:
    History(const std::string& res, const std::string& expr, const std::string& det);
	History(const std::string& inp);
    History(const Converter& conv, const Evaluator& ev);

	friend std::ostream& operator<<(std::ostream& os, const History& log);
	friend std::istream& operator>>(std::istream& is, History& log);
    void operator()(const Converter& conv, const Evaluator& ev);

    std::string getExpression() const;
    std::string getDetails() const;
    std::string getResult() const;
	void setExpression(const std::string& str);
	void setDetails(const std::string& str);
	void setResult(const std::string& str);
	void setResult(double num);
};

#endif // !HISTORY_H
