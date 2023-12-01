#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>

auto find_number(const std::string& line) -> uint32_t {
    const std::vector<std::string> numbers = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    std::vector<std::vector<int>> found_numbers;

    for(int i= 0; i < line.size(); ++i) {
        if(isdigit(line[i])) {
            found_numbers.push_back({i, line[i] - '0'});
        }
    }

    for(int i = 0; i < numbers.size(); ++i) {
        int occurence = line.find(numbers[i], 0);
        while (occurence != std::string::npos) {
            found_numbers.push_back({occurence, i + 1});
            occurence = line.find(numbers[i], occurence + 1);
        }
    }


    std::vector<int> sorted_numbers;
    for(int i = 0; i < line.size(); ++i) {
        for(int j = 0; j < found_numbers.size(); ++j) {
            if(found_numbers[j][0] == i) {
                sorted_numbers.push_back(found_numbers[j][1]);
                found_numbers.erase(found_numbers.begin() + j);
            }
        }
    }

    if(sorted_numbers.size() == 1) {
        return std::stoi(std::to_string(sorted_numbers[0]) + std::to_string(sorted_numbers[0]));
    } else {
        return std::stoi(std::to_string(sorted_numbers[0]) + std::to_string(sorted_numbers[sorted_numbers.size() - 1]));
    }
}

auto main(int argc, char* argv[]) -> int {
    int result = 0;
    std::string line;

    if(argc != 2) {
        std::cout << "invalid use" << std::endl;
        return 1;
    }
    std::string filename;
    if(*argv[1] == '1') {
        filename = "./two.input.txt";
    } else {
        filename = "./two.sample.txt";
    }

    std::ifstream file(filename);
    if(file.is_open()) {
        while(std::getline(file, line)) {
            result += find_number(line);
        }
        file.close();
    }

    std::cout << "Result: " << result << std::endl;
    return 0;
}