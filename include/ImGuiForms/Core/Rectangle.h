// Rectangle.h
#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>
#include <memory>  // For std::unique_ptr
#include <imgui.h>
#include "Size.h"
#include "Point.h"

namespace ImGuiForms {
    class Rectangle {
    public:
        float x, y, width, height;
            // Constructors - DON'T define bodies here, only in .cpp
            Rectangle();
            Rectangle(float x, float y, float width, float height);
            Rectangle(const Point& location, const Size& size);
    
            // Properties - DON'T define bodies here, only in .cpp
            bool IsEmpty() const;
            float Area() const;
            float AspectRatio() const;
            float Perimeter() const;
            Point Center() const;
    
            // Simple accessors can be inline
            float Left() const { return x; }
            float Right() const { return x + width; }
            float Top() const { return y; }
            float Bottom() const { return y + height; }
    
            // Add missing properties that your code expects
            Point TopLeft() const { return Point(x, y); }
            Point TopRight() const { return Point(x + width, y); }
            Point BottomLeft() const { return Point(x, y + height); }
            Point BottomRight() const { return Point(x + width, y + height); }
            bool Contains(const Point& point) const;
    
            // Static factory methods - DECLARE ONLY, implement in .cpp
            static Rectangle FromTwoPoints(const Point& p1, const Point& p2);
            static Rectangle FromCenter(const Point& center, const Size& size);
    
            // Operations - DECLARE ONLY, implement in .cpp
            Rectangle Union(const Rectangle& other) const;
            Rectangle Intersection(const Rectangle& other) const;
            bool ContainsPoint(const Point& point) const;
            bool ContainsRectangle(const Rectangle& other) const;
    
            Rectangle Scale(float factor) const;
            Rectangle Scale(float xFactor, float yFactor) const;
    
            // Alignment methods - DECLARE ONLY
            Rectangle AlignLeft(const Rectangle& container) const;
            Rectangle AlignRight(const Rectangle& container) const;
            Rectangle AlignTop(const Rectangle& container) const;
            Rectangle AlignBottom(const Rectangle& container) const;
            Rectangle AlignCenterHorizontal(const Rectangle& container) const;
            Rectangle AlignCenterVertical(const Rectangle& container) const;
            Rectangle CenterIn(const Rectangle& container) const;
            Rectangle ClampTo(const Rectangle& bounds) const;
            Rectangle FitInside(const Rectangle& container) const;
            Rectangle Rotate90() const;
    
            // Subdivision methods - DECLARE ONLY
        std::vector<Rectangle> SubdivideHorizontal(int count) const;
        std::vector<Rectangle> SubdivideVertical(int count) const;
        std::vector<std::vector<Rectangle>> CreateGrid(int rows, int cols) const;
    
        // Distance calculations - DECLARE ONLY
        float DistanceToPoint(const Point& point) const;
        float DistanceTo(const Rectangle& other) const;
    
        // String representation - DECLARE ONLY
        std::string ToString() const;
        void DebugPrint() const;
    
        // ImGui conversion helpers
        ImVec2 ToImVec2() const { return ImVec2(x, y); }
        ImVec2 SizeToImVec2() const { return ImVec2(width, height); }
        static Rectangle FromImVec2(const ImVec2& pos, const ImVec2& size) {
        return Rectangle(pos.x, pos.y, size.x, size.y);
        }
    };
}
