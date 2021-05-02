#include <sstream>
#include <regex>

#include "Evaluator.h"
#include "MathFunc.h"
#include "..\Utility\UtilityFunctions.h"

Evaluator::Evaluator(const std::string& expr) : RPNTool(expr),
	funcRegex(R"(([A-Za-z]{3}[A-Za-z]*)(\d+))") {}

Error* Evaluator::operator()()
{
	return evaluate();
}

Error* Evaluator::operator()(const std::string& expr)
{
	
	return evaluate(expr);
}

void Evaluator::clear()
{
	numbers.clear();
	numbers.shrink_to_fit();
	result = 0;
}

double Evaluator::getResult() const
{
	return result;
}

std::string Evaluator::getResultStr() const
{
	return to_clean_string(result);
}

Error* Evaluator::evaluate()
{
	std::stringstream inpStream(_expression);
	std::string currToken;
	std::smatch funcMatches;

	while (getline(inpStream, currToken, ' '))
	{
		if (std::regex_match(currToken, numRegex)) // Number
			numbers.push_back(std::stod(currToken));
		else if (currToken.size() == 1 ) // Operator
		{
			char c = currToken[0];
			double x, n1, n2;
			switch (c)
			{
			case '+': case '-': case '*': case '/': case '^': case '%': // Regular operators
				n2 = numbers.back();
				numbers.pop_back();
				n1 = numbers.back();
				numbers.pop_back();
				
				switch (c)
				{
				case '+':
					x = n1 + n2;
					break;
				case '-':
					x = n1 - n2;
					break;
				case '*':
					x = n1 * n2;
					break;
				case '/':
					if (n2 == 0)
					{
						errP = new Error("Sorry, division by zero is sadly impossible (" +
							to_clean_string(n1) + " / " +
							to_clean_string(n2) + ")");
						clear();
						return errP;
					}
					x = n1 / n2;
					break;
				case '^':
					x = std::pow(n1, n2);
					break;
				case '%':
					if (isInteger(n1) && isInteger(n2))
					{
						x = static_cast<int>(n1) % static_cast<int>(n2);
					}
					else
					{
						errP = new Error("Using modulo operator on non-integer values (" +
							to_clean_string(n1) + " % " +
							to_clean_string(n2) + ")");
						clear();
						return errP;
					}
					break;
				}
				break;
			
			case 'n': // Negation
				x = -1.0 * numbers.back();
				numbers.pop_back();
				break;

			default:
				deleteError();
				errP = new InputError("Invalid token during evaluation", currToken);
				clear();
				return errP;
				break;
			}

			numbers.push_back(x);
		}
		else if (std::regex_match(currToken, funcMatches, funcRegex)) // Function
		{
			std::string funcName = funcMatches[1];
			size_t funcArgN = std::stoi(funcMatches[2]);

			if (isFunc(funcName))
			{
				MathFunc f(funcName);
				std::vector<double> funcArgs;
				for (size_t i = 0; i < funcArgN; i++) // Fetch arguments
				{
					funcArgs.push_back(numbers.back());
					numbers.pop_back();
				}

				double funcResult = f(funcArgs);
				if (f.getError()) // Check for errors
				{
					errP = f.captureError();
					clear();
					return errP;
				}
				numbers.push_back(funcResult); // Add the function result
			}
			else
			{
				deleteError();
				errP = new InputError("Invalid function during evaluation", currToken);
				clear();
				return errP;
			}
		}
		else
		{
			deleteError();
			errP = new InputError("Invalid token during evaluation", currToken);
			clear();
			return errP;
		}
		
		numbers.shrink_to_fit();
	}

	result = numbers[0];

	return errP;
}

Error* Evaluator::evaluate(const std::string& expr)
{
	while (!numbers.empty())
		numbers.pop_back();
	
	clear();
	_expression = expr;

	return evaluate();
}
