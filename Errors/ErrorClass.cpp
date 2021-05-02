#include "ErrorClass.h"

ErrorClass::ErrorClass(Error* p) : errP(p) {}

ErrorClass::~ErrorClass()
{
	deleteError();
}

Error* ErrorClass::getError() const
{
	return errP;
}

Error* ErrorClass::captureError()
{
	Error* p = errP;
	errP = nullptr;
	return p;
}

void ErrorClass::deleteError()
{
	if (errP)
	{
		delete errP;
		errP = nullptr;
	}
}

void ErrorClass::deleteError(Error*& err)
{
	if (err)
	{
		delete err;
		err = nullptr;
	}
}
