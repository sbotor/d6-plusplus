#ifndef ERRORCLASS_H
#define ERRORCLASS_H

#include "Error.h"

class ErrorClass
{
protected:
	Error* errP;

public:
	ErrorClass(Error* p = nullptr);
	~ErrorClass();

	Error* getError() const;
	Error* captureError();
	void deleteError();
	static void deleteError(Error*& err);
};

#endif // !ERRORCLASS_H