#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool are_matching_cols(const std::vector<std::string>& patterns, int col1, int col2) {
	for (int i = 0; i < patterns.size(); ++i) {
		if (patterns[i][col1] != patterns[i][col2]) {
			return false;
		}
	}
	return true;
}

auto are_matching_rows(const std::vector<std::string>& patterns, int row1, int row2) -> bool {
	return patterns[row1] == patterns[row2];
}

auto is_mirror_col(const std::vector<std::string>& patterns, int mirror_column, int test_column)
	-> int64_t {
	uint16_t distance = std::abs(mirror_column - test_column);

	if (test_column < 0 || test_column + ((distance * 2) - 1) >= patterns[0].size()) {
		return true;
	} else {
		return is_mirror_col(patterns, mirror_column, test_column - 1) &&
			   are_matching_cols(patterns, test_column, test_column + ((distance * 2) - 1));
	}
}

auto is_mirror_row(const std::vector<std::string>& patterns, int64_t mirror_row, int64_t test_row)
	-> bool {
	int16_t distance = std::abs(mirror_row - test_row);

	if (test_row < 0 || test_row + ((distance * 2) - 1) >= patterns.size()) {
		return true;
	} else {
		return is_mirror_row(patterns, mirror_row, test_row - 1) &&
			   are_matching_rows(patterns, test_row, test_row + ((distance * 2) - 1));
	}
}

auto find_mirror_row(const std::vector<std::string>& patterns) -> int64_t {
	for (int i = 1; i < patterns.size(); ++i) {
		if (is_mirror_row(patterns, i, i - 1)) {
			return i;
		}
	}

	return -1;
}

auto find_mirror_col(const std::vector<std::string>& patterns) -> int64_t {
	for (int i = 1; i < patterns[0].size(); ++i) {
		if (is_mirror_col(patterns, i, i - 1)) {
			return i;
		}
	}
	return -1;
}

auto main(int argc, char* argv[]) -> int {
	if (argc < 2) {
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
		std::cerr << "File Didnt Open Cluster F**k\n";
		return 1;
	}

	std::string line;
	std::vector<std::string> lines;
	while (std::getline(fin, line)) {
		lines.push_back(line);
	}
	int64_t sum = 0;
	for (int i = 0; i < lines.size(); ++i) {
		std::vector<std::string> patterns;

		// while line is not empty
		while (lines[i].size() > 0) {
			patterns.push_back(lines[i]);
			++i;
		}

		int64_t mirror = find_mirror_col(patterns);

		if (mirror != -1) {
			sum += mirror;
		} else {
			mirror = find_mirror_row(patterns);

			if (mirror != -1) {
				sum += 100 * mirror;
			} else {
				std::cout << "Welp No Mirrors\n";
			}
		}

		patterns.clear();
	}

	std::cout << "Solution: " << sum << '\n';

	return 0;
}
