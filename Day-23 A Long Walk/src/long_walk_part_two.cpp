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

struct Edge {
	int cost;
	Pos pos;
};

struct Junction {
	Pos pos;
	std::vector<Edge> edges;
};

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

const std::array<Pos, 4> directions {
	{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}
};

bool isJunction(const Pos& pos, const std::vector<std::string>& map) {
	if (map[pos[0]][pos[1]] == '#') {
		return false;
	}

	int numPaths {0};
	for (const auto& direction : directions) {
		Pos newPos {pos[0] + direction[0], pos[1] + direction[1]};
		if (newPos[0] < 0 || newPos[1] < 0 || newPos[0] > map.size() - 1 ||
			newPos[1] > map.front().size() - 1) {
			continue;
		}

		if (map[newPos[0]][newPos[1]] != '#') {
			++numPaths;
		}
	}

	return numPaths > 2;
}

Edge getEdge(Pos pos,
	const Pos& startPos,
	const Pos& endPos,
	const std::vector<std::string>& map,
	std::unordered_set<Pos>& visited) {
	int cost {1};
	while (!isJunction(pos, map)) {
		if (pos == startPos || pos == endPos) {
			return {cost, pos};
		}

		visited.insert(pos);

		bool isDeadEnd {true};
		for (const auto& direction : directions) {
			Pos newPos {pos[0] + direction[0], pos[1] + direction[1]};
			if (newPos[0] < 0 || newPos[1] < 0 || newPos[0] > map.size() - 1 ||
				newPos[1] > map.front().size() - 1 || map[newPos[0]][newPos[1]] == '#' ||
				contains(visited, newPos)) {
				continue;
			}

			pos = newPos;
			++cost;
			isDeadEnd = false;
			break;
		}

		if (isDeadEnd) {
			return {0, pos};
		}
	}

	return {cost, pos};
}

std::vector<Edge> getEdges(
	const Pos& pos, const Pos& startPos, const Pos& endPos, const std::vector<std::string>& map) {
	std::vector<Edge> edges;

	for (const auto& direction : directions) {
		Pos newPos {pos[0] + direction[0], pos[1] + direction[1]};
		if (newPos[0] < 0 || newPos[1] < 0 || newPos[0] > map.size() - 1 ||
			newPos[1] > map.front().size() - 1 || map[newPos[0]][newPos[1]] == '#') {
			continue;
		}

		std::unordered_set<Pos> visited;
		visited.insert(pos);
		Edge edge {getEdge(newPos, startPos, endPos, map, visited)};
		if (edge.cost != 0) {
			edges.push_back(edge);
		}
	}

	return edges;
}

std::unordered_map<Pos, Junction> getJunctions(
	const std::vector<std::string>& map, const Pos& startPos, const Pos& endPos) {
	std::unordered_map<Pos, Junction> junctions;

	for (int i = 0; i < map.size(); ++i) {
		for (int j = 0; j < map[i].size(); ++j) {
			if (isJunction({i, j}, map) || Pos {i, j} == startPos || Pos {i, j} == endPos) {
				std::vector<Edge> edges = getEdges({i, j}, startPos, endPos, map);
				junctions[{
					i, j
				}]				= {{i, j}, edges};
			}
		}
	}

	return junctions;
}

// There is probs a faster way but this gets done in like 2min...
void getLongestPath(const Pos& startPos,
	int currSteps,
	std::unordered_map<Pos, Junction>& junctions,
	std::unordered_set<Pos> visited,
	std::unordered_map<Pos, int>& maxDistances) {
	visited.insert(startPos);
	maxDistances[startPos] = std::max(currSteps, maxDistances[startPos]);

	for (const auto& edge : junctions.at(startPos).edges) {
		if (!contains(visited, edge.pos)) {
			getLongestPath(edge.pos, currSteps + edge.cost, junctions, visited, maxDistances);
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

	std::unordered_map<Pos, Junction> junctions {getJunctions(map, startPos, endPos)};

	int currSteps {0};
	std::unordered_set<Pos> visited;
	std::unordered_map<Pos, int> maxDistances;
	getLongestPath(startPos, currSteps, junctions, visited, maxDistances);

	std::cout << "Longest path distance: " << maxDistances.at(endPos) << "\n";

	return 0;
}
