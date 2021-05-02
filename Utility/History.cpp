#include <sstream>
#include <algorithm>

#include "History.h"
#include "UtilityFunctions.h"

History::History(const std::string& res, const std::string& expr, const std::string& det)
{
	result = res;
	original_expr = expr;
	details = det;
}

History::History(const std::string& inp)
{
	std::stringstream inpStream(inp);
	std::string s;

	// Get result
	std::getline(inpStream, s, ';');
	s = trim(s);
    result = s;

	// Get the original expression
	std::getline(inpStream, s, ';');
	s = trim(s);
	original_expr = s;

	// Get the details
	std::getline(inpStream, s, ';');
	s = trim(s);
	details = s;
}

History::History(const Converter& conv, const Evaluator& ev)
{
    operator()(conv, ev);
}

std::ostream& operator<<(std::ostream& os, const History& log)
{
	os << log.result << "; " << log.original_expr <<
		"; " << log.details << std::endl;
	
	return os;
}

std::istream& operator>>(std::istream& is, History& log)
{
	std::string s;

	// Get result
	std::getline(is, s, ';');
	s = trim(s);
	log.result = s;

	// Get the original expression
	std::getline(is, s, ';');
	s = trim(s);
	log.original_expr = s;

	// Get the details
	std::getline(is, s, ';');
	s = trim(s);
	log.details = s;

	return is;
}

void History::operator()(const Converter& conv, const Evaluator& ev)
{
    result = ev.getResultStr();
    original_expr = conv.getPrettyExpr();
    details = conv.getDetails();
}

std::string History::getExpression() const
{
    return original_expr;
}

std::string History::getDetails() const
{
    return details;
}

std::string History::getResult() const
{
    return result;
}

void History::setExpression(const std::string& str)
{
	original_expr = str;
}

void History::setDetails(const std::string& str)
{
	details = str;
}

void History::setResult(const std::string& str)
{
	result = str;
}

void History::setResult(double num)
{
	setResult(to_clean_string(num));
}
