#include "StringParser.h"
#include <fstream>
#include <vector>

StringParser::StringParser(const SettingsHandler& inSet, const TimeClock& inTime)
	:
	m_Timer(inTime),
	m_Settings(inSet)
{
	SetCSVfromDate();
}

std::string StringParser::OutputToCSV(std::string inSR, std::string inNotes)
{

	CheckCSV();

	std::ofstream out(m_csvName, std::ofstream::app);
	std::string temp = CraftFullCSVRow(inSR, inNotes);
	out << temp << "\n";

	return temp;
}

std::string StringParser::GetCSVNameNoPath(void) const
{
	auto str = m_csvName;
	str.erase(0, m_csvName.find_last_of("\\") + 1);
	return str;
}

void StringParser::ClearCurrentLog(void)
{
	std::ofstream out(m_csvName, std::ofstream::trunc);
	out << BuildHeader();
}

void StringParser::CheckCSV(void) const
{
	std::ifstream in(m_csvName);
	if (!in) {
		std::ofstream out(m_csvName, std::ofstream::app);
		out << BuildHeader();
	}
}

void StringParser::RemoveLastLine(void)
{
	CheckCSV();
	std::ifstream in(m_csvName);
	std::ifstream count(m_csvName);
	std::string temp;
	std::string fileInfo;
	unsigned lineCount = 0;
	do {
		temp.clear();
		std::getline(count, temp);
		if (!temp.empty()) {
			lineCount++;
		}
	} while (!count.eof() && !temp.empty());
	if (lineCount <= 2) {
		lineCount = 2;
	}

	for (unsigned i = 0; i < lineCount - 1; i++) {
		temp.clear();
		std::getline(in, temp);
		fileInfo.append(temp);
		fileInfo.append("\n");
	}

	std::ofstream out(m_csvName, std::ofstream::trunc);
	out << fileInfo;

}

void StringParser::SetCSVfromDate(void)
{
	m_csvName = m_csvRoot;
	if (m_Settings.GetAutoSplit() || m_Settings.GetMonthSplit()) {
		if (!m_Settings.GetMonthSplit()) {
			int first = (int)m_Settings.GetFirstDayOfWeek();
			int last = (int)m_Settings.GetLastDayOfWeek();
			int day = (int)m_Timer.DayofWeektoInt();
			std::string tempDateF;
			std::string tempDateL;
			m_csvName.append(" ");

			//Add First Day
			tempDateF = m_Timer.GetDateShiftX(first - day);
			tempDateF.erase(tempDateF.find_last_of("-"));
			tempDateF.replace(tempDateF.find("-"), 1, " ");
			m_csvName.append(tempDateF);

			//Add Last Day
			tempDateL = m_Timer.GetDateShiftX(last - day);
			tempDateL.erase(tempDateL.find_last_of("-"));
			tempDateL.replace(tempDateL.find("-"), 1, " ");
			if (tempDateF != tempDateL) {
				m_csvName.append(" to ");
				m_csvName.append(tempDateL);
			}
		}
		else {
			m_csvName.append(" ");
			m_csvName.append(m_Timer.MonthFirstToLast());
		}
	}
	m_csvName.append(m_csvFiletype);
}

void StringParser::StampCurrentLog(void)
{
	CheckCSV();
	std::ifstream in(m_csvName);
	std::ifstream count(m_csvName);
	std::string temp;
	std::vector<std::string> srCount;
	unsigned lineCount = 0;
	do {
		temp.clear();
		std::getline(count, temp);
		if (!temp.empty() && temp.find("Total Cases") == std::string::npos) {
			lineCount++;
		}
	} while (!count.eof() && !temp.empty());

	if (lineCount > 1) {
		std::getline(in, temp);
		temp.clear();
		std::getline(in, temp);

		while (temp.find("Total Cases") != std::string::npos) { std::getline(in, temp); }

		temp.erase(0, temp.find(',') + 1);
		temp.erase(temp.find(','));
		srCount.push_back(temp);

		for (unsigned i = 2; i < lineCount; i++) {
			std::getline(in, temp);
			while (temp.find("Total Cases") != std::string::npos) { std::getline(in, temp); }
			temp.erase(0, temp.find(',') + 1);
			temp.erase(temp.find(','));
			for (unsigned j = 0; j < srCount.size(); j++) {
				if (srCount[j] == temp) {
					break;
				}
				else if (j == srCount.size() - 1) {
					srCount.push_back(temp);
					break;
				}
			}
		}

		std::ofstream out(m_csvName, std::ofstream::app);
		out << "  Total Cases:  ,'" << srCount.size() << ",  Total Calls:  ,'" << lineCount - 1 << "\n";
	}

}

std::string StringParser::ParseRawToCSV(std::string str)
{
	unsigned notMilAmPm = 0; // 0 MilitaryTime, 1 AM, 2 PM
	std::string token = "";
	std::string output = "";


	//Lower all Letters
	for (unsigned i = 0; i < str.size(); i++) {
		if (str[i] >= 65 && str[i] <= 90) {
			str[i] = str[i] + 32; //Make into White Space
		}
	}
	//Remove Double whitespace
	while (str.find("  ") != std::string::npos) {
		str.erase(str.find("  "), 1);
	}
	//Add Trailiing Whitespace so AM/PM check and Time Check goes through full line
	str.append(" ");

	//AM or PM, 
	while (str.find("am") != std::string::npos) {
		if (((str[str.find("am") - 1] == 32) || (str[str.find("am") - 1] >= 48 && str[str.find("am") - 1] <= 57)) && str[str.find("am") + 2] == 32) {
			notMilAmPm = 1;
			break;
		}
		else {
			str.erase(str.find("am"), 2);
		}
	}
	while (str.find("pm") != std::string::npos) {
		if (((str[str.find("pm") - 1] == 32) || (str[str.find("pm") - 1] >= 48 && str[str.find("pm") - 1] <= 57)) && str[str.find("pm") + 2] == 32) {
			notMilAmPm = 2;
			break;
		}
		else {
			str.erase(str.find("pm"), 2);
		}
	}

	//Start Optional Column Checks
	//Call Type - Defaults to Misc
	CTypes callType;
	if (m_Settings.GetCType() || m_Settings.GetOutcomeType()) {
		if (str.find("intro") != std::string::npos) {
			callType = CTypes::Intro;
		}
		else if (str.find("triage") != std::string::npos) {
			callType = CTypes::Triage;
		}
		else if (str.find("lower") != std::string::npos || str.find("close") != std::string::npos || str.find("closure") != std::string::npos) {
			callType = CTypes::Closure;
		}
		else {
			callType = CTypes::Misc;
		}
	}
	//Connected - Assumes Answered
	CConnect callConnected;
	if (m_Settings.GetCConnect() || m_Settings.GetOutcomeConnect()) {
		if (str.find("no answer") != std::string::npos || str.find("didn't answer") != std::string::npos
			|| str.find("voicemail") != std::string::npos || str.find("left vm") != std::string::npos
			|| str.find("no vm") != std::string::npos || str.find("vm full") != std::string::npos
			|| str.find("failed") != std::string::npos)
		{
			if (str.find("no voicemail") != std::string::npos || str.find("no vm") != std::string::npos
				|| str.find("voicemail full") != std::string::npos || str.find("vm full") != std::string::npos) {
				callConnected = CConnect::NoAnswerNoVm;
			}
			else if (str.find("voicemail") != std::string::npos || str.find(" vm") != std::string::npos || str.find(",vm") != std::string::npos) {
				callConnected = CConnect::NoAnswerVM;
			}
			else {
				callConnected = CConnect::NoAnswer;
			}
		}
		else {
			callConnected = CConnect::Answered;
		}
	}
	//Temp - Defaults cool
	CTemps callTemp;
	if (m_Settings.GetCTemp() || m_Settings.GetOutcomeTemp()) {
		if (str.find(" hot ") != std::string::npos || str.find(":hot") != std::string::npos 
			|| str.find(" hot,") != std::string::npos || str.find(",hot") != std::string::npos) {
			callTemp = CTemps::Hot;
		}
		else if (str.find(" warm") != std::string::npos || str.find(":warm") != std::string::npos || str.find(",warm") != std::string::npos) {
			callTemp = CTemps::Warm;
		}
		else {
			callTemp = CTemps::Cool;
		}
	}
	//Ending Optional Column Checks

	//All non-numbers (besides : ) turn to Whitespace
	for (unsigned i = 0; i < str.size(); i++) {
		if (str[i] < 48 || str[i] > 58) {
			str[i] = 32;
		}
	}
	//Remove Double whitespace
	while (str.find("  ") != std::string::npos) {
		str.erase(str.find("  "), 1);
	}
	//Remove Leading Whitespace
	while (str.find(" ") == 0) {
		str.erase(str.find(" "), 1);
	}

	//Remove : Directly
	while (str.find(":") != std::string::npos) {
		str.erase(str.find(":"), 1);
	}

	//Fill Token with Time
	while (str.find(" ") != std::string::npos) {
		if (str.find(" ") > 4) {
			str.erase(0, str.find(" ") + 1);
		}
		else {
			if (str.find(" ") > 2) {
				token = str;
				token.erase(token.find(" "), token.size());
				str.erase(0, str.find(" ") + 1);
				break;
			}
			else {
				str.erase(str.find(" "), 1);
			}
		}
	}

	if (token.empty() && str.size() > 0) {
		token = str;
		str.clear();
	}

	//Time
	if (token.size() > 0) {
		if (token.size() == 4) {
			token.insert(2, ":");
			if (notMilAmPm == 1) {
				token.append(" AM");
			}
			else if (notMilAmPm == 2) {
				token.append(" PM");
			}
		}
		else if (token.size() <= 2) {
			token.append(":00");
			if (notMilAmPm == 1) {
				token.append(" AM");
			}
			else if (notMilAmPm == 2) {
				token.append(" PM");
			}
		}
		else {
			token.insert(1, ":");
			if (notMilAmPm == 1) {
				token.append(" AM");
			}
			else if (notMilAmPm == 2) {
				token.append(" PM");
			}
		}
	}
	output = token;
	token.clear();

	//---------------Phone Number---------------
	if (!m_Settings.GetPN()) {
		output.push_back(',');
		//Country Code check
		unsigned countryCodeNum = 0;
		if (str.find(" ") <= 3) {
			countryCodeNum = str.find(" ");
		}
		//Remove Whitespace until sufficient numbers for Phone Number found, and fill token if found
		while (str.find(" ") != std::string::npos) {
			if (str.find(" ") > 9) {
				token = str;
				token.erase(token.find(" "), token.size());
				if (token.size() > 15) {
					token.erase(15, token.size());
				}
				break;
			}
			else {
				str.erase(str.find(" "), 1);
			}
		}
		//if Token still empty, just put whatever numbers found in it
		if (token.empty()) {
			token = str;
		}
		//Formatting
		if (token.size() != 0 && countryCodeNum != token.size()) //Eval left to right, so fail on condition 1 (==0) shouldn't cause issue with condition 2. 
		{
			if (token.size() - countryCodeNum < 5) {
				countryCodeNum = 0;
			}
			//Dash 1
			if (token.size() - countryCodeNum > 4) {
				token.insert(token.size() - 4, "-");
			}
			//Dash 2
			if ((token.size() - countryCodeNum) > 8) {
				token.insert(token.size() - 8, "-");
			}
			//Country Code Space
			if (countryCodeNum != 0) {
				token.insert(countryCodeNum, " ");
			}
			if (countryCodeNum == 3 && token.size() == 12) {
				token.replace(3, 1, "-");
			}

			//PLUS for International calling
			if (token.find(" ") != std::string::npos || token.size() > 12) {
				token.insert(0, "+");
			}
		}
		token.insert(0, "'");
		output.append(token);
	}

	//Add Optional Columns
	//Type
	if (m_Settings.GetCType()) {
		output.append(",");
		switch (callType)
		{
		case StringParser::CTypes::Intro:
			output.append("Intro");
			break;
		case StringParser::CTypes::Triage:
			output.append("Triage");
			break;
		case StringParser::CTypes::Closure:
			output.append("Closure");
			break;
		case StringParser::CTypes::Misc:
			output.append("Misc.");
			break;
		default:
			output.append("Unknown");
			break;
		}
	}
	//Connected
	if (m_Settings.GetCConnect()) {
		output.append(",");
		switch (callConnected)
		{
		case StringParser::CConnect::Answered:
			output.append("Answered");
			break;
		case StringParser::CConnect::NoAnswer:
			output.append("No Answer");
			break;
		case StringParser::CConnect::NoAnswerNoVm:
			output.append("No Answer-No VM");
			break;
		case StringParser::CConnect::NoAnswerVM:
			output.append("No Answer-VM");
			break;
		default:
			output.append("Unknown");
			break;
		}
	}
	//Temp
	if (m_Settings.GetCTemp()) {
		output.append(",");
		switch (callTemp)
		{
		case StringParser::CTemps::Cool:
			output.append("Cool");
			break;
		case StringParser::CTemps::Warm:
			output.append("Warm");
			break;
		case StringParser::CTemps::Hot:
			output.append("Hot");
			break;
		default:
			output.append("Unknown");
			break;
		}
	}
	//Outcome
	if (m_Settings.GetOutcomeType() || m_Settings.GetOutcomeConnect() || m_Settings.GetOutcomeTemp()) {
		output.append(",");
		//Type
		if (m_Settings.GetOutcomeType()) {
			switch (callType)
			{
			case StringParser::CTypes::Intro:
				output.append(" Intro ");
				break;
			case StringParser::CTypes::Triage:
				output.append(" Triage ");
				break;
			case StringParser::CTypes::Closure:
				output.append(" Closure ");
				break;
			case StringParser::CTypes::Misc:
				output.append(" Misc. ");
				break;
			default:
				output.append(" Unknown Type ");
				break;
			}
		}
		//Connect
		if (m_Settings.GetOutcomeConnect()) {
			switch (callConnected)
			{
			case StringParser::CConnect::Answered:
				output.append(" Answered ");
				break;
			case StringParser::CConnect::NoAnswer:
				output.append(" No Answer ");
				break;
			case StringParser::CConnect::NoAnswerNoVm:
				output.append(" No Answer-No VM ");
				break;
			case StringParser::CConnect::NoAnswerVM:
				output.append(" No Answer-VM ");
				break;
			default:
				output.append(" Success Unknown ");
				break;
			}
		}
		//Temp
		if (m_Settings.GetOutcomeTemp()) {
			switch (callTemp)
			{
			case StringParser::CTemps::Cool:
				output.append(" Temp:Cool ");
				break;
			case StringParser::CTemps::Warm:
				output.append(" Temp:Warm ");
				break;
			case StringParser::CTemps::Hot:
				output.append(" Temp:Hot ");
				break;
			default:
				output.append(" Temp:Unknown ");
				break;
			}
		}
	}

	return output;
}

std::string StringParser::CraftFullCSVRow(std::string inSR, std::string inNotes)
{
	std::string row;
	row = m_Timer.GetDate();
	row.push_back(',');
	
	//Erase inSR non-numbers
	for (unsigned i = 0; i < inSR.size();) {
		if (inSR[i] < 48 || inSR[i] > 57) {
			inSR.erase(i, 1);
			if (i != 0) { --i; }
			else { i = 0; }
		}
		else {
			i++;
		}
	}
	row.append("'");
	row.append(inSR);
	row.push_back(',');

	row.append(ParseRawToCSV(inNotes));

	return row;

}

std::string StringParser::BuildHeader(void) const
{
	std::string output = "  Date  ,  SR#  ,  Time  ";
	if (!m_Settings.GetPN()) {
		output.append(",  Phone#  ");
	}
	if (m_Settings.GetCType()) {
		output.append(",  Type  ");
	}
	if (m_Settings.GetCConnect()) {
		output.append(",  Connected(Y//N)  ");
	}
	if (m_Settings.GetCTemp()) {
		output.append(",  Temp  ");
	}
	if (m_Settings.GetOutcomeType() || m_Settings.GetOutcomeConnect() || m_Settings.GetOutcomeTemp()) {
		output.append(",  Outcome  ");
	}
	output.append("\n");
	return output;
}
