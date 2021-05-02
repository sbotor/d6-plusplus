#ifndef ROLLER_H
#define ROLLER_H

#include <list>

#include "Die.h"
#include "..\Errors\InputError.h"
#include "..\Errors\ErrorClass.h"

class Roller : public ErrorClass
{
private:
	std::list<Die> _dice;
	std::string _rollExpression;
	int _sides, _startingDice, _kdNum, result;
	bool _exploding;
	char _keepDrop, _highLow;

	double sum();
	void keep();
	void drop();

protected:
	

public:
	//Roller(const double res = 6, const int n = 1, const int s = 6);
	Roller(const std::string& d);
	/*Roller(const int& startingDice = 1, const int& sides = 6,
		const bool& exploding = false, const char& keepDrop = '\0',
		const char& highLow = '\0', const int& kdNum = 1);*/

	int operator()();
	friend std::ostream& operator<<(std::ostream& os, Roller& r);

	int getResult() const;
	std::string getDetails() const;
	int roll();
};

#endif // !ROLLER_H
