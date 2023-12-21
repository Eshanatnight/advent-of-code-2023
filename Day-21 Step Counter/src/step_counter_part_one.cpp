#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <vector>

using Position = std::array<int, 2>;

template<>
struct std::hash<Position> {
	size_t operator()(const Position& s) const noexcept {
		size_t h1 = std::hash<int> {}(s[0]);
		size_t h2 = std::hash<int> {}(s[1]);
		return h1 ^ (h2 << 1);
	}
};

auto get_map(std::ifstream& fin, std::vector<std::string>& map) -> Position {
	std::string line;
	Position pos;

	int row {0};
	while (fin >> line) {
		map.push_back(line);
		size_t startCol {line.find('S')};
		if (startCol != std::string::npos) {
			pos[0]			   = row;
			pos[1]			   = startCol;
			map[row][startCol] = '.';
		}
		++row;
	}

	return pos;
}

auto update_position(
	std::unordered_set<Position>& startPositions, const std::vector<std::string>& map) -> void {
	std::unordered_set<Position> nextPositions {};

	for (const auto& pos : startPositions) {
		if (pos[0] > 0 && map[pos[0] - 1][pos[1]] != '#') {
			nextPositions.insert({pos[0] - 1, pos[1]});
		}

		if (pos[0] < map.size() - 1 && map[pos[0] + 1][pos[1]] != '#') {
			nextPositions.insert({pos[0] + 1, pos[1]});
		}

		if (pos[1] > 0 && map[pos[0]][pos[1] - 1] != '#') {
			nextPositions.insert({pos[0], pos[1] - 1});
		}

		if (pos[1] < map[pos[0]].size() - 1 && map[pos[0]][pos[1] + 1] != '#') {
			nextPositions.insert({pos[0], pos[1] + 1});
		}
	}

	startPositions = nextPositions;
}

auto main(int argc, char* argv[]) -> int {
	if (argc != 2) {
		std::cerr << "Invalid Usage\n";

		std::cerr << "Usage: " << argv[0] << " <filename>\n";
		return 1;
	}
	std::string file_name;

	if (*argv[1] == '1') {
		file_name = "sample.txt";
	} else {
		file_name = "input.txt";
	}

	std::ifstream fin {file_name};

	if (!fin) {
		std::cerr << "Could not open file: " << argv[1] << "\n";
		return 2;
	}

	std::vector<std::string> map;
	Position startPos {get_map(fin, map)};
	std::unordered_set<Position> startPositions;
	startPositions.insert(startPos);

	int steps {64};
	while (steps--) {
		update_position(startPositions, map);
	}

	std::cout << "Number of positions: " << startPositions.size() << "\n";

	return 0;
}