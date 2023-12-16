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
	string hand;
	uint64_t bid;

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

auto exists(vector<int> vec, int target) -> bool {
	return find(vec.begin(), vec.end(), target) != vec.end();
}

auto get_relative_strength(char c) -> uint8_t {
	switch (c) {
	case 'J' :
		return 1;
	case '2' :
		return 2;
	case '3' :
		return 3;
	case '4' :
		return 4;
	case '5' :
		return 5;
	case '6' :
		return 6;
	case '7' :
		return 7;
	case '8' :
		return 8;
	case '9' :
		return 9;
	case 'T' :
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
		exit(1);
		break;
	}
}

auto get_strength(const string& hand) -> Strength {
	Strength max_hand_strength = HIGH_CARD;

	for (auto face : "23456789TQKA") {
		string possible_hand = hand;
		replace(possible_hand.begin(), possible_hand.end(), 'J', face);

		Strength hand_strength;

		vector<int32_t> occurrence;
		map<char, int32_t> count_occurrence;

		for (auto c : possible_hand) {
			++count_occurrence[c];
		}

		for (auto p : count_occurrence) {
			occurrence.push_back(p.second);
		}

		if (exists(occurrence, 5)) {
			hand_strength = FIVE_OF_A_KIND;
		} else if (exists(occurrence, 4)) {
			hand_strength = FOUR_OF_A_KIND;
		} else if (exists(occurrence, 3) && exists(occurrence, 2)) {
			hand_strength = FULL_HOUSE;
		} else if (exists(occurrence, 3)) {
			hand_strength = THREE_OF_A_KIND;
		} else if (count(occurrence.begin(), occurrence.end(), 2) == 2) {
			hand_strength = TWO_PAIR;
		} else if (exists(occurrence, 2)) {
			hand_strength = ONE_PAIR;
		} else {
			hand_strength = HIGH_CARD;
		}
		max_hand_strength = max(max_hand_strength, hand_strength);
	}
	return max_hand_strength;
}

auto compare(const Hand& c1, const Hand& c2) -> bool {
	string hand1 = c1.hand;
	string hand2 = c2.hand;

	if (get_strength(hand1) != get_strength(hand2)) {
		return get_strength(hand1) < get_strength(hand2);
	} else {
		for (int i = 0; i < 5; ++i) {
			if (hand1[i] != hand2[i])
				return get_relative_strength(hand1[i]) < get_relative_strength(hand2[i]);
		}
		return false;
	}
}

auto main(int argc, char* argv[]) -> int {
	if (argc != 2) {
		cerr << "Invalid Useage\n";
		return 1;
	}

	string file_name;
	if (*argv[1] == '1') {
		file_name = "sample.txt";
	} else {
		file_name = "input.txt";
	}

	ifstream file(file_name);
	string line;
	vector<string> input;
	while (getline(file, line)) {
		input.push_back(line);
	}

	vector<Hand> cards_hand;

	for (const string& card_hand : input) {
		stringstream ss(card_hand);

		string hand;
		ss >> hand;
		string bid_str;
		ss >> bid_str;
		uint64_t bid = stoll(bid_str);
		cards_hand.emplace_back(bid, hand);
	}
	sort(cards_hand.begin(), cards_hand.end(), compare);

	uint64_t total_winnings = 0;
	for (int i = 0; i < cards_hand.size(); ++i) {
		total_winnings += (i + 1) * cards_hand[i].bid;
	}

	cout << "Solution: " << total_winnings << '\n';

	return 0;
}