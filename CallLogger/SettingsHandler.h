#pragma once
#include <string>

class SettingsHandler {
public:
	SettingsHandler(void);
	bool GetAutoSplit(void) const { return m_AutoSplitLogs; }
	bool GetMonthSplit(void) const { return m_MonthSplitting; }
	bool GetWorkday(unsigned day) const { return m_WorkDays[day]; }
	void ResetSettings(void);
	void SetAutoSplit(bool split) { m_AutoSplitLogs = split; }
	void SetMonthSplit(bool split) { m_MonthSplitting = split; }
	void SetWorkday(unsigned dayX, bool inSet) { m_WorkDays[dayX] = inSet; }
	void SaveSettingsToFile(void);
	unsigned GetFirstDayOfWeek(void) const;
	unsigned GetLastDayOfWeek(void) const;

private:
	const std::string m_SettingsFile = "Settings\\Settings.txt";
	bool m_AutoSplitLogs = true;
	bool m_MonthSplitting = false;
	bool m_WorkDays[7] = { true,true,true,true,true,true,true };

	bool StoB(const std::string& inStr) const;

};