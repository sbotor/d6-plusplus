#include "InputError.h"

InputError::InputError(const std::string& msg, const std::string& token) : Error(msg), _token(token) {}

void InputError::prompt(QWidget* parent)
{
	// Construct the message
	QString message = "Input error: " + QString::fromStdString(_msg);
	if (!_token.empty())
		message.append("\nToken: \"" + QString::fromStdString(_token) + "\".");
	
	QMessageBox::warning(parent, "A problem", message);
}

std::string InputError::what()
{
	return _msg + ": " + "\"" + _token + "\"";
}
