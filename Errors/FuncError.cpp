#include "FuncError.h"

FuncError::FuncError(const std::string& msg, const std::string& funcName,
	const std::string& argNum, const std::string& expectedNum) :
	Error(msg), _funcName(funcName), _argNum(argNum), _expectedNum(expectedNum) {}

void FuncError::prompt(QWidget* parent)
{
	// Construct the message
	QString message = "Function problem: " + QString::fromStdString(_msg) +
		"\nIn function: \"" + QString::fromStdString(_funcName) +
		"\"\nProvided: " + QString::fromStdString(_argNum) +
		", expected: " + QString::fromStdString(_expectedNum);

	QMessageBox::warning(parent, "A problem", message);
}

std::string FuncError::what()
{
	return _msg + " in function " + _funcName +
		"\", arguments: " + _argNum + "/" + _expectedNum;
}
