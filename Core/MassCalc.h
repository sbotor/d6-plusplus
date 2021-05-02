#ifndef MASSCALC_H
#define MASSCALC_H

#include <vector>
#include <mutex>
#include <fstream>

#include "../Utility/History.h"

class MassCalc
{
private:
	std::vector<History> v;
	size_t errCount;
	std::mutex counterMtx;

	void procThread(size_t first, size_t last);
	void incErrC();

public:
	MassCalc();
	MassCalc(std::ifstream& file);
	
	void add(const std::string& inpStr);
	size_t process();
	size_t process(std::ifstream& file);
	void save(std::ofstream& file);
	size_t size();
	size_t getErrNum();
};

#endif // !MASSCALC_H
