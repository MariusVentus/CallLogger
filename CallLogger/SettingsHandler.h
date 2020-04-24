#pragma once
#include <string>

class SettingsHandler {
public:
	SettingsHandler(void);
	void SetAutoSplit(bool split) { M_AutoSplitLogs = split; }
	void SetWorkday(unsigned dayX, bool inSet) { m_WorkDays[dayX] = inSet; }

private:
	const std::string m_SettingsFile = "Settings\\Settings.txt";
	bool M_AutoSplitLogs = true;
	bool m_WorkDays[7] = { true,true,true,true,true,true,true };

	bool StoB(const std::string& inStr) const;

};