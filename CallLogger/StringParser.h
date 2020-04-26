#pragma once
#include "TimeClock.h"
#include "SettingsHandler.h"

class StringParser {
public:
	StringParser(const SettingsHandler& inSet, const TimeClock& inTime);
	std::string OutputToCSV(std::string inSR, std::string inNotes);
	std::string GetCSVName(void) const { return m_csvName; }
	std::string GetCSVNameNoPath(void) const;
	void ClearCurrentLog(void);
	void CheckCSV(void) const;
	void RemoveLastLine(void);
	void SetCSVfromDate(void);
	void StampCurrentLog(void);
	
private:
	const TimeClock& m_Timer;
	const SettingsHandler& m_Settings;
	const std::string m_csvRoot = "Call Logs\\CallLog";
	const std::string m_csvFiletype = ".csv";
	std::string m_csvName = "";
	std::string ParseRawToCSV(std::string str);
	std::string CraftFullCSVRow(std::string inSR, std::string inNotes);
};