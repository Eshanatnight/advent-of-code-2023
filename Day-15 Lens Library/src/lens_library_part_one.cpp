#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

[[nodiscard]] auto to_string_vec(std::ifstream& fin) -> std::vector<std::string> {
	std::vector<std::string> vec;
	std::string line;
	while (std::getline(fin, line)) {
		vec.push_back(line);
	}
	return vec;
}

[[nodiscard]] auto split(const std::string& str, char delimeter = ' ') -> std::vector<std::string> {
	std::vector<std::string> vec;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimeter)) {
		vec.push_back(token);
	}
	return vec;
}

[[nodiscard]] auto calculate_hash(const std::string& hash) -> uint64_t {
	uint64_t result {0};

	for (int i = 0; i < hash.size(); ++i) {
		result += static_cast<uint64_t>(hash[i]);
		result *= 17;
		result %= 256;
	}

	return result;
}

auto main(int argc, char* argv[]) -> int {
	if (argc <= 1) {
		std::cerr << "Invalid Useage\n";
		return 1;
	}

	std::string file_name;
	if (*argv[1] == '1') {
		file_name = "sample.txt";
	} else {
		file_name = "input.txt";
	}

	std::ifstream fin(file_name);
	if (!fin) {
		std::cerr << "Could not open file: " << file_name << std::endl;
		return 1;
	}

	auto vec						= to_string_vec(fin);
	std::vector<std::string> hashes = split(vec[0], ',');

	uint64_t val {0};

	for (const auto& hash : hashes) {
		val += calculate_hash(hash);
	}

	std::cout << "Solution: " << val << '\n';

	return 0;
}
