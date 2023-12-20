#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
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

[[nodiscard]] auto solve(std::string& vec) -> int64_t {
	using key_t	  = std::string;
	using value_t = std::vector<std::pair<std::string, int64_t>>;

	std::vector<std::string> hashes = split(vec, ',');

	std::unordered_map<key_t, value_t> hash_map;

	uint64_t val {0};

	for (const auto& hash : hashes) {
		auto hash_split = split(hash, '=');

		bool is_removal;

		if (hash_split.size() == 2) {
			is_removal = false;
		} else {
			is_removal = true;
			// delete the last character from label
			hash_split[0].pop_back();
		}

		auto box = calculate_hash(hash_split[0]);

		if (is_removal) {
			// ? IS REMOVAL
			// if the box is in the map, check if it's vector contains the label (hash_split[0])
			if (hash_map.find(std::to_string(box)) != hash_map.end()) {
				// if the label is in the vector, remove it
				for (int i = 0; i < hash_map[std::to_string(box)].size(); i++) {
					if (hash_map[std::to_string(box)][i].first == hash_split[0]) {
						hash_map[std::to_string(box)].erase(
							hash_map[std::to_string(box)].begin() + i);
						break;
					}
				}
				// if the vector of the box is empty, remove the box from the map
				if (hash_map[std::to_string(box)].empty()) {
					hash_map.erase(std::to_string(box));
				}
			}
			// no action if the box is not in the map
		} else {
			// ? IS ADDITION
			// if the box is in the map, check if it's vector contains the label (hash_split[0])
			if (hash_map.find(std::to_string(box)) != hash_map.end()) {
				// if the label is in the vector, add 1 to the count
				bool found = false;
				for (int i = 0; i < hash_map[std::to_string(box)].size(); i++) {
					if (hash_map[std::to_string(box)][i].first == hash_split[0]) {
						hash_map[std::to_string(box)][i].second = std::stoi(hash_split[1]);
						found									= true;
						break;
					}
				}
				// if the label is not in the vector, add it
				if (!found) {
					hash_map[std::to_string(box)].push_back(
						std::make_pair(hash_split[0], std::stoi(hash_split[1])));
				}
			} else {
				// if the box is not in the map, add it and add the label
				hash_map[std::to_string(box)].push_back(
					std::make_pair(hash_split[0], std::stoi(hash_split[1])));
			}
		}
	}

	int64_t ret_val = 0;

	// calculate the value of the boxes
	for (auto box : hash_map) {
		int64_t box_value = 0;
		for (int i = 1; i <= box.second.size(); i++) {
			box_value += (1 + std::stoi(box.first)) * i * box.second[i - 1].second;
		}
		ret_val += box_value;
	}

	return ret_val;
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

	auto vec	= to_string_vec(fin)[0];
	auto result = solve(vec);
	std::cout << result << std::endl;

	return 0;
}
