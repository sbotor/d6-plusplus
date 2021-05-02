#include <regex>
#include <random>

#include "Die.h"
#include "..\Utility\UtilityFunctions.h"

std::random_device Die::rd;

std::mt19937 Die::gen(rd());

Die::Die(const int ns) { sides = ns, kept = true; result = 0; }

bool Die::operator<(const Die& d) const
{
	return result < d.result;
}

bool Die::operator>(const Die& d) const
{
	return result > d.result;
}

std::ostream& operator<<(std::ostream& os, const Die& d)
{
	std::string outp = "";
	
	if (!d.kept) // Check if dropped die and add a 'dropped' character
		outp.append(std::string(1, Die::drop_char));
	
	outp.append(to_clean_string(d.result));

	os << outp;
	return os;
}

int Die::getResult() const
{
	return result;
}

bool Die::isKept() const
{
	return kept;
}

int Die::rollDie()
{
	if (sides == 1)
		result = 1;
	else
	{
		std::uniform_int_distribution<int> dist(1, sides);
		result = dist(gen);
	}

	return result;
}

void Die::keepDie(const bool& b)
{
	kept = b;
}
