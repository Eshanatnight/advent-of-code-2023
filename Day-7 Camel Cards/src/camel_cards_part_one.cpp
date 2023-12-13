#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Hand {
	uint64_t bid;
	string hand;

	Hand(uint64_t bid, string hand) : bid(bid), hand(move(hand)) {}
};

enum Strength {
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	FIVE_OF_A_KIND
};

auto exists(const vector<int32_t>& v, int32_t x) -> bool {
	return find(v.begin(), v.end(), x) != v.end();
}

auto get_relative_strength(char c) -> uint8_t {
	switch (c) {
	case '2' :
		return 1;
	case '3' :
		return 2;
	case '4' :
		return 3;
	case '5' :
		return 4;
	case '6' :
		return 5;
	case '7' :
		return 6;
	case '8' :
		return 7;
	case '9' :
		return 8;
	case 'T' :
		return 9;
	case 'J' :
		return 10;
	case 'Q' :
		return 11;
	case 'K' :
		return 12;
	case 'A' :
		return 13;
	default :
		cerr << "Invalid card value: " << c << endl;
		cerr << "Exiting" << endl;
		std::exit(1);
		break;
	}
}

auto get_strength(const string& hand) -> Strength {
	vector<int32_t> occurences;
	map<char, int32_t> count_occurences;

	for (auto c : hand) {
		++count_occurences[c];
	}

	for (auto p : count_occurences) {
		occurences.push_back(p.second);
	}

	if (exists(occurences, 5)) {
		return FIVE_OF_A_KIND;
	} else if (exists(occurences, 4)) {
		return FOUR_OF_A_KIND;
	} else if (exists(occurences, 3) && exists(occurences, 2)) {
		return FULL_HOUSE;
	} else if (exists(occurences, 3)) {
		return THREE_OF_A_KIND;
	} else if (count(occurences.begin(), occurences.end(), 2) == 2) {
		return TWO_PAIR;
	} else if (exists(occurences, 2)) {
		return ONE_PAIR;
	} else {
		return HIGH_CARD;
	}
}

auto compare(const Hand& c1, const Hand& c2) -> bool {

	string hand1 = c1.hand;
	string hand2 = c2.hand;

	if (get_strength(hand1) != get_strength(hand2)) {
		return get_strength(hand1) < get_strength(hand2);
	} else {
		for (int i = 0; i < 5; ++i) {
			if (hand1[i] != hand2[i]) {
				return get_relative_strength(hand1[i]) < get_relative_strength(hand2[i]);
			}
		}

		return false;
	}
}

auto main(int argc, char* argv[]) -> int {

	if (argc != 2) {
		cerr << "Invalid Useage" << endl;
		return 1;
	}

	string file_name;
	if (*argv[1] == '1') {
		file_name = "./one.sample.txt";
	} else {
		file_name = "./one.input.txt";
	}

	ifstream file(file_name);

	if (!file.is_open()) {
		cerr << "Could not open file: " << file_name << endl;
		return 1;
	}

	string line;
	vector<string> lines;
	while (getline(file, line)) {
		lines.push_back(line);
	}

	vector<Hand> hands;

	for (const auto& line : lines) {
		stringstream ss(line);

		string hand;
		ss >> hand;

		string _bid;
		ss >> _bid;

		uint64_t bid = stoull(_bid);

		hands.emplace_back(bid, hand);
	}

	sort(hands.begin(), hands.end(), compare);

	uint64_t total = 0;

	for (int i = 0; i < hands.size(); ++i) {
		total += hands[i].bid * (i + 1);
	}

	cout << "Solution: " << total << endl;
}