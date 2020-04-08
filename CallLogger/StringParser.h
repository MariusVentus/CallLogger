#pragma once
#include "TimeClock.h"

class StringParser {
public:
	std::string OutputToCSV(std::string inSR, std::string inNotes);
	void RemoveLastLine(void);
	const std::string csvName = "CallLog.csv";
private:
	TimeClock m_timer;
	std::string ParseRawToCSV(std::string str);
	std::string CraftFullCSVRow(std::string inSR, std::string inNotes);
};