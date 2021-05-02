#ifndef COMMHIST_H
#define COMMHIST_H

#include <string>

#include "HistoryQueue.h"

class CommHist
{
private:
	HistoryQueue<std::string> commands;
	HistoryQueue<std::string>::const_iterator currIt;
	bool start;

	static const int maxN;

public:
	CommHist();

	void add(const std::string& comm);
	std::string read() const;
	void next();
	void prev();
	void reset();
	void set();
	bool empty() const;
	bool isReset() const;
};

#endif // !COMMHIST_H