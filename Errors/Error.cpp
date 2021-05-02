#include "Error.h"

Error::Error(const std::string& msg) : _msg(msg) {}

Error::~Error() {}

void Error::prompt(QWidget* parent)
{
	// Construct the message
	QString message = "An error occured: " + QString::fromStdString(_msg);
	QMessageBox::warning(parent, "A problem", message);
}

std::string Error::what()
{
	return _msg;
}
