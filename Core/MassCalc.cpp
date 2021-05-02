#include <thread>

#include <QtDebug>

#include "MassCalc.h"

void MassCalc::procThread(size_t first, size_t last)
{
	for (size_t i = first; i < last; i++)
	{
		// Convert
		Converter conv(v[i].getExpression());
		if (conv()) // Conversion error
		{
			v[i].setDetails(conv.getError()->what());
			incErrC();
			continue;
		}

		// Evaluate
		Evaluator ev(conv.getConverted());
		if (ev())
		{
			v[i].setDetails(ev.getError()->what());
			incErrC();
			continue;
		}

		// If no errors
		v[i].setDetails(conv.getDetails());
		v[i].setResult(ev.getResult());
	}
}

void MassCalc::incErrC()
{
	std::unique_lock<std::mutex> lock(counterMtx);
	++errCount;
	lock.unlock();
}

MassCalc::MassCalc() : errCount(0) {}

MassCalc::MassCalc(std::ifstream& file)
{
	process(file);
}

void MassCalc::add(const std::string& inpStr)
{
	if (!inpStr.empty())
        v.push_back(History("", inpStr, ""));
}

size_t MassCalc::process()
{   
    errCount = 0;
	size_t n = v.size();

	if (n >= 4) // Four threads for n >= 4
	{
		std::thread th1(&MassCalc::procThread, this, 0, n / 4);
		std::thread th2(&MassCalc::procThread, this, n / 4, 2 * n / 4);
		std::thread th3(&MassCalc::procThread, this, n / 4, 3 * n / 4);
		procThread(3 * n / 4, n);

		th1.join();
		th2.join();
		th3.join();
	}
	else if (n < 4 && n > 0) // A single thread for n < 4
		procThread(0, n);
	else
		return 0;

	return errCount;
}

size_t MassCalc::process(std::ifstream& file)
{
	v.clear();

	std::string line;

	while (std::getline(file, line, '\n'))
		add(line);

	return process();
}

void MassCalc::save(std::ofstream& file)
{
	for (auto& x : v)
		file << x;
}

size_t MassCalc::size()
{
	return v.size();
}

size_t MassCalc::getErrNum()
{
	return errCount;
}
