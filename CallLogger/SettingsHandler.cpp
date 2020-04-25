#include "SettingsHandler.h"
#include <fstream>

SettingsHandler::SettingsHandler(void)
{
	ResetSettings();

}

void SettingsHandler::ResetSettings(void)
{
	std::ifstream in(m_SettingsFile);
	if (in) {
		std::string temp = "";
		do {
			temp.clear();
			std::getline(in, temp);

			if (temp == "[AutoSplit Logs]") {
				std::getline(in, temp);
				SetAutoSplit(StoB(temp));
			}
			else if (temp == "[Workdays]") {
				std::getline(in, temp);
				char tc = temp.back();
				unsigned dc = 6;
				do {
					temp.pop_back();
					if (tc != ',') {
						if (tc == 't') {
							SetWorkday(dc, true);
							if (dc > 0) { dc--; }
						}
						else if (tc == 'f') {
							SetWorkday(dc, false);
							if (dc > 0) { dc--; }
						}
					}
					if (!temp.empty()) { tc = temp.back(); }
				} while (!temp.empty());
			}


		} while (!in.eof() && !temp.empty());
	}
	else {
		std::ofstream out(m_SettingsFile, std::ofstream::trunc);
		out << "[AutoSplit Logs]\ntrue\n[Workdays]\nt,t,t,t,t,t,t";
	}
}

void SettingsHandler::SaveSettingsToFile(void)
{
	std::ofstream out(m_SettingsFile, std::ofstream::trunc);
	out << "[AutoSplit Logs]\n";
	if (m_AutoSplitLogs) {
		out << "true\n";
	}
	else {
		out << "false\n";
	}
	out << "[Workdays]\n";
	for (unsigned i = 0; i < 7;i++) {
		if (m_WorkDays[i]) {
			out << "t,";
		}
		else {
			out << "f,";
		}
	}

}

unsigned SettingsHandler::GetFirstDayOfWeek(void) const
{
	for (unsigned i = 0; i < 7; i++) {
		if (m_WorkDays[i]) {
			return i;
		}
	}
	return 0;
}

unsigned SettingsHandler::GetLastDayOfWeek(void) const
{
	for (unsigned i = 7; i > 0; i--) {
		if (m_WorkDays[i-1]) {
			return i-1;
		}
	}
	return 6;
}

bool SettingsHandler::StoB(const std::string& inStr) const
{
	if (inStr == "true" || inStr == "True") {
		return true;
	}
	else if (inStr == "false" || inStr == "False") {
		return false;
	}
	else {
		return false;
	}
}
