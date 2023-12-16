#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

auto process_input(const string& file_name) -> vector<string> {
	ifstream file(file_name);
	string line;

	vector<string> input;
	while (getline(file, line)) {
		input.push_back(line);
	}

	return input;
}

auto is_num(const string& str) -> bool {
	for (const auto& c : str) {
		if (c == '-') {
			continue;
		}

		if (c < '0' || c > '9') {
			return false;
		}
	}

	return true;
}

auto get_ints(const string& line) -> vector<int64_t> {
	vector<int64_t> list;

	stringstream ss(line);
	string token;

	while (ss >> token) {
		if (!token.empty() && is_num(token)) {
			list.push_back(stoll(token));
		}
	}

	return list;
}

auto solve(const vector<string>& input) -> int64_t {
	int64_t next_val {0};

	for (const auto& line : input) {
		if (line.empty()) {
			continue;
		}
		vector<vector<int64_t>> extrapolate;
		vector<int64_t> history = get_ints(line);

		extrapolate.push_back(history);

		uint64_t extrapolate_idx {0};

		while (true) {
			bool is_only_zero {true};
			vector<int64_t> sequence;

			int extrapolate_size = extrapolate[extrapolate_idx].size();
			for (int i = 1; i < extrapolate_size; ++i) {
				int64_t new_number_in_sequence =
					extrapolate[extrapolate_idx][i] - extrapolate[extrapolate_idx][i - 1];
				sequence.push_back(new_number_in_sequence);
				if (new_number_in_sequence != 0) {
					is_only_zero = false;
				}
			}

			extrapolate.push_back(sequence);

			if (is_only_zero) {
				break;
			}

			++extrapolate_idx;
		}

		for (const auto& history : extrapolate) {
			next_val += history[history.size() - 1];
		}
	}

	return next_val;
}

auto main(int argc, char* argv[]) -> int {
	if (argc != 2) {
		cerr << "Invalid Useage" << endl;
		return 1;
	}

	string file_name;
	if (*argv[1] == '1') {
		file_name = "sample.txt";
	} else {
		file_name = "input.txt";
	}

	auto input = process_input(file_name);

	auto result = solve(input);

	cout << "Result: " << result << endl;

	return 0;
}
