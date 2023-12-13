#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void checkString(const std::string& s, int& red, int& green, int& blue) {
	std::smatch m;

	if (std::regex_search(s, m, std::regex("[0-9]* red"))) {
		if (int temp = stoi(m[0]); temp > red) {
			red = temp;
		}
	}

	if (std::regex_search(s, m, std::regex("[0-9]* green"))) {
		if (int temp = stoi(m[0]); temp > green) {
			green = temp;
		}
	}

	if (std::regex_search(s, m, std::regex("[0-9]* blue"))) {
		if (int temp = stoi(m[0]); temp > blue) {
			blue = temp;
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Invalid Useage" << std::endl;
		return 1;
	}

	std::string file_name;
	if (*argv[1] == '1') {
		file_name = "./part_two.input.txt";
	} else {
		file_name = "./part_two.sample.txt";
	}
	std::cout << *argv[1] << std::endl;
	std::cout << "Using file: " << file_name << std::endl;
	std::ifstream ifs(file_name);

	if (not ifs) {
		std::cout << "Unable to open file" << std::endl;
		return 1;
	}

	std::string s;
	unsigned sum = 0;
	while (std::getline(ifs, s)) {
		std::string delimiter = "; ";

		size_t pos = 0;
		std::string token;

		int red	  = 0;
		int green = 0;
		int blue  = 0;
		while (((pos = s.find(delimiter)) != std::string::npos)) {
			token = s.substr(0, pos);
			checkString(token, red, green, blue);
			s.erase(0, pos + delimiter.length());
		}
		checkString(s, red, green, blue);
		sum += red * green * blue;
	}
	std::cout << sum << endl;
	return 0;
}