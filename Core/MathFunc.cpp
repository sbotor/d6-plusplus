#include <sstream>
#include <algorithm>
#include <cmath>
#include <numeric>

#include "MathFunc.h"
#include "..\Utility\UtilityFunctions.h"

const std::map<std::string, double (*)(const std::vector<double>&, Error*&)> MathFunc::funcs
{
	{"floor", &mf_floor}, {"ceil", &mf_ceil}, {"round", &mf_round},
	{"max", &mf_max}, {"min", &mf_min}, {"neg", &mf_neg},
	{"sqrt", &mf_sqrt}, {"avg", &mf_avg}, {"abs", &mf_abs},
	{"sin", &mf_sin}, {"cos", &mf_cos}, {"tan", &mf_tan}
};

MathFunc::MathFunc(const std::string& name) : ErrorClass(nullptr), func_name(name){}

//double MathFunc::operator()(const std::string& name, const std::string& args)
//{
//	func_name = name;
//	return operator()(args);
//}

double MathFunc::operator()(const std::string& name, const std::vector<double>& args)
{
	func_name = name;
	return operator()(args);
}

//double MathFunc::operator()(const std::string& args)
//{
//	deleteError(); // Remove previous error
//	std::map<std::string, double (*)(const std::vector<double>&, Error*)>::const_iterator f = funcs.find(func_name); // Find the function
//
//	if (f == funcs.end())
//	{
//		errP = new InputError("Function does not exist.", func_name);
//		return 0.0;
//	}
//
//	return f->second(tokenise(args), errP); // Call the function
//}

double MathFunc::operator()(const std::vector<double>& args)
{
	auto f = funcs.find(func_name); // Find the function

	if (f == funcs.end())
	{
		deleteError();
		errP = new InputError("Function does not exist", func_name);
		return 0.0;
	}

	return f->second(args, errP); // Call the function
}

bool MathFunc::exists(const std::string& name)
{
	auto found = funcs.find(name);

    if (found != funcs.end())
		return true;
	else
		return false;
}

std::vector<double> MathFunc::tokenise(const std::string& args)
{
	std::stringstream ss(args);
	std::string s;
	std::vector<double> argvect;

	while (std::getline(ss, s, ' '))
		argvect.push_back(std::stod(s));

	return argvect;
}

double MathFunc::mf_max(const std::vector<double>& args, Error*& err)
{	
	size_t argNum = args.size();
	if (argNum < 2)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "max", to_clean_string(argNum), "2+");
		return 0.0;
	}

	auto result_it = std::max_element(args.begin(), args.end());

	return *result_it;
}

double MathFunc::mf_min(const std::vector<double>& args, Error*& err)
{
	size_t argNum = args.size();
	if (argNum < 2)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "min", to_clean_string(argNum), "2+");
		return 0.0;
	}

	auto result_it = std::min_element(args.begin(), args.end());

	return *result_it;
}

double MathFunc::mf_neg(const std::vector<double>& args, Error*& err)
{
	size_t argNum = args.size();
	if (argNum != 1)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "neg", to_clean_string(argNum), "1");
		return 0.0;
	}
	
	return -1.0 * args[0];
}

double MathFunc::mf_sqrt(const std::vector<double>& args, Error*& err)
{
	size_t argNum = args.size();
	if (argNum != 1)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "sqrt", to_clean_string(argNum), "1");
		return 0.0;
	}

	return sqrt(args[0]);
}

double MathFunc::mf_avg(const std::vector<double>& args, Error*& err)
{
	size_t argNum = args.size();
	if (argNum < 1)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "avg", to_clean_string(argNum), "1+");
		return 0.0;
	}

    return std::accumulate(args.begin(), args.end(), 0.0) / static_cast<double>(argNum);
}

double MathFunc::mf_abs(const std::vector<double>& args, Error*& err)
{
	size_t argNum = args.size();
	if (argNum != 1)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "abs", to_clean_string(argNum), "1");
		return 0.0;
	}

	double x = args[0];
	return x >= 0 ? x : -1.0 * x;
}

double MathFunc::mf_sin(const std::vector<double>& args, Error*& err)
{
	size_t argNum = args.size();
	if (argNum != 1)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "sin", to_clean_string(argNum), "1");
		return 0.0;
	}

	return std::sin(args[0]);
}

double MathFunc::mf_cos(const std::vector<double>& args, Error*& err)
{
	size_t argNum = args.size();
	if (argNum != 1)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "cos", to_clean_string(argNum), "1");
		return 0.0;
	}

	return std::cos(args[0]);
}

double MathFunc::mf_tan(const std::vector<double>& args, Error*& err)
{
	size_t argNum = args.size();
	if (argNum != 1)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "tan", to_clean_string(argNum), "1");
		return 0.0;
	}

	return std::tan(args[0]);
}

double MathFunc::mf_floor(const std::vector<double>& args, Error*& err)
{
	size_t argNum = args.size();
	if (argNum != 1)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "floor", to_clean_string(argNum), "1");
		return 0.0;
	}
	
	return floor(args[0]);
}

double MathFunc::mf_ceil(const std::vector<double>& args, Error*& err)
{
	size_t argNum = args.size();
	if (argNum != 1)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "ceil", to_clean_string(argNum), "1");
		return 0.0;
	}
	
	return ceil(args[0]);
}

double MathFunc::mf_round(const std::vector<double>& args, Error*& err)
{
	size_t argNum = args.size();
	if (argNum != 1)
	{
		deleteError(err);
		err = new FuncError("Invalid argument number", "round", to_clean_string(argNum), "1");
		return 0.0;
	}
	
	return round(args[0]);
}

size_t MathFunc::countArgs(const std::string& args)
{
	std::stringstream ss(args);
	std::string s;
	size_t c = 0;
	std::vector<double> argv;

	while (std::getline(ss, s, ' '))
		c++;

	return c;
}
