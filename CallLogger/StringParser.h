#pragma once
#include "TimeClock.h"

class StringParser {
public:
	std::string OutputToCSV(std::string inSR, std::string inNotes);
	std::string GetCSVName(void) const { return csvName; }
	void ClearCurrentLog(void);
	void RemoveLastLine(void);
	
private:
	TimeClock m_timer;
	const std::string csvName = "Call Logs\\CallLog.csv";
	std::string ParseRawToCSV(std::string str);
	std::string CraftFullCSVRow(std::string inSR, std::string inNotes);
};