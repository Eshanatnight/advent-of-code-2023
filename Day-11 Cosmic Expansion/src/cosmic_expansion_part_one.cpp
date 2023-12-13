#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

struct Galaxy {
	int row;
	int col;

	Galaxy(const int row = 0, const int col = 0) : row(row), col(col) {}
};

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Invalid Useage\n";
		return 1;
	}
	std::string file_name;
	if (*argv[1] == '1') {
		file_name = "one.sample.txt";
	} else {
		file_name = "one.input.txt";
	}

	std::string line;
	std::fstream file(file_name);
	std::vector<Galaxy> galaxies;
	std::vector<std::string> image;
	std::vector<int> empty_rows;
	std::vector<int> empty_cols;
	while (std::getline(file, line)) {
		image.push_back(line);
		bool has_galaxy = false;
		for (int col = 0; col < line.size(); col++) {
			if (line[col] == '#') {
				galaxies.emplace_back(image.size() - 1, col);
				has_galaxy = true;
			}
		}
		if (!has_galaxy) {
			empty_rows.push_back(image.size());
		}
	}
	for (int col = 0; col < image[0].size(); col++) {
		bool has_galaxy = false;
		for (const auto& line : image) {
			if (line[col] == '#') {
				has_galaxy = true;
				break;
			}
		}
		if (!has_galaxy) {
			empty_cols.push_back(col);
		}
	}

	std::vector<int> empty_rows_before_galaxy;
	std::vector<int> empty_cols_before_galaxy;
	for (const auto& galaxy : galaxies) {
		empty_rows_before_galaxy.push_back(std::distance(std::begin(empty_rows),
			std::find_if(std::begin(empty_rows), std::end(empty_rows), [&galaxy](const auto& row) {
				return galaxy.row < row;
			})));

		empty_cols_before_galaxy.push_back(std::distance(std::begin(empty_cols),
			std::find_if(std::begin(empty_cols), std::end(empty_cols), [&galaxy](const auto& col) {
				return galaxy.col < col;
			})));
	}

	int total = 0;
	for (int idx_1 = 0; idx_1 < galaxies.size(); idx_1++) {
		const auto& g1 = galaxies[idx_1];
		for (int idx_2 = idx_1 + 1; idx_2 < galaxies.size(); idx_2++) {
			const auto& g2 = galaxies[idx_2];
			total += std::abs(galaxies[idx_2].row - galaxies[idx_1].row) +
					 std::abs(galaxies[idx_2].col - galaxies[idx_1].col) +
					 std::abs(empty_rows_before_galaxy[idx_1] - empty_rows_before_galaxy[idx_2]) +
					 std::abs(empty_cols_before_galaxy[idx_1] - empty_cols_before_galaxy[idx_2]);
		}
	}
	std::cout << "Solution: " << total << '\n';
	return 0;
}