#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

enum Direction {
	RIGHT,
	DOWN,
	LEFT,
	UP
};

int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

auto getAdj(int x, int y, Direction dir, int steps)
	-> std::vector<std::tuple<int64_t, int64_t, int64_t, Direction>> {
	std::vector<std::tuple<int64_t, int64_t, int64_t, Direction>> ret;

	if (steps < 10) ret.push_back({x + dx[dir], y + dy[dir], steps + 1, dir});
	if (steps >= 4) {
		int d = (dir + 1) % 4;
		ret.push_back({x + dx[d], y + dy[d], 1, static_cast<Direction>(d)});

		d = (dir + 3) % 4;
		ret.push_back({x + dx[d], y + dy[d], 1, static_cast<Direction>(d)});
	}
	return ret;
}

auto dijkstra(const std::vector<std::string>& input)
	-> std::vector<std::vector<std::vector<std::vector<int64_t>>>> {
	std::set<std::tuple<int64_t, int64_t, int64_t, int64_t, Direction>> S;

	int maxStepsInOneDirection = 10;

	std::vector<std::vector<std::vector<std::vector<int64_t>>>> dist(input.size(),
		std::vector<std::vector<std::vector<int64_t>>>(input[0].size(),
			std::vector<std::vector<int64_t>>(
				maxStepsInOneDirection + 1, std::vector<int64_t>(4, INT64_MAX))));

	for (auto dir : {UP, DOWN, LEFT, RIGHT}) {
		S.insert(std::make_tuple(0, 0, 0, 0, dir));
		dist[0][0][0][dir] = 0;
	}

	while (!S.empty()) {
		auto [d, x, y, steps, dir] = *S.begin();
		S.erase(S.begin());

		if (d > dist[x][y][steps][dir]) continue;

		for (auto [nx, ny, nsteps, ndir] : getAdj(x, y, dir, steps)) {
			if (nx < 0 || nx >= input.size() || ny < 0 || ny >= input[0].size()) continue;

			if (nsteps > maxStepsInOneDirection) continue;

			int weight = input[nx][ny] - '0';
			if (dist[nx][ny][nsteps][ndir] > dist[x][y][steps][dir] + weight) {
				dist[nx][ny][nsteps][ndir] = dist[x][y][steps][dir] + weight;
				S.insert({dist[nx][ny][nsteps][ndir], nx, ny, nsteps, ndir});
			}
		}
	}

	return dist;
}

auto solve(const std::vector<std::string>& input) -> int64_t {
	int64_t result = INT64_MAX;

	std::vector<std::vector<std::vector<std::vector<int64_t>>>> dist = dijkstra(input);

	for (auto dir : {UP, DOWN, LEFT, RIGHT})
		for (int steps = 4; steps <= 10; ++steps)
			result = std::min(result, dist[input.size() - 1][input[0].size() - 1][steps][dir]);

	return result;
}

auto main(int argc, char* argv[]) -> int {
	if (argc <= 1) {
		std::cerr << "Invalid Useage\n";
		return 1;
	}

	std::string file_name;
	if (*argv[1] == '1') {
		file_name = "sample.txt";
	} else if (*argv[1] == '2') {
		file_name = "sample2.txt";
	} else {
		file_name = "input.txt";
	}

	std::ifstream file(file_name);
	std::string line;

	std::vector<std::string> input;
	while (std::getline(file, line)) {
		input.push_back(line);
	}

	std::cout << "Solution: " << solve(input) << std::endl;
}
