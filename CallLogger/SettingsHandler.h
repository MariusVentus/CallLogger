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

	//Added Options Getters and Setters
	bool GetPN(void) const { return m_DisablePhoneNumbers; }
	bool GetCType(void) const { return m_EnableCallType; }
	bool GetCConnect(void) const { return m_EnableCallConnected; }
	bool GetCTemp(void) const { return m_EnableCallTemp; }
	bool GetOutcomeType(void) const { return m_EnableOutcomeType; }
	bool GetOutcomeConnect(void) const { return m_EnableOutcomeConnected; }
	bool GetOutcomeTemp(void) const { return m_EnableOutcomeTemp; }
	void SetPN(bool input) { m_DisablePhoneNumbers = input; }
	void SetCType(bool input) { m_EnableCallType = input; }
	void SetCConnect(bool input) { m_EnableCallConnected = input; }
	void SetCTemp(bool input) { m_EnableCallTemp = input; }
	void SetOutcomeType(bool input) { m_EnableOutcomeType = input; }
	void SetOutcomeConnect(bool input) { m_EnableOutcomeConnected = input; }
	void SetOutcomeTemp(bool input) { m_EnableOutcomeTemp = input; }

private:
	const std::string m_SettingsFile = "Settings\\Settings.txt";
	bool m_AutoSplitLogs = true;
	bool m_MonthSplitting = false;
	bool m_WorkDays[7] = { true,true,true,true,true,true,true };
	//Added options. 
	bool m_DisablePhoneNumbers = false;
	bool m_EnableCallType = false;
	bool m_EnableCallConnected = false;
	bool m_EnableCallTemp = false;
	bool m_EnableOutcomeType = false;
	bool m_EnableOutcomeConnected = false;
	bool m_EnableOutcomeTemp = false;

	bool StoB(const std::string& inStr) const;

};