#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <array>

#ifndef DEBUG
    #define DEBUG 1
#endif

using namespace std;

struct RangeMap {
    uint64_t source_start;
    uint64_t dest_start;
    uint64_t range;
};

struct Range {
    uint64_t start;
    uint64_t range;
};

auto is_in_range(uint64_t src, RangeMap range) -> bool {
    return range.source_start <= src && range.source_start + range.range > src;
}

auto find_dest_in_range(const array<vector<RangeMap>, 7>& ranges, uint64_t src, int range) -> uint64_t {
    for(int i= 0; i < ranges.at(range).size(); ++i) {
        if(is_in_range(src, ranges[range][i])) {
            return ranges[range][i].dest_start +
                (src - ranges[range][i].source_start);
        }
    }

    return src;
}

auto convert_seed_to_location(array<vector<RangeMap>, 7>& ranges, uint64_t seed) -> uint64_t{
    for(int i = 0; i < 7; ++i) {
        seed = find_dest_in_range(ranges, seed, i);
    }

    return seed;
}

auto solve(const string& file_name) -> uint64_t {
    ifstream file(file_name);
    
    vector<uint64_t> seeds;
    string line;
    char* number_size;
    array<vector<RangeMap>, 7> ranges;

    getline(file, line);
    line = line.substr(7);
    while(line.length() > 0) {
        #if DEBUG
            cout << line << endl;
        #endif
        seeds.push_back(strtoll(line.c_str(), &number_size, 10));        
        line = line.substr(number_size - line.c_str());
    }

    getline(file, line);
    getline(file, line);

    int i{0};
    while(getline(file, line)) {
        if(line == "") {
            getline(file, line);
            i++;
            continue;
        }

        RangeMap range;

        range.dest_start = strtoll(line.c_str(), &number_size, 10);
        line = line.substr(number_size - line.c_str());
        range.source_start = strtoll(line.c_str(), &number_size, 10);
        line = line.substr(number_size - line.c_str());
        range.range = strtoll(line.c_str(), &number_size, 10);
        line = line.substr(number_size - line.c_str());
        ranges[i].push_back(range);
    }

    auto lowest = INT64_MAX;
    for(int i = 0; i < seeds.size(); ++i) {
        uint64_t location = convert_seed_to_location(ranges, seeds[i]);
        lowest = location < lowest ? location : lowest;
    }

    return lowest;
}


auto main(int argc, char* argv[]) -> int {
    if(argc != 2) {
        cout << "Invalid Useage" << endl;
        return 1;
    }

    string file_name;

    if(*argv[1] == '1') {
        file_name = "./part_one.sample.txt";
    } else {
        file_name = "./part_one.input.txt";
    }

    cout << "Result: " << solve(file_name) << endl;


    return 0;
}
