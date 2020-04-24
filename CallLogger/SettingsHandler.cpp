#include "SettingsHandler.h"
#include <fstream>

SettingsHandler::SettingsHandler(void)
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
				temp.pop_back();
				do {
					if (tc != ',') {
						if (tc == 't') {
							SetWorkday(dc, true);
							dc--;
						}
						else if (tc == 'f') {
							SetWorkday(dc, false);
							dc--;
						}
					}
					tc = temp.back();
					temp.pop_back();
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
