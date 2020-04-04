#pragma once
#include "TimeClock.h"

class StringParser {
public:
	std::string ParseRawToCSV(std::string str);
	std::string CraftFullCSVRow(std::string inSR, std::string inNotes);
private:
	TimeClock m_timer;
};