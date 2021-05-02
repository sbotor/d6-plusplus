#include <regex>
#include <sstream>
#include <iostream>
#include <cmath>

#include "Converter.h"
#include "..\Utility\UtilityFunctions.h"

//const std::regex funcRegex(R"([A-Za-z]{3}[A-Za-z]*)");

Converter::Converter(const std::string& expr) : RPNTool(expr), prevToken(TokenType::start),
	funcRegex(R"([A-Za-z]{3}[A-Za-z]*)") {}

Error* Converter::operator()()
{
	return convert();
}

Error* Converter::operator()(const std::string& expr)
{
	return convert(expr);
}

void Converter::clear()
{
	data.clear();
	prettyExpression.clear();
	converted.clear();
	opStack.clear();
	while (!argC.empty())
		argC.pop();
	output.clear();
	output.shrink_to_fit();
	details.clear();
	details.shrink_to_fit();
}

int Converter::prec(const char& c)
{
	switch (c)
	{
	case '+': case '-':
		return 0;
		break;
	case '*': case '/': case '%':
		return 1;
		break;
	case '^':
		return 2;
		break;
	case 'n':
		return 3;
		break;
    default:
        return -1;
        break;
	}
}

bool Converter::isLeftAssoc(const char& c)
{
	switch (c)
	{
	case '+': case '-': case '*': case '/': case '%':
		return true;
		break;
	case '^': case 'n':
		return false;
		break;
     default:
        return false;
        break;
	}
}

std::string Converter::getData() const { return data; }

std::string Converter::getPrettyExpr() const
{
	return prettyExpression;
}

std::string Converter::getConverted() const
{	
	return converted;
}

std::string Converter::getDetails() const
{
	std::string s;

	for (std::vector<std::string>::const_iterator it = details.begin(); it != details.end(); it++)
		s.append(*it);

	return s;
}

void Converter::normalise()
{
	// This is a very bad piece of code but will make do
	data = std::regex_replace(_expression, std::regex(R"(\s+)"), ""); // Delete all spaces
	data = std::regex_replace(data, std::regex(R"([\(\)\+\-\*/,\^%])"), " $& "); // Divide into tokens by ()+-*/^%,
	data = std::regex_replace(data, std::regex(R"(\s+)"), " "); // Delete all multi spaces
	data = std::regex_replace(data, std::regex(R"((\s+)([dD]\d+))"), "$011$02"); // Add 1 to a die if it was omitted
	data = std::regex_replace(data, std::regex(R"(^([dD]\d+))"), "1$01"); // Add 1 to a die if it was omitted at the beginning of the expression

	// Trim the data
	data = trim(data);
}

void Converter::popOut()
{
	if (!opStack.empty())
	{
		if (isFunc(opStack.front()))
		{
			if (!argC.empty())
			{
				opStack.front().append(to_clean_string(argC.top())); // Add the number of arguments to the name
				argC.pop();
			}
			else
			{
				deleteError();
				errP = new InputError("Unexpected function", opStack.front());
				return;
			}
		}
		
		output.push_back(opStack.front());
		opStack.pop_front();
	}
}

void Converter::popArg()
{
	while (!opStack.empty())
	{
		std::deque<std::string>::iterator it = opStack.begin();
		if ((*it) == "(" && ++it != opStack.end() && isFunc(*it))
			break;
		else
		{
			popOut();
			if (errP)
				return;
		}
	}
}

void Converter::addOperator(const char& c)
{
	while (!opStack.empty() && opStack.front() != "(" &&
		prec(opStack.front()[0]) >= prec(c) && isLeftAssoc(c))
		popOut();

	opStack.push_front(std::string(1, c));
}

void Converter::addFunc(const std::string& name)
{
	opStack.push_front(name);
	argC.push(1);
}

std::string Converter::addDie(const std::string& d)
{
	Roller r(d);
	std::string result_str = to_clean_string(r());
	if (r.getError())
	{
		deleteError();
		errP = r.captureError();
		return "";
	}

	output.push_back(result_str);

	return r.getDetails();
}

void Converter::popParen()
{
	while (!opStack.empty() && opStack.front() != "(")
	{
		popOut();
		if (errP)
			return;
	}
	if (opStack.empty())
	{
		deleteError();
		errP = new InputError("Mismatched parentheses", "");
		return;
	}

	if (!opStack.empty() && opStack.front() == "(") // Pop the '(' out
		opStack.pop_front();
	if (!opStack.empty() && opStack.front().size() >= 3) // Pop the function out
	{
		popOut();
		if (errP)
			return;
	}
}

void Converter::outputToStr()
{
	converted = "";
	for (size_t i = 0; i < output.size(); i++)
	{
		converted.append(output[i]);
		if (i != output.size() - 1)
			converted.append(" ");
	}
}

void Converter::addPretty(const std::string& token)
{
	if (prevToken == TokenType::num || prevToken == TokenType::oper ||
		prevToken == TokenType::r_par)
		prettyExpression.append(" ");

	if (std::regex_match(token, numRegex))
		prettyExpression.append(to_clean_string(token));
	else
		prettyExpression.append(token);
}

bool Converter::expected(const TokenType& token)
{
	switch (token)
	{
	// A number or a function
	case TokenType::num: case TokenType::func:
		if (prevToken == TokenType::start || prevToken == TokenType::oper ||
			prevToken == TokenType::l_par || prevToken == TokenType::neg)
			return true;
		break;

	// A binary operator
	case TokenType::oper:
		if (prevToken == TokenType::num || prevToken == TokenType::r_par)
			return true;
		break;

	// Negation
	case TokenType::neg:
		if (prevToken == TokenType::start || prevToken == TokenType::l_par)
			return true;
		break;

	// Left parenthesis
	case TokenType::l_par:
		if (prevToken == TokenType::start || prevToken == TokenType::func ||
			prevToken == TokenType::oper || prevToken == TokenType::neg)
			return true;
		break;

	// Right parenthesis
	case TokenType::r_par:
		if (prevToken == TokenType::num || prevToken == TokenType::r_par)
			return true;
		break;

	// Comma
	case TokenType::com:
		if (prevToken == TokenType::num || prevToken == TokenType::r_par)
			return true;
		break;

	default:
		return false;
		break;
	}

	return false;
}

Error* Converter::convert()
{	
	normalise();

	std::stringstream inpStream(data);
	std::string currToken;

	while (getline(inpStream, currToken, ' '))
	{
		addPretty(currToken);
		
		/*
			The token is a number
		*/
		if (std::regex_match(currToken, numRegex))
		{
			if (expected(TokenType::num)) // Check if number expected
			{
				output.push_back(to_clean_string(currToken));
				details.push_back(to_clean_string(currToken));
				prevToken = TokenType::num;
			}
			else
			{
				deleteError();
				errP = new InputError("Unexpected token", currToken);
				clear();
				return errP;
			}
		}

		/*
			The token can be an operator or e
		*/
		else if (currToken.length() == 1)
		{
			char c = currToken[0];
			switch (c)
			{
			case '+': case '*': case '/': case '^': case '%':
				if (expected(TokenType::oper)) // Check if expected
				{
					addOperator(c);
					details.push_back(" " + currToken + " ");
					prevToken = TokenType::oper;
				}
				else
				{
					deleteError();
					errP = new InputError("Unexpected token", currToken);
					clear();
					return errP;
				}
				break;
			
			case '-':
				if (expected(TokenType::oper)) // Regular operator
				{
					addOperator(c);
					details.push_back(" " + currToken + " ");
					prevToken = TokenType::oper;
				}
				else if (expected(TokenType::neg)) // Unary negation
				{
					addOperator('n');
					details.push_back("-");
					prevToken = TokenType::neg;
				}
				else
				{
					deleteError();
					errP = new InputError("Unexpected token", currToken);
					clear();
					return errP;
				}
				break;
			
			case '(':
				if (expected(TokenType::l_par)) // Check if expected
				{
					opStack.push_front(std::string(1, c));
					details.push_back(currToken);
					prevToken = TokenType::l_par;
				}
				else
				{
					deleteError();
					errP = new InputError("Unexpected token", currToken);
					clear();
					return errP;
				}
				break;
			
			case ')':
				if (expected(TokenType::r_par))
				{
					popParen();
					details.push_back(currToken);
					prevToken = TokenType::r_par;
				}
				else
				{
					errP = new InputError("Unexpected token", currToken);
					clear();
					return errP;
				}
				break;
			
			case ',':
				if (expected(TokenType::com)) // Check if expected
				{
					popArg();
					if (errP)
					{
						clear();
						return errP;
					}
					if (!argC.empty())
						argC.top()++;
					else
					{
						deleteError();
						errP = new InputError("Unexpected comma", "");
						clear();
						return errP;
					}
					details.push_back(currToken + " ");
					prevToken = TokenType::com;
				}
				else
				{
					deleteError();
					errP = new InputError("Unexpected comma", "");
					clear();
					return errP;
				}
				break;
			
			case 'e': case 'E':
				if (expected(TokenType::num)) // Check if number expected
				{
					output.push_back(to_clean_string(M_E));
					details.push_back(to_clean_string(M_E));
					prevToken = TokenType::num;
				}
				else
				{
					deleteError();
					errP = new InputError("Unexpected token", currToken);
					clear();
					return errP;
				}
				break;
			
			default:
				deleteError();
				errP = new InputError("Unrecognised token", currToken);
				clear();
				return errP;
				break;
			}
		}
		
		/*
			The token is a dice notation
		*/
		else if (std::regex_match(currToken, dieRegex))
		{
			if (expected(TokenType::num)) // Check if expected
			{
				std::string rollDetails = addDie(currToken);
				if (errP) // Check for dice errors
				{
					clear();
					return errP;
				}
				details.push_back(rollDetails);
				prevToken = TokenType::num;
			}
			else
			{
				deleteError();
				errP = new InputError("Unexpected token", currToken);
				clear();
				return errP;
			}
		}
		
		/*
			The token is a function (min. 3 alpha characters)
		*/
		else if (std::regex_match(currToken, funcRegex) && isFunc(str_lower(currToken)))
		{
			if (expected(TokenType::func)) // Check if expected
			{
				addFunc(str_lower(currToken));
				details.push_back(str_lower(currToken));
				prevToken = TokenType::func;
			}
			else
			{
				deleteError();
				errP = new InputError("Unexpected token", currToken);
				clear();
				return errP;
			}
		}
		
		/*
			The token is pi
		*/
		else if (std::regex_match(currToken, std::regex(R"([pP][iI])")))
		{
			if (expected(TokenType::num)) // Check if number expected
			{
				output.push_back(to_clean_string(M_PI));
				details.push_back(to_clean_string(M_PI));
				prevToken = TokenType::num;
			}
		else
		{
			deleteError();
			errP = new InputError("Unexpected token", currToken);
			clear();
			return errP;
		}
		}
		else
		{
		deleteError();
		errP = new InputError("Unexpected token", currToken);
			clear();
			return errP;
		}
	}

	// Check if the last token was a number or right parenthesis
	if (prevToken != TokenType::num && prevToken != TokenType::r_par)
	{
		deleteError();
		errP = new InputError("Unexpected token", currToken);
		clear();
		return errP;
	}

	while (!opStack.empty())
	{
		if (opStack.front() == "(" || opStack.front() == ")")
		{
			deleteError();
			errP = new InputError("Mismatched parentheses", "");
			clear();
			return errP;
		}
		popOut();
		if (errP)
		{
			clear();
			return errP;
		}
	}
	output.shrink_to_fit();
	outputToStr();

	return errP;
}

Error* Converter::convert(const std::string& expr)
{
	clear();

	data = expr;
	return convert();
}

std::ostream& operator<<(std::ostream& os, const Converter& c)
{
	os << "Data: " << c.prettyExpression << "\n" <<
		"Converted: " << c.converted << "\n" <<
		"Details: " << c.getDetails();
	
	return os;
}
