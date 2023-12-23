#include "./cpputils.h"

#include "cpputils.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

[[nodiscard]] auto read_file_to_string(const std::string& file_name) -> std::string {
	std::ifstream file(file_name);

	if (!file) {
		std::cerr << "Could not open file: " << file_name << std::endl;
		std::exit(FILE_NOT_FOUND);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

[[nodiscard]] auto to_string_vec(std::string_view str) -> std::vector<std::string> {
	std::vector<std::string> vec;
	std::stringstream ss(str.data());
	std::string line;
	while (std::getline(ss, line)) {
		vec.push_back(line);
	}
	return vec;
}

[[nodiscard]] auto to_string_vec(std::ifstream& fin) -> std::vector<std::string> {
	if (!fin) {
		std::cerr << "File not found\n";
		std::exit(FILE_NOT_FOUND);
	}

	std::vector<std::string> vec;
	std::string line;
	while (std::getline(fin, line)) {
		vec.push_back(line);
	}
	return vec;
}

[[nodiscard]] auto split(const std::string& str, char delimeter) -> std::vector<std::string> {
	std::vector<std::string> vec;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimeter)) {
		vec.push_back(token);
	}
	return vec;
}
