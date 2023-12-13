#pragma once

#include <cstdint>
#include <fstream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

auto read_file_to_string(const std::string& path) -> std::string;
auto to_string_vec(std::string_view str) -> std::vector<std::string>;
auto to_string_vec(std::ifstream fin) -> std::vector<std::string>;
auto split(const std::string& str, char delimeter = ' ') -> std::vector<std::string>;

struct Point2D {
	int64_t x;
	int64_t y;

	constexpr explicit Point2D(int x, int y) : x(x), y(y) {}
	constexpr explicit Point2D() : x(0), y(0) {}
	constexpr explicit Point2D(const Point2D& other) : x(other.x), y(other.y) {}
	constexpr explicit Point2D(Point2D&& other) : x(other.x), y(other.y) {}
	auto operator=(const Point2D& other) -> Point2D& {
		x = other.x;
		y = other.y;
		return *this;
	}
	auto operator=(Point2D&& other) -> Point2D& {
		x = other.x;
		y = other.y;
		return *this;
	}
	auto operator==(const Point2D& other) const -> bool {
		return x == other.x && y == other.y;
	}
	auto operator!=(const Point2D& other) const -> bool {
		return !(*this == other);
	}
	auto operator<(const Point2D& other) const -> bool {
		return x < other.x || (x == other.x && y < other.y);
	}
	auto operator>(const Point2D& other) const -> bool {
		return x > other.x || (x == other.x && y > other.y);
	}
	auto operator<=(const Point2D& other) const -> bool {
		return *this < other || *this == other;
	}
	auto operator>=(const Point2D& other) const -> bool {
		return *this > other || *this == other;
	}
	auto operator+(const Point2D& other) const -> Point2D {
		return Point2D(x + other.x, y + other.y);
	}
	auto operator-(const Point2D& other) const -> Point2D {
		return Point2D(x - other.x, y - other.y);
	}
	auto operator+=(const Point2D& other) -> Point2D& {
		x += other.x;
		y += other.y;
		return *this;
	}
	auto operator-=(const Point2D& other) -> Point2D& {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	auto operator*(int scalar) const -> Point2D {
		return Point2D(x * scalar, y * scalar);
	}
	auto operator/(int scalar) const -> Point2D {
		return Point2D(x / scalar, y / scalar);
	}
	auto operator*=(int scalar) -> Point2D& {
		x *= scalar;
		y *= scalar;
		return *this;
	}
	auto operator/=(int scalar) -> Point2D& {
		x /= scalar;
		y /= scalar;
		return *this;
	}

	[[nodiscard]] auto manhattan_distance(const Point2D& other) const -> int64_t {
		return std::abs(x - other.x) + std::abs(y - other.y);
	}
};
