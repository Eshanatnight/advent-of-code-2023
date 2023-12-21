#include <array>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_map>
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

template<typename T>
auto contains(const std::unordered_set<T>& set, const T& target) -> bool {
	// https://stackoverflow.com/questions/17016175/why-is-stdunordered-setmissing-contains-method
	return set.find(target) != set.end();
}

auto get_map(std::ifstream& inf, std::vector<std::string>& map) -> Position {
	std::string line;
	Position pos;

	int row {0};
	while (inf >> line) {
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

auto bfs(std::unordered_set<Position>& startPositions,
	std::unordered_set<Position>& visited,
	std::vector<std::string>& map) -> void {
	std::unordered_set<Position> nextPositions;

	int iSize {static_cast<int>(map.size())};
	int jSize {static_cast<int>(map.front().size())};

	for (const auto& pos : startPositions) {
		std::array<Position, 4> deltas {
			{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}
		  };
		for (const auto& delta : deltas) {
			Position checkPos {(((pos[0] + delta[0]) % iSize) + iSize) % iSize,
				(((pos[1] + delta[1]) % jSize) + jSize) % jSize};
			auto does_contain = contains<Position>(visited, {pos[0] + delta[0], pos[1] + delta[1]});
			if (map[checkPos[0]][checkPos[1]] != '#' && !does_contain) {
				nextPositions.insert({pos[0] + delta[0], pos[1] + delta[1]});
				visited.insert({pos[0] + delta[0], pos[1] + delta[1]});
			}
		}
	}

	startPositions = nextPositions;
}

auto main(int argc, char* argv[]) -> int {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>\n";
		return 1;
	}

	std::string filename;
	if (*argv[1] == '1') {
		filename = "sample.txt";
	} else {
		filename = "input.txt";
	}

	std::ifstream inf {filename};

	if (!inf) {
		std::cerr << "Could not open file: " << argv[1] << "\n";
		return 2;
	}

	std::vector<std::string> map;
	Position startPos {get_map(inf, map)};
	std::unordered_set<Position> startPositions;
	startPositions.insert(startPos);
	std::unordered_set<Position> visited;
	visited.insert(startPos);
	std::unordered_map<Position, int> offsets;
	offsets[startPos] = 0;

	int steps {26501365};
	// it's quadratic because the middle row/column is empty so you can get to
	// the same start position by moving exactly the length of the map
	// From there you can go the same distance as found with bfs to get to
	// identical position but in a repeated board
	//
	// Since you can go up or right (or down or left) this creates a square number
	// of repeated boards that you can reach
	//
	// Therefore after n steps we could reach all the same positions as walking
	// n mod(board_len) steps.
	//
	// If we note the number of reachable steps each time this happens for the
	// first three times then we can solve the quadratic
	std::vector<int> params;
	std::vector<int> lens;
	lens.push_back(1);

	for (int i = 0; i < steps; ++i) {
		bfs(startPositions, visited, map);
		lens.push_back(startPositions.size());
		// we have done n mod(board_len) steps
		if (i % map.size() == steps % map.size()) {
			int len {0};
			// we want the squares just explored not the ones we are about to explore
			for (int j = 0; j < lens.size() - 1; ++j) {
				// We can find how many others we could get to by noting that you can
				// always get back to the same square in an even number of moves
				//
				// Therefore, in the bfs after n iterations you could also have reached
				// the positions at step i where i % 2 == n % 2. This is faster than
				// repeating already explored squares in the bfs
				if (j % 2 == i % 2) {
					len += lens[j];
				}
			}
			params.push_back(len);
		}

		if (params.size() == 3) {
			break;
		}
	}

	long long p1 {params[0]};
	long long p2 {params[1] - params[0]};
	long long p3 {params[2] - params[1]};
	long long ip {steps / static_cast<long long>(map.size())};

	// solve the quadratic
	long long totalSteps {p1 + p2 * ip + (ip * (ip - 1) / 2) * (p3 - p2)};
	std::cout << "Number of positions: " << totalSteps << "\n";

	return 0;
}
