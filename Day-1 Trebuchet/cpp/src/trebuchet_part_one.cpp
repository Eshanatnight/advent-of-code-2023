#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

auto get_number(const std::string& line) -> uint32_t {
	std::string num_str = "";

	for (int i = 0; i < line.size(); ++i) {
		if (isdigit(line[i])) {
			num_str += line[i];
		}
	}

	std::string number = "";

	if (num_str.size() == 1) {
		number += num_str[0];
		number += num_str[0];
	} else {
		number += num_str[0];
		number += num_str[num_str.size() - 1];
	}

	return std::stoi(number);
}

auto main(int argc, char* argv[]) -> int {
	auto result = 0;
	std::string file_name;
	if (argc < 2) {
		std::cout << "Please provide a file number" << std::endl;
		return 1;
	}

	if (*argv[1] == '1') {
		file_name = "./one.input.txt";
	} else {
		file_name = "./one.sample.txt";
	}
	std::ifstream file(std::move(file_name));
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			result += get_number(line);
		}

		file.close();
	}

	std::cout << "Result: " << result << std::endl;
}