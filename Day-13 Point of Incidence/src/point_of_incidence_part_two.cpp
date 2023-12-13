#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto are_different_rows(const std::vector<std::string>& pattern, int row1, int row2) -> int64_t {
	int count = 0;
	for (int i = 0; i < pattern[row1].size(); ++i) {
		if (pattern[row1][i] != pattern[row2][i]) {
			++count;
		}
	}
	return count;
}

auto are_different_cols(const std::vector<std::string>& pattern, int col1, int col2) -> int64_t {
	int count = 0;
	for (int i = 0; i < pattern.size(); ++i) {
		if (pattern[i][col1] != pattern[i][col2]) {
			++count;
		}
	}
	return count;
}

auto is_fixable_mirror_row(
	const std::vector<std::string>& patterns, int mirror_row, int test_row, bool& fixed) -> bool {
	int32_t distance = std::abs(mirror_row - test_row);

	if (test_row < 0 || test_row + ((distance * 2) - 1) >= patterns.size()) {
		return true;
	} else {
		int64_t differences =
			are_different_rows(patterns, test_row, test_row + ((distance * 2) - 1));
		if (differences == 1) {
			fixed = true;
			return is_fixable_mirror_row(patterns, mirror_row, test_row - 1, fixed);
		} else if (differences == 0) {
			return is_fixable_mirror_row(patterns, mirror_row, test_row - 1, fixed);
		} else {
			return false;
		}
	}
}

auto is_fixable_mirror_cols(
	const std::vector<std::string>& patterns, int mirror_col, int test_col, bool& fixed) -> bool {
	int32_t distance = std::abs(mirror_col - test_col);
	if (test_col < 0 || test_col + ((distance * 2) - 1) >= patterns[0].size()) {
		return true;
	} else {
		int differences = are_different_cols(patterns, test_col, test_col + ((distance * 2) - 1));
		if (differences == 1) {
			fixed = true;
			return is_fixable_mirror_cols(patterns, mirror_col, test_col - 1, fixed);
		} else if (differences == 0) {
			return is_fixable_mirror_cols(patterns, mirror_col, test_col - 1, fixed);
		} else {
			return false;
		}
	}
}

auto find_fixable_mirrors_row(const std::vector<std::string>& patterns) -> int64_t {
	for (int i = 1; i < patterns.size(); ++i) {
		bool fixed = false;

		if (is_fixable_mirror_row(patterns, i, i - 1, fixed)) {
			if (fixed) {
				return i;
			}
		}
	}

	return -1;
}

auto find_fixable_mirrors_cols(const std::vector<std::string>& patterns) -> int64_t {
	for (int i = 1; i < patterns[0].size(); ++i) {
		bool fixed = false;
		if (is_fixable_mirror_cols(patterns, i, i - 1, fixed)) {
			if (fixed) {
				return i;
			}
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

		int64_t fixable_mirrors = find_fixable_mirrors_row(patterns);

		if (fixable_mirrors != -1) {
			sum += 100 * fixable_mirrors;
		} else {
			fixable_mirrors = find_fixable_mirrors_cols(patterns);

			if (fixable_mirrors != -1) {
				sum += fixable_mirrors;
			} else {
				std::cerr << "No Fixable Mirrors\n";
				return 1;
			}
		}

		patterns.clear();
	}

	std::cout << "Solution: " << sum << '\n';

	return 0;
}
