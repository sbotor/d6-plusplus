#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>
#include <fstream>

#include <QMessageBox>
#include <QString>

class Error
{
protected:
	std::string _msg;

public:
	Error(const std::string& msg);
    virtual ~Error();
	
	virtual void prompt(QWidget* parent);
	virtual std::string what();
};

#endif // !ERROR_H
