#ifndef DIE_H
#define DIE_H

#include <string>
#include <iostream>
#include <random>

class Die
{
private:
	unsigned int sides;
	bool kept;
	unsigned int result;
	static std::random_device rd;
	static std::mt19937 gen;

public:
	// Symbol definitons:
	static constexpr char expl_char = '!';
	static constexpr char drop_char = '#';
	
	Die(const int ns = 6);

	bool operator<(const Die& d) const;
	bool operator>(const Die& d) const;
	friend std::ostream& operator<<(std::ostream& os, const Die& d);

	int getResult() const;
	bool isKept() const;
	int rollDie();
	void keepDie(const bool& b);
};

#endif // !DIE_H
