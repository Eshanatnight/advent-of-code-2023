#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Number {
	int col;
	int row;
	int number;
	int order; // number of digits in number
};

struct Gear {
	int n = 0;
	vector<int> numbers;
};

int main(int argc, char* argv[]) {
	string file_name;
	if (argc < 1) {
		cout << "Invalid Useage" << endl;
		return -1;
	}

	if (*argv[1] == '1') {
		file_name = "./part_two.sample.txt";
	} else {
		file_name = "./part_two.input.txt";
	}

	string line;
	fstream file(file_name);

	vector<string> map;
	vector<Number> numbers; // store numbers and their location
	while (getline(file, line)) {
		map.push_back(line);
		bool is_number		= false;
		int number			= -1;
		int start			= 0;
		int order			= 0;
		const auto add_util = [&]() {
			Number n;
			n.row	 = map.size() - 1;
			n.col	 = start;
			n.number = number;
			n.order	 = order;
			numbers.push_back(n);
			number = -1;
			start  = 0;
			order  = 0;
		};

		for (int i = 0; i < line.size(); i++) {
			const char c = line[i];
			if (c >= '0' && c <= '9') {
				if (is_number == false) {
					number = 0;
					start  = i;
					order  = 0;
				}

				is_number = true;
				number	  = (number * 10) + int(c - '0');
				order++;
			} else {
				is_number = false;
			}

			if (number != -1 && !is_number) {
				add_util();
			}
		}

		if (number != -1) {
			add_util();
		}
	}

	unordered_map<int, Gear> gears;
	for (const auto& n : numbers) {
		for (int row = n.row - 1; (row <= n.row + 1); row++) {
			if (row < 0 || row >= map.size()) {
				continue;
			}
			const int col_increment = (row == n.row) ? n.order + 1 : 1;

			for (int col = n.col - 1; (col <= n.col + n.order); col += col_increment) {
				if (col < 0 || col >= map[row].size()) {
					continue;
				}

				if (map[row][col] == '*') {
					const int id = map[0].size() * row + col; // assumes each row is the same length
					if (gears.find(id) == gears.end()) {
						Gear g;
						g.n = 1;
						g.numbers.push_back(n.number);
						gears[id] = g;
					} else {
						gears[id].n++;
						gears[id].numbers.push_back(n.number);
					}
				}
			}
		}
	}

	size_t ans = 0;
	for (const auto& [id, gear] : gears) {
		if (gear.n == 2) {
			ans += (gear.numbers[0] * gear.numbers[1]);
		}
	}

	cout << ans << endl;
	return 0;
}