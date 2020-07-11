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
			else if (temp == "[MonthSplit Logs]") {
				std::getline(in, temp);
				SetMonthSplit(StoB(temp));
			}
			//Added Options Start
			else if (temp == "[Disable PhoneNumbers]") {
				std::getline(in, temp);
				SetPN(StoB(temp));
			}
			else if (temp == "[Enable CallType]") {
				std::getline(in, temp);
				SetCType(StoB(temp));
			}
			else if (temp == "[Enable CallConnected]") {
				std::getline(in, temp);
				SetCConnect(StoB(temp));
			}
			else if (temp == "[Enable CallTemp]") {
				std::getline(in, temp);
				SetCTemp(StoB(temp));
			}
			else if (temp == "[Enable OutcomeType]") {
				std::getline(in, temp);
				SetOutcomeType(StoB(temp));
			}
			else if (temp == "[Enable OutcomeConnected]") {
				std::getline(in, temp);
				SetOutcomeConnect(StoB(temp));
			}
			else if (temp == "[Enable OutcomeTemp]") {
				std::getline(in, temp);
				SetOutcomeTemp(StoB(temp));
			}
			//Added Options End
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
		SaveSettingsToFile();
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
	out << "[MonthSplit Logs]\n";
	if (m_MonthSplitting) {
		out << "true\n";
	}
	else {
		out << "false\n";
	}

	//Added Options Start - Adding settings can be easier. No reason to keep this.
	out << "[Disable PhoneNumbers]\n";
	if (m_DisablePhoneNumbers) {
		out << "true\n";
	}
	else {
		out << "false\n";
	}
	//
	out << "[Enable CallType]\n";
	if (m_EnableCallType) {
		out << "true\n";
	}
	else {
		out << "false\n";
	}
	//
	out << "[Enable CallConnected]\n";
	if (m_EnableCallConnected) {
		out << "true\n";
	}
	else {
		out << "false\n";
	}
	//
	out << "[Enable CallTemp]\n";
	if (m_EnableCallTemp) {
		out << "true\n";
	}
	else {
		out << "false\n";
	}
	//
	out << "[Enable OutcomeType]\n";
	if (m_EnableOutcomeType) {
		out << "true\n";
	}
	else {
		out << "false\n";
	}
	//
	out << "[Enable OutcomeConnected]\n";
	if (m_EnableOutcomeConnected) {
		out << "true\n";
	}
	else {
		out << "false\n";
	}
	//
	out << "[Enable OutcomeTemp]\n";
	if (m_EnableOutcomeTemp) {
		out << "true\n";
	}
	else {
		out << "false\n";
	}
	//Added Options End

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
	auto str = inStr;
	//Lower all Letters
	for (unsigned i = 0; i < str.size(); i++) {
		if (str[i] >= 65 && str[i] <= 90) {
			str[i] = str[i] + 32; //Make into lowercase
		}
	}
	//Bool
	if (str == "true") {
		return true;
	}
	else if (str == "false") {
		return false;
	}
	else {
		return false;
	}
}
