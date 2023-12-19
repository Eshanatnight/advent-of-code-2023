#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

constexpr std::array<char, 4> ruleIndex {'x', 'm', 'a', 's'};

struct Rule {
	int index;
	int value;
	bool less;
	std::string target;

	bool evaluate(const std::array<int, 4>& p) const {
		return less ? p[index] < value : p[index] > value;
	}
};

struct WorkFlow {
	std::vector<Rule> rules;
	std::string allFalse;

	void process_rule(const std::string s) {
		std::stringstream ss(s);
		std::string line;
		while (std::getline(ss, line, ',')) {
			auto p = line.find(':');
			if (p == std::string::npos) {
				allFalse = line;
				break;
			}

			Rule r;
			r.target = line.substr(p + 1);
			line	 = line.substr(0, p);

			r.index = std::distance(
				ruleIndex.cbegin(), std::find(ruleIndex.cbegin(), ruleIndex.cend(), line[0]));
			r.less	= line[1] == '<';
			r.value = stoi(line.substr(2));

			rules.push_back(r);
		}
	}

	const std::string& get_next(const std::array<int, 4>& p) const {
		for (const auto& r : rules) {
			if (r.evaluate(p)) {
				return r.target;
			}
		}
		return allFalse;
	}
};

template<typename T>
class y_combinator
{
	T lambda;

public:

	constexpr y_combinator(T&& t) : lambda(std::forward<T>(t)) {}

	template<typename... Args>
	constexpr decltype(auto) operator()(Args&&... args) const {
		return lambda(std::move(*this), std::forward<Args>(args)...);
	}
};

auto solve(std::ifstream& file) -> int64_t {
	int64_t result = 0;
	std::string line;
	std::map<std::string, WorkFlow> workflows;

	while (std::getline(file, line)) {
		if (line.empty()) {
			break;
		}

		WorkFlow w;
		auto p			 = line.find('{');
		std::string name = line.substr(0, p);
		line			 = line.substr(p + 1, line.size() - p - 2);
		w.process_rule(line);
		workflows[name] = w;
	}

	while (std::getline(file, line)) {
		line = line.substr(1, line.size() - 2);
		std::stringstream ss(line);
		std::array<int, 4> p = {0, 0, 0, 0};
		for (int& i : p) {
			std::string s;
			std::getline(ss, s, ',');
			i = stoi(s.substr(2));
		}

		std::string at = "in";
		while (at != "A" && at != "R") {
			at = workflows.find(at)->second.get_next(p);
		}

		if (at == "A") {
			for (int i : p) {
				result += i;
			}
		}
	}

	auto GetCombinations =
		y_combinator([&workflows](auto GetCombinations,
						 const std::string& ruleName,
						 std::array<std::pair<int64_t, int64_t>, 4> vr) -> int64_t {
			if (ruleName == "R") {
				return 0;
			}
			if (ruleName == "A") {
				return std::accumulate(vr.cbegin(),
					vr.cend(),
					1ll,
					[](int64_t a, const std::pair<int64_t, int64_t>& r) {
						return a * (r.second - r.first + 1);
					});
			}

			int64_t result	  = 0;
			const WorkFlow& w = workflows.find(ruleName)->second;

			for (const Rule& r : w.rules) {
				std::array<std::pair<int64_t, int64_t>, 4> split = vr;
				if (r.less) {
					split[r.index].second = r.value - 1;
					vr[r.index].first	  = r.value;
					result += GetCombinations(r.target, split);
				} else {
					split[r.index].first = r.value + 1;
					vr[r.index].second	 = r.value;
					result += GetCombinations(r.target, split);
				}
			}

			return result + GetCombinations(w.allFalse, vr);
		});

	constexpr std::array<std::pair<int64_t, int64_t>, 4> initial = {std::make_pair(1ll, 4000ll),
		std::make_pair(1ll, 4000ll),
		std::make_pair(1ll, 4000ll),
		std::make_pair(1ll, 4000ll)};

	return GetCombinations("in", initial);
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
	if (!file) {
		std::cerr << "Error opening input file\n";
		return 1;
	}

	std::cout << "Solution: " << solve(file) << '\n';
}
