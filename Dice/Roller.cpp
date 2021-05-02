#include <iostream>
#include <regex>
#include <set>
#include <sstream>

#include "Roller.h"
#include "..\Utility\UtilityFunctions.h"

Roller::Roller(const std::string& d) : ErrorClass(nullptr), _rollExpression(d)
{
	result = -1;
	std::smatch matches;
	if (!std::regex_match(d, matches,
		std::regex(R"((\d*)[dD](\d+)(!?)(([kd])([hl])?(\d+))?)"))) // Incorrect expression
	{
		deleteError();
		errP = new InputError("Incorrect die expression.", _rollExpression);
		return;
	}

	// Data extraction
	_startingDice = std::stoi(matches[1]); // Find the number of dice
	_sides = std::stoi(matches[2]); // Find the number of sides
	if (matches[3] == "!") // Check if exploding
		_exploding = true;
	else
		_exploding = false;
	if (!matches[4].str().empty()) // Check for the need to keep/drop
	{
		_keepDrop = matches[5].str()[0]; // Keep or drop
		if (matches[6].str().empty()) // Check for lowest/highest specifier
		{
			if (matches[5] == 'k')
				_highLow = 'h';
			else
				_highLow = 'l';
		}
		else
			_highLow = matches[6].str()[0];
		_kdNum = std::stoi(matches[7].str()); // Keep/drop how many
	}
	else
	{
		_keepDrop = '\0';
		_highLow = '\0';
		_kdNum = 0;
	}
}

//Roller::Roller(const int& startingDice, const int& sides,
//	const bool& exploding, const char& keepDrop,
//	const char& highLow, const int& kdNum) :
//	ErrorClass(nullptr),
//	_startingDice(startingDice),
//	_sides(sides),
//	_exploding(exploding),
//	_keepDrop(keepDrop),
//	_highLow(highLow),
//	_kdNum(kdNum),
//	result(0) {}

int Roller::operator()()
{
	return roll();
}

std::ostream& operator<<(std::ostream& os, Roller& r)
{
	return os << r.getDetails();
}

double Roller::sum()
{
	result = 0;
	for (auto d : _dice)
	{
		if (d.isKept())
			result += d.getResult();
	}

	return result;
}

int Roller::getResult() const
{
	return result;
}

std::string Roller::getDetails() const
{
	std::string outp = to_clean_string(result) + "[";
	for (auto it = _dice.begin(); it != _dice.end(); it++)
	{
		std::stringstream ss;

		ss << *it;
		outp.append(ss.str());
		if (to_clean_string(it->getResult()) == to_clean_string(_sides) && _exploding)
			outp.append(std::string(1, Die::expl_char));
		if (!(it == std::prev(_dice.end())))
			outp.append("+");
	}
	outp.append("]");

	return outp;
}

int Roller::roll()
{
	_dice.clear(); // Clear the previous dice
	
	if (_sides <= 0) // Zero sides or less
	{
		deleteError();
		errP = new InputError("A die cannot have less than 1 side.", _rollExpression);
		return 0;
	}

	if (_keepDrop && _kdNum > _startingDice) // Incorrect n. of drop / keep dice
	{
		deleteError();
		errP = new InputError("Incorrect number of keep/drop dice.", _rollExpression);
		return 0;
	}
	
	for (int i = 0; i < _startingDice; i++) // Add starting dice
		_dice.push_back(Die(_sides));

	for (auto it = _dice.begin(); it != _dice.end(); it++)
	{
		if (it->rollDie() == _sides && _exploding)
			_dice.insert(std::next(it), Die(_sides));
	}

	if (_keepDrop == 'k')
		keep();
	else if (_keepDrop == 'd')
		drop();

	return sum();
}

void Roller::keep()
{
	std::vector<double> toKeep; // Results to keep
	toKeep.reserve(_kdNum);
	{
		std::multiset<double> sorted; // Make a sorted list of results
		for (auto d : _dice)
			sorted.insert(d.getResult());

		if (_highLow == 'h') // Push the results to a vector
		{
			auto it = sorted.rbegin();
			for (size_t i = 0; i < _kdNum; i++)
			{
				toKeep.push_back(*it);
				it++;
			}
		}
		else
		{
			auto it = sorted.begin();
			for (size_t i = 0; i < _kdNum; i++)
			{
				toKeep.push_back(*it);
				it++;
			}
		}
	}

	for (auto dice_it = _dice.begin(); dice_it != _dice.end(); dice_it++) // Set keep flags of the dice
	{
		bool keep = false;
		for (auto keep_it = toKeep.begin(); keep_it != toKeep.end(); keep_it++) // Iterate over toKeep values to check if the Die should be kept
		{
			if (dice_it->getResult() == *keep_it)
			{
				toKeep.erase(keep_it);
				keep = true;
				break;
			}
		}
		dice_it->keepDie(keep);
	}
}

void Roller::drop()
{
	std::vector<double> toDrop; // Results to drop
	toDrop.reserve(_kdNum);
	{
		std::multiset<double> sorted; // Make a sorted list of results
		for (auto d : _dice)
			sorted.insert(d.getResult());

		if (_highLow == 'h') // Push the results to a vector
		{
			auto it = sorted.rbegin();
			for (size_t i = 0; i < _kdNum; i++)
			{
				toDrop.push_back(*it);
				it++;
			}
		}
		else
		{
			auto it = sorted.begin();
			for (size_t i = 0; i < _kdNum; i++)
			{
				toDrop.push_back(*it);
				it++;
			}
		}
	}

	for (auto dice_it = _dice.begin(); dice_it != _dice.end(); dice_it++) // Set keep flags of the dice
	{
		bool keep = true;
		for (auto drop_it = toDrop.begin(); drop_it != toDrop.end(); drop_it++) // Iterate over toKeep values to check if the Die should be kept
		{
			if (dice_it->getResult() == *drop_it)
			{
				toDrop.erase(drop_it);
				keep = false;
				break;
			}
		}
		dice_it->keepDie(keep);
	}
}
