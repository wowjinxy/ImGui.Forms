#pragma once
#include <imgui.h>
#include <string>
#include <cmath>

namespace ImGuiForms {

    /**
     * @brief Represents a 2D point with x and y coordinates
     */
    struct Point {
        float x, y;

        // Constructors
        Point() : x(0.0f), y(0.0f) {}
        Point(float x, float y) : x(x), y(y) {}

        // ImGui conversion methods
        ImVec2 ToImVec2() const {
            return ImVec2(x, y);
        }

        static Point FromImVec2(const ImVec2& vec) {
            return Point(vec.x, vec.y);
        }

        // Implicit conversion to ImVec2 for convenience
        operator ImVec2() const {
            return ImVec2(x, y);
        }

        // Basic operations
        Point operator+(const Point& other) const {
            return Point(x + other.x, y + other.y);
        }

        Point operator-(const Point& other) const {
            return Point(x - other.x, y - other.y);
        }

        Point operator*(float factor) const {
            return Point(x * factor, y * factor);
        }

        Point operator/(float factor) const {
            if (factor != 0.0f) {
                return Point(x / factor, y / factor);
            }
            return Point();
        }

        // Comparison operators
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Point& other) const {
            return !(*this == other);
        }

        // Utility methods
        float DistanceTo(const Point& other) const {
            float dx = x - other.x;
            float dy = y - other.y;
            return std::sqrt(dx * dx + dy * dy);
        }

        float DistanceSquaredTo(const Point& other) const {
            float dx = x - other.x;
            float dy = y - other.y;
            return dx * dx + dy * dy;
        }

        Point Normalize() const {
            float length = std::sqrt(x * x + y * y);
            if (length > 0.0f) {
                return Point(x / length, y / length);
            }
            return Point();
        }

        float Length() const {
            return std::sqrt(x * x + y * y);
        }

        float LengthSquared() const {
            return x * x + y * y;
        }

        // String representation
        std::string ToString() const {
            return "Point(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }
    };

} // namespace ImGuiForms