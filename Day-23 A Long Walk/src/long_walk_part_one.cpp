#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define FILE_NOT_FOUND 1

template<typename T>
auto print_vec(const std::vector<T>& vec) -> void {
	for (const T& item : vec) {
		std::cout << item << '\n';
	}
}

[[nodiscard]] auto to_string_vec(std::ifstream& fin) -> std::vector<std::string> {
	if (!fin) {
		std::cerr << "File not found\n";
		std::exit(FILE_NOT_FOUND);
	}

	std::vector<std::string> vec;
	std::string line;
	while (std::getline(fin, line)) {
		vec.push_back(line);
	}
	return vec;
}

using Pos = std::array<int64_t, 2>;

template<>
struct std::hash<Pos> {
	size_t operator()(const Pos& s) const noexcept {
		size_t h1 = std::hash<int> {}(s[0]);
		size_t h2 = std::hash<int> {}(s[1]);
		return h1 ^ (h2 << 1);
	}
};

template<typename T>
[[nodiscard]] auto contains(const std::unordered_set<T>& set, const T& target) -> bool {
	// https://stackoverflow.com/questions/17016175/why-is-stdunordered-setmissing-contains-method
	return set.find(target) != set.end();
}

auto getLongestPath(const Pos& startPos,
	int currSteps,
	const std::vector<std::string>& map,
	std::unordered_set<Pos> visited,
	std::unordered_map<Pos, int>& maxDistances) -> void {
	const std::array<Pos, 4> directions {
		{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}
	  };

	visited.insert(startPos);
	if (currSteps < maxDistances[startPos]) {
		return;
	}

	maxDistances[startPos] = currSteps;

	for (const auto& direction : directions) {
		Pos newPos {startPos[0] + direction[0], startPos[1] + direction[1]};
		if (newPos[0] < 0 || newPos[1] < 0 || newPos[0] > map.size() - 1 ||
			newPos[1] > map.front().size() - 1 || contains(visited, newPos)) {
			continue;
		}

		char tile {map[newPos[0]][newPos[1]]};
		switch (tile) {
		case '.' :
			getLongestPath(newPos, currSteps + 1, map, visited, maxDistances);
			break;
		case 'v' :
			newPos[0] += directions[0][0];
			newPos[1] += directions[0][1];
			if (!contains(visited, newPos)) {
				getLongestPath(newPos, currSteps + 2, map, visited, maxDistances);
			}
			break;
		case '>' :
			newPos[0] += directions[1][0];
			newPos[1] += directions[1][1];
			if (!contains(visited, newPos)) {
				getLongestPath(newPos, currSteps + 2, map, visited, maxDistances);
			}
			break;
		case '^' :
			newPos[0] += directions[2][0];
			newPos[1] += directions[2][1];
			if (!contains(visited, newPos)) {
				getLongestPath(newPos, currSteps + 2, map, visited, maxDistances);
			}
			break;
		case '<' :
			newPos[0] += directions[3][0];
			newPos[1] += directions[3][1];
			if (!contains(visited, newPos)) {
				getLongestPath(newPos, currSteps + 2, map, visited, maxDistances);
			}
			break;
		}
	}
}

auto main(int argc, char* argv[]) -> int {
	if (argc < 2) {
		std::cerr << "Invalid Useage\n";
		return 1;
	}

	std::string file_name;

	if (*argv[1] == '1') {
		file_name = "sample.txt";
	} else {
		file_name = "input.txt";
	}

	std::ifstream file(file_name);
	auto map = to_string_vec(file);

#ifdef Debug
	print_vec(map);
#endif

	int64_t i {0};

	while (i < map[0].size()) {
		if (map[0][i] == '.') {
			break;
		}

		++i;
	}

	Pos startPos {0, i};

	i = 0;
	while (i < map.back().size()) {
		if (map.back()[i] == '.') {
			break;
		}
		++i;
	}

	Pos endPos {static_cast<int>(map.size()) - 1, i};
	int currSteps {0};
	std::unordered_set<Pos> visited;
	std::unordered_map<Pos, int> maxDistances;
	getLongestPath(startPos, currSteps, map, visited, maxDistances);

	std::cout << "Longest path distance: " << maxDistances.at(endPos) << "\n";

	return 0;
}
