#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// only 12 red cubes, 13 green cubes, and 14 blue cubes
auto checkString(const std::string& s) -> bool {
	std::smatch m;

	if (std::regex_search(s, m, std::regex("[0-9]* red"))) {
		if (stoi(m[0]) > 12) return false;
	}

	if (std::regex_search(s, m, std::regex("[0-9]* green"))) {
		if (stoi(m[0]) > 13) return false;
	}

	if (std::regex_search(s, m, std::regex("[0-9]* blue"))) {
		if (stoi(m[0]) > 14) return false;
	}

	return true;
}

auto main(int argc, char* argv[]) -> int {
	if (argc != 2) {
		std::cout << "Invalid Useage" << std::endl;
		return 1;
	}
	std::string file_name;
	if (*argv[1] == '1') {
		file_name = "./part_one.sample.txt";
	} else {
		file_name = "./part_one.input.txt";
	}

	std::ifstream ifs(file_name);
	if (not ifs) {
		std::cout << "Unable to open file" << std::endl;
	}

	std::string line;
	unsigned sum = 0;
	while (std::getline(ifs, line)) {
		int id = 0;
		char buf[1024]; // possible buffer overflow if line is longer than 1024 characters
		sscanf(line.c_str(), "Game %d: %[^\t\n]", &id, &buf);

		std::string s(buf);
		std::string delimiter = "; ";

		size_t pos = 0;
		std::string token;
		bool found = false;
		while (((pos = s.find(delimiter)) != std::string::npos) and not found) {
			token = s.substr(0, pos);
			if (not checkString(token)) {
				found = true;
			}
			s.erase(0, pos + delimiter.length());
		}
		if (not found and not checkString(s)) {
			found = true;
		}

		if (not found) {
			sum += id;
		}
	}
	std::cout << sum << endl;
	return 0;
}
