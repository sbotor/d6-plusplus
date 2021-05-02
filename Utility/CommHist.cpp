#include "CommHist.h"

const int CommHist::maxN = 15;

void CommHist::reset()
{
	currIt = commands.cbegin();
	start = true;
}

void CommHist::set()
{
	start = false;
}

bool CommHist::empty() const
{
	return commands.empty();
}

bool CommHist::isReset() const
{
	return start;
}

CommHist::CommHist() : commands(), currIt(nullptr, nullptr), start(false) {}

void CommHist::add(const std::string& comm)
{
	if (commands.size() == maxN)
		commands.pop_back();

	commands.push_front(comm);
	reset();
}

std::string CommHist::read() const
{
	return *currIt;
}

void CommHist::next()
{
	set();
	if (currIt != commands.cend() && currIt != --(commands.cend()))
		++currIt;
}

void CommHist::prev()
{
	set();
	if (currIt != commands.cend() && currIt != commands.cbegin())
		--currIt;
}
