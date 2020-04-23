#include "StringParser.h"
#include <fstream>
#include <vector>

std::string StringParser::OutputToCSV(std::string inSR, std::string inNotes)
{

	std::ifstream in(csvName);
	if (!in) {
		std::ofstream tempOut(csvName, std::ofstream::app);
		tempOut << "  Date  ,  SR#  ,  Time  ,  Phone#  \n";
	}

	std::ofstream out(csvName, std::ofstream::app);
	out << CraftFullCSVRow(inSR, inNotes) << "\n";

	return CraftFullCSVRow(inSR, inNotes);
}

void StringParser::ClearCurrentLog(void)
{
	std::ofstream out(csvName, std::ofstream::trunc);
	out << "  Date  ,  SR#  ,  Time  ,  Phone#  \n";
}

void StringParser::RemoveLastLine(void)
{
	std::ifstream in(csvName);
	if (in) {
		std::ifstream count(csvName);
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

		std::ofstream out(csvName, std::ofstream::trunc);
		out << fileInfo;
	}
}

void StringParser::StampCurrentLog(void)
{
	std::ifstream in(csvName);
	if (in) {
		std::ifstream count(csvName);
		std::string temp;
		std::vector<std::string> srCount;
		unsigned lineCount = 0;
		do {
			temp.clear();
			std::getline(count, temp);
			if (!temp.empty()) {
				lineCount++;
			}
		} while (!count.eof() && !temp.empty());

		if (lineCount > 1) {
			std::getline(in, temp);
			temp.clear();
			std::getline(in, temp);
			temp.erase(0, temp.find(',') + 1);
			temp.erase(temp.find(','));
			srCount.push_back(temp);

			for (unsigned i = 2; i < lineCount; i++) {
				std::getline(in, temp);
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

			std::ofstream out(csvName, std::ofstream::app);
			out << "  Total Cases:  ,'" << srCount.size() << ",  Total Calls:  ,'" << lineCount - 1 << "\n";
		}
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

	//AM or PM, 
	while (str.find("am") != std::string::npos) {
		if (str[str.find("am") + 2] < 97 || str[str.find("am") + 2] > 122) {
			notMilAmPm = 1;
			break;
		}
		else {
			str.erase(str.find("am"), 2);
		}
	}
	while (str.find("pm") != std::string::npos) {
		if (str[str.find("pm") + 2] < 97 || str[str.find("pm") + 2] > 122) {
			notMilAmPm = 2;
			break;
		}
		else {
			str.erase(str.find("pm"), 2);
		}
	}

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
	//Time
	if (token.size() > 0) {
		if (token.size() == 4) {
			token.insert(2, ":");
			if (notMilAmPm == 1) {
				token.insert(5, " AM");
			}
			else if (notMilAmPm == 2) {
				token.insert(5, " PM");
			}
		}
		else {
			token.insert(1, ":");
			if (notMilAmPm == 1) {
				token.insert(4, " AM");
			}
			else if (notMilAmPm == 2) {
				token.insert(4, " PM");
			}
		}
	}
	token.push_back(',');

	output = token;
	token.clear();

	//Phone Number
	unsigned countryCodeNum = 0;
	if (str.find(" ") <= 3) {
		countryCodeNum = str.find(" ");
	}
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
	if (token == "") {
		token = str;
	}
	if (token.size() != 0) {
		token.insert(token.size() - 4, "-");
		if ((token.size() - countryCodeNum) > 8) {
			token.insert(token.size() - 8, "-");
		}
		if (countryCodeNum != 0) {
			token.insert(countryCodeNum, " ");
		}
		if (countryCodeNum == 3 && (token.size() - countryCodeNum) == 9) {
			token.replace(token.size() - 9, 1, "-");
		}

		if (countryCodeNum != 3 || token.size() > 12) {
			token.insert(0, "+");
		}
	}
	token.insert(0, "'");
	output.append(token);
	return output;
}

std::string StringParser::CraftFullCSVRow(std::string inSR, std::string inNotes)
{
	std::string row;
	row = m_timer.GetDate();
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
