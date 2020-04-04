#pragma once
#include "TimeClock.h"

class StringParser {
public:
	void OutputToCSV(std::string inSR, std::string inNotes);
private:
	TimeClock m_timer;
	std::string ParseRawToCSV(std::string str);
	std::string CraftFullCSVRow(std::string inSR, std::string inNotes);
};