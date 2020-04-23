#pragma once
#include <string>

class TimeClock {
public:
	TimeClock(void);
	void RefreshTime(void);
	std::string GetFullTime(void) const;
	std::string GetDate(void) const;
	std::string GetDateNDay(void) const;
	std::string GetDay(void) const;
	void SetTime(const std::string& inTime) { m_time = inTime; }
private:
	std::string m_time;
};
