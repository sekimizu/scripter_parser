#include "pch.h"
#include "ScripterParser.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <unordered_map>

#include <openssl/md5.h>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/algorithm/hex.hpp>

#define DEBUG_VER
//#define GOLDEN_CONFIG_FILE "C:\\Users\\kenny.pan\\Desktop\\keyXDDD"
#define DEFAULT_KEY_FILENAME "C:\\Users\\kenny.pan\\Desktop\\key"
#define GOLDEN_CONFIG_FILE DEFAULT_KEY_FILENAME
#define CONFIG_FILE "C:\\Users\\kenny.pan\\Desktop\\PlanA.txt"

#ifdef DEBUG_VER
#pragma comment(lib, "libcryptoMTd.lib")
#pragma comment(lib, "libsslMTd.lib")
#else
#pragma comment(lib, "libcryptoMT.lib")
#pragma comment(lib, "libsslMT.lib")
#endif

using namespace std;

/* unordered_map<cmd_name, vector<pair<line_number, cmd_value>>> */
typedef unordered_map<string, vector<pair<int, string>>> CONFIG;

string md5_from_file(const string& path) {
	unsigned char result[MD5_DIGEST_LENGTH];
	boost::iostreams::mapped_file_source src(path);
	MD5((unsigned char*)src.data(), src.size(), result);
	ostringstream sout;
	sout << hex << setfill('0');
	for (auto c : result) sout << setw(2) << (int)c;
	return sout.str();
}

class Parser {
private:
	/*
	 * Accept three type of patterns
	 *		AAAAAA=BBBBBB -> name : AAAAAA, value : BBBBBB
	 *		CCCCCC(DDDDDD) -> name : CCCCCC, value : DDDDDD
	 *		EEEEEE.sh FFFFFF -> name : EEEEEE, value : FFFFFF
	 *
	 *		Both BBBBBB, DDDDDD, FFFFFF could be NULL
	 */
	
	CONFIG config;
	vector<vector<string>> pattern = { {"=", "-"}, {"(", ")"}, {".sh", "-"} };
	bool is_key;
	string key_md5sum;
	string filename;
	ifstream mFile;
	
	/* remove left blank */
	string ltrim(const string& s)	{
		size_t start = s.find_first_not_of(" ");
		return (start == string::npos) ? "" : s.substr(start);
	}
	/* remove right blank */
	string rtrim(const string& s)	{
		size_t end = s.find_last_not_of(" ");
		return (end == string::npos) ? "" : s.substr(0, end + 1);
	}
	/* remove all blank */
	string trim(const string& s) {
		return rtrim(ltrim(s));
	}
public:
	Parser(string filepath, bool key) {
		key_md5sum = "";
		this->is_key = key;
		this->filename = filepath;
		if (filepath.empty()) throw "Config filepath could not be empty !";
		mFile.open(filepath);
		if (!mFile.is_open()) throw "Can not open config file !";
	}
	~Parser() {
		if (mFile.is_open()) {
			mFile.close();
		}
	}
	string get_md5_sum() {
		return this->key_md5sum;
	}
	string get_file_name() {
		return this->filename;
	}
	void load_file() {
		string text;
		int line = 0;

		while (getline(mFile, text)) {
			line++;
			bool is_found = false;
			if (text.empty()) continue;
			/* key context need to do un-hex */
			if (is_key) {
				text = boost::algorithm::unhex(text);
			}
			/* record key's md5sum */
			if (!is_key && text.size() >= 3 && text[0] == '#' && text[1] == '#') {
				this->key_md5sum = text.substr(3, text.size());
				cout << " Get key's md5sum = [" << this->key_md5sum << "]" << endl;
			}
			/* skip this line if it starts with '#' */
			if (text[0] == '#') continue;
			/* strip left and right blank */
			text = trim(text);

			/* scan each line according to the pattern type */
			for (int i = 0; !is_found && i < static_cast<int>(pattern.size()); i++) {
				int index = text.find(pattern[i][0]);
				if (index != -1) {
					string name = text.substr(0, index);
					string value = text.substr(index + pattern[i][0].size(), text.size() - (pattern[i][1] == "-" ? 0 : index + pattern[i][0].size() + 1));
					config[name].push_back(make_pair(line, value));
					/* mark is_found to true for early leave */
					is_found = true;
				}
			}
		}
	}
	CONFIG getConfig() {
		return this->config;
	}
#ifdef DEBUG
	void display() {
		for (auto x : config) {
			cout << " CMD : " << x.first << endl;
			for (auto y : x.second) {
				cout << " \t Line : " << y.first << " , Value : " << y.second << endl;
			}
		}
	}
#endif
};

class Checker{
	CONFIG golden, current;
public:
	Checker(CONFIG a, CONFIG b) {
		golden = a;
		current = b;
	}
	void dump_message(string cmd, int index, bool dump_all) {
		if (dump_all) {
			/* print all values with same cmd name */
			cout << ">>> Key config >>> " << golden[cmd].size() << " times !!!" << endl;
			for (auto x : golden[cmd]) {
				cout << " \t Line : " << x.first << " , Value : " << x.second << endl;
			}
			cout << ">>> Current config >>> " << current[cmd].size() << " times !!!" << endl;
			for (auto x : current[cmd]) {
				cout << " \t Line : " << x.first << " , Value : " << x.second << endl;
			}
		} else {
			/* print specific value with given cmd name and value's index */
			cout << ">>> Key config >>>" << endl;
			cout << " \t Line : " << golden[cmd][index].first << " , Value : " << golden[cmd][index].second << endl;
			cout << ">>> Current config >>>" << endl;
			cout << " \t Line : " << current[cmd][index].first << " , Value : " << current[cmd][index].second << endl;
		}
	}
	bool compare() {
		if (golden.size() < current.size()) {
			cout << "strange command found in current config " << endl;
			for (auto x : current) {
				string ignore_cmd = "*" + x.first;
				if (!golden.count(x.first) && !golden.count(ignore_cmd)) {
					cout << " CMD : " << x.first << endl;
					for (auto y : x.second) {
						cout << " \t Line : " << y.first << " , Value : " << y.second << endl;
					}
				}
			}
			return false;
		}
		for (auto x : golden) {
			string cmd = x.first;
			if (cmd.empty() || cmd.at(0) == '*') continue;

			cout << " Check CMD : " << cmd << endl;
			if (x.second.size() != current[cmd].size()) {
				cout << " mismatch found !!!" << endl;
				dump_message(x.first, -1, true);
				return false;
			}
			for (int i = 0; i < static_cast<int>(x.second.size()); i++) {
				if (golden[cmd][i].second != current[cmd][i].second) {
					cout << " mismatch found !!!" << endl;
					dump_message(cmd, i, false);
					return false;
				}
			}
		}
		cout << " All match !!!" << endl;
		return true;
	};
};

int main(int argc, char **argv) {
	int ret = -1;
	if (argc == 1 || argc == 3) {
		try {
			Parser golden(argc == 1 ? GOLDEN_CONFIG_FILE : argv[1], true), current(argc == 1 ? CONFIG_FILE : argv[2], false);
			/* read key and config file */
			cout << " >>>>>>> Read key" << endl;
			golden.load_file();
			cout << " >>>>>>> Read config" << endl;
			current.load_file();
			/* check md5sum is match or not */
			cout << " >>>>>>> Check md5sum" << endl;
			if (current.get_md5_sum().size() != 32) {
				cout << " No valid md5sum found in config file !!!" << endl;
				return 0;
			}
			else if (current.get_md5_sum() != md5_from_file(golden.get_file_name())) {
				cout << " md5sum no match !!!" << endl;
				return 0;
			}
#ifdef DEBUG
			cout << " >>>>>>> Dump golden" << endl;
			golden.display();
			cout << " >>>>>>> Dump current" << endl;
			current.display();
#endif
			/* compare each cmd and value */
			cout << " >>>>>>> Start compare" << endl;
			Checker checker(golden.getConfig(), current.getConfig());
			ret = checker.compare();
		}
		catch (const char* msg) {
			cout << msg << endl;
		}
	}
	else if (argc == 2) {
		ifstream fromFile;
		fstream toFile;

		fromFile.open(argv[1]);
		toFile.open(DEFAULT_KEY_FILENAME, ios::out);

		if (!fromFile.is_open() || !toFile.is_open()) {
			cout << "Can't open file !!!" << endl;
			return ret;
		}

		cout << " About to transfer text to hex data key..." << endl;
		string text;
		while (getline(fromFile, text)) {
			cout << ".";
			toFile << boost::algorithm::hex(text) << endl;
		}
		fromFile.close();
		toFile.close();
		cout << " DONE !!! " << endl;
		cout << " MD5SUM = " << md5_from_file(DEFAULT_KEY_FILENAME) << endl;
	}
	else {
		cout << " Usage : " << endl;
		cout << " \tTo generate hex file (default name : key) and return md5sum : Scripter.exe <Original config file>" << endl;
		cout << " \tTo do the comparsion with two files : Scripter.exe <key> <config file> " << endl;
	}
	return ret;
}