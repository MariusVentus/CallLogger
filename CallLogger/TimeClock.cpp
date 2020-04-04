#include "TimeClock.h"
#include <ctime>

TimeClock::TimeClock(void)
{
	RefreshTime();
}

void TimeClock::RefreshTime(void)
{
	time_t result = time(NULL);
	char cstr[26];
	ctime_s(cstr, sizeof cstr, &result);
	m_time = cstr;
	//Remove Double Whitespace
	while (m_time.find("  ") != std::string::npos) {
		m_time.erase(m_time.find("  "), 1);
	}
	//Remove newline
	if (m_time.find("\n") != std::string::npos) {
		m_time.erase(m_time.find("\n"), 1);
	}
}

std::string TimeClock::GetFullTime(void) const
{
	return m_time;
}

std::string TimeClock::GetDate(void) const
{
	auto str = m_time;

	//Remove Day
	str.erase(0, str.find(" ") + 1);

	//EraseTime
	str.erase(str.find(":") - 2, 2);
	str.erase(str.find(":"), 6);

	//Remove Double Whitespace
	while (str.find("  ") != std::string::npos) {
		str.erase(str.find("  "), 1);
	}

	//Whitespace to -
	while (str.find(" ") != std::string::npos) {
		str.replace(str.find(" "), 1, "-");
	}
	return str;
}

std::string TimeClock::GetDateNDay(void) const
{
	auto str = m_time;

	//EraseTime
	str.erase(str.find(":") - 2, 2);
	str.erase(str.find(":"), 6);

	//Remove Double Whitespace
	while (str.find("  ") != std::string::npos) {
		str.erase(str.find("  "), 1);
	}

	return str;
}

std::string TimeClock::GetDay(void) const
{
	auto str = m_time;
	//Remove all but Day
	str.erase(str.find(" "));

	return str;
}

