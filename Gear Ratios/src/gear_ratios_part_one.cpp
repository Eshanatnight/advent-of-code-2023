#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <numeric>

using namespace std;

struct Number {
    int col;
    int row;
    int number;
    int order;      // number of digits in number
};


auto main(int argc, char* argv[]) -> int
{
    if(argc != 2) {
        std::cout << "Invalid Useage" << std::endl;
        return 1;
    }
    std::string file_name;
    if(*argv[1] == '1') {
        file_name = "./part_one.sample.txt";
    } else {
        file_name = "./part_one.input.txt";
    }

    std::ifstream ifs(file_name);
    if (!ifs) {
        std::cout << "Unable to open file" << std::endl;
    }

    string line;

    vector<string> map;
    vector<Number> numbers;

    while (getline(ifs, line)) {
        map.push_back(line);
        bool is_number = false;
        int number = -1;
        int start = 0;
        int order = 0;

        const auto add_util = [&]() {
            Number n;
            n.row = map.size() - 1;
            n.col = start;
            n.number = number;
            n.order = order;

            numbers.push_back(n);
            number = -1;
            start = 0;
            order = 0;
        };

        for (int i = 0; i < line.size(); ++i) {
            const char c = line[i];

            if (c >= '0' && c <= '9') {
                if (is_number == false) {
                    number = 0;
                    start = i;
                    order = 0;
                }

                is_number = true;

                number = (number * 10) + static_cast<int>(c - '0');
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


    vector<int> parts;

    for (const auto& n : numbers) {
        bool still_searching = true;

        for (int row = n.row - 1; (row <= n.row + 1) && still_searching; row++) {
            if (row < 0 || row >= map.size()) {
                continue;
            }

            const int col_increment = (row == n.row) ? n.order + 1 : 1;
            for (int col = n.col - 1; (col <= n.col + n.order) && still_searching; col += col_increment) {
                if (col < 0 || col >= map[row].size()) {
                    continue;
                }
                if (map[row][col] != '.' && !(map[row][col] >= '0' && map[row][col] <= '9')) {
                    still_searching = false;
                    parts.push_back(n.number);
                }
            }
        }
    }

    const auto total = accumulate(std::begin(parts), std::end(parts), 0);
    std::cout << total << '\n';
    return 0;
}
