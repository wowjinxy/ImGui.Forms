// Point.h
#pragma once
#include <imgui.h>

namespace ImGuiForms {
    struct Point {
        float x, y;

        Point() : x(0.0f), y(0.0f) {}
        Point(float x, float y) : x(x), y(y) {}

        // ImGui conversion
        ImVec2 ToImVec2() const { return ImVec2(x, y); }
        static Point FromImVec2(const ImVec2& vec) { return Point(vec.x, vec.y); }

        // Basic operations
        Point operator+(const Point& other) const { return Point(x + other.x, y + other.y); }
        Point operator-(const Point& other) const { return Point(x - other.x, y - other.y); }
        Point operator*(float factor) const { return Point(x * factor, y * factor); }

        bool operator==(const Point& other) const { return x == other.x && y == other.y; }
        bool operator!=(const Point& other) const { return !(*this == other); }
    };
}