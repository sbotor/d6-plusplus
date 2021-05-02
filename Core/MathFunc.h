#ifndef MATHFUNC_H
#define MATHFUNC_H

#include <string>
#include <vector>
#include <map>

#include "..\Errors\InputError.h"
#include "..\Errors\FuncError.h"
#include "..\Errors\ErrorClass.h"

class MathFunc : public ErrorClass
{
private:
	std::string func_name;
	const static std::map<std::string, double (*)(const std::vector<double>&, Error*&)> funcs;
	
	static std::vector<double> tokenise(const std::string& args);
	static size_t countArgs(const std::string& args);

	// *Obsolete*
	// Functions (string input):
	/*static double mf_floor(const std::string& args, Error* err);
	static double mf_ceil(const std::string& args, Error* err);
	static double mf_round(const std::string& args, Error* err);
	static double mf_max(const std::string& args, Error* err);
	static double mf_min(const std::string& args, Error* err);
	static double mf_neg(const std::string& args, Error* err);
	static double mf_sqrt(const std::string& args, Error* err);
	static double mf_avg(const std::string& args, Error* err);
	static double mf_abs(const std::string& args, Error* err);*/

	// Functions (vector input)
	static double mf_floor(const std::vector<double>& args, Error*& err);
	static double mf_ceil(const std::vector<double>& args, Error*& err);
	static double mf_round(const std::vector<double>& args, Error*& err);
	static double mf_max(const std::vector<double>& args, Error*& err);
	static double mf_min(const std::vector<double>& args, Error*& err);
	static double mf_neg(const std::vector<double>& args, Error*& err);
	static double mf_sqrt(const std::vector<double>& args, Error*& err);
	static double mf_avg(const std::vector<double>& args, Error*& err);
	static double mf_abs(const std::vector<double>& args, Error*& err);
	static double mf_sin(const std::vector<double>& args, Error*& err);
	static double mf_cos(const std::vector<double>& args, Error*& err);
	static double mf_tan(const std::vector<double>& args, Error*& err);

public:
	MathFunc(const std::string& name = "");

	//double operator()(const std::string& name, const std::string& args);
	double operator()(const std::string& name, const std::vector<double>& args);
	//double operator()(const std::string& args);
	double operator()(const std::vector<double>& args);

	static bool exists(const std::string& name);

};

#endif //!MATHFUNC_H
