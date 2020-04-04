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
private:
	std::string m_time;
};
