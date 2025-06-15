#include "ImGuiForms/Core/Rectangle.h"
#include "ImGuiForms/Core/Point.h"
#include "ImGuiForms/Core/Size.h"
#include <cstdio>

// Helper functions to avoid std::min/max issues
namespace {
    inline float SafeMin(float a, float b) { return a < b ? a : b; }
    inline float SafeMax(float a, float b) { return a > b ? a : b; }
    inline float SafeClamp(float value, float min_val, float max_val) {
        return SafeMax(min_val, SafeMin(value, max_val));
    }
}

namespace ImGuiForms {

    // Constructors
    Rectangle::Rectangle() : x(0.0f), y(0.0f), width(0.0f), height(0.0f) {}

    Rectangle::Rectangle(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {
    }

    Rectangle::Rectangle(const Point& location, const Size& size)
        : x(location.x), y(location.y), width(size.width), height(size.height) {
    }

    // Properties
    bool Rectangle::IsEmpty() const {
        return width <= 0.0f || height <= 0.0f;
    }

    float Rectangle::Area() const {
        return width * height;
    }

    float Rectangle::AspectRatio() const {
        return height != 0.0f ? width / height : 0.0f;
    }

    float Rectangle::Perimeter() const {
        return 2.0f * (width + height);
    }

    Point Rectangle::Center() const {
        return Point(x + width / 2.0f, y + height / 2.0f);
    }

    bool Rectangle::Contains(const Point& point) const {
        return point.x >= x && point.x <= Right() &&
            point.y >= y && point.y <= Bottom();
    }

    // Static factory methods
    Rectangle Rectangle::FromTwoPoints(const Point& p1, const Point& p2) {
        float minX = SafeMin(p1.x, p2.x);
        float minY = SafeMin(p1.y, p2.y);
        float maxX = SafeMax(p1.x, p2.x);
        float maxY = SafeMax(p1.y, p2.y);
        return Rectangle(minX, minY, maxX - minX, maxY - minY);
    }

    Rectangle Rectangle::FromCenter(const Point& center, const Size& size) {
        return Rectangle(center.x - size.width / 2.0f, center.y - size.height / 2.0f,
            size.width, size.height);
    }

    // Operations
    Rectangle Rectangle::Union(const Rectangle& other) const {
        if (IsEmpty()) return other;
        if (other.IsEmpty()) return *this;

        float minX = SafeMin(x, other.x);
        float minY = SafeMin(y, other.y);
        float maxX = SafeMax(Right(), other.Right());
        float maxY = SafeMax(Bottom(), other.Bottom());

        return Rectangle(minX, minY, maxX - minX, maxY - minY);
    }

    Rectangle Rectangle::Intersection(const Rectangle& other) const {
        float maxLeft = SafeMax(x, other.x);
        float maxTop = SafeMax(y, other.y);
        float minRight = SafeMin(Right(), other.Right());
        float minBottom = SafeMin(Bottom(), other.Bottom());

        if (maxLeft < minRight && maxTop < minBottom) {
            return Rectangle(maxLeft, maxTop, minRight - maxLeft, minBottom - maxTop);
        }
        return Rectangle(); // Empty rectangle
    }

    bool Rectangle::ContainsPoint(const Point& point) const {
        return point.x >= x && point.x <= Right() &&
            point.y >= y && point.y <= Bottom();
    }

    bool Rectangle::ContainsRectangle(const Rectangle& other) const {
        return other.x >= x && other.y >= y &&
            other.Right() <= Right() && other.Bottom() <= Bottom();
    }

    Rectangle Rectangle::Scale(float factor) const {
        return Rectangle(x * factor, y * factor, width * factor, height * factor);
    }

    Rectangle Rectangle::Scale(float xFactor, float yFactor) const {
        return Rectangle(x * xFactor, y * yFactor, width * xFactor, height * yFactor);
    }

    // Alignment methods
    Rectangle Rectangle::AlignLeft(const Rectangle& container) const {
        return Rectangle(container.x, y, width, height);
    }

    Rectangle Rectangle::AlignRight(const Rectangle& container) const {
        return Rectangle(container.Right() - width, y, width, height);
    }

    Rectangle Rectangle::AlignTop(const Rectangle& container) const {
        return Rectangle(x, container.y, width, height);
    }

    Rectangle Rectangle::AlignBottom(const Rectangle& container) const {
        return Rectangle(x, container.Bottom() - height, width, height);
    }

    Rectangle Rectangle::AlignCenterHorizontal(const Rectangle& container) const {
        return Rectangle(container.x + (container.width - width) / 2.0f, y, width, height);
    }

    Rectangle Rectangle::AlignCenterVertical(const Rectangle& container) const {
        return Rectangle(x, container.y + (container.height - height) / 2.0f, width, height);
    }

    Rectangle Rectangle::CenterIn(const Rectangle& container) const {
        return Rectangle(
            container.x + (container.width - width) / 2.0f,
            container.y + (container.height - height) / 2.0f,
            width, height
        );
    }

    Rectangle Rectangle::ClampTo(const Rectangle& bounds) const {
        float newX = SafeClamp(x, bounds.x, bounds.Right() - width);
        float newY = SafeClamp(y, bounds.y, bounds.Bottom() - height);
        return Rectangle(newX, newY, width, height);
    }

    Rectangle Rectangle::FitInside(const Rectangle& container) const {
        if (IsEmpty() || container.IsEmpty()) return Rectangle();

        float scaleX = container.width / width;
        float scaleY = container.height / height;
        float scale = SafeMin(scaleX, scaleY);

        Size newSize(width * scale, height * scale);
        return FromCenter(container.Center(), newSize);
    }

    Rectangle Rectangle::Rotate90() const {
        return Rectangle(x, y, height, width);
    }

    // Subdivision methods
    std::vector<Rectangle> Rectangle::SubdivideHorizontal(int count) const {
        std::vector<Rectangle> result;
        if (count <= 0) return result;

        float subWidth = width / static_cast<float>(count);
        for (int i = 0; i < count; ++i) {
            result.push_back(Rectangle(x + static_cast<float>(i) * subWidth, y, subWidth, height));
        }
        return result;
    }

    std::vector<Rectangle> Rectangle::SubdivideVertical(int count) const {
        std::vector<Rectangle> result;
        if (count <= 0) return result;

        float subHeight = height / static_cast<float>(count);
        for (int i = 0; i < count; ++i) {
            result.push_back(Rectangle(x, y + static_cast<float>(i) * subHeight, width, subHeight));
        }
        return result;
    }

    std::vector<std::vector<Rectangle>> Rectangle::CreateGrid(int rows, int cols) const {
        std::vector<std::vector<Rectangle>> grid(rows);
        if (rows <= 0 || cols <= 0) return grid;

        float cellWidth = width / static_cast<float>(cols);
        float cellHeight = height / static_cast<float>(rows);

        for (int row = 0; row < rows; ++row) {
            grid[row].resize(cols);
            for (int col = 0; col < cols; ++col) {
                grid[row][col] = Rectangle(
                    x + static_cast<float>(col) * cellWidth,
                    y + static_cast<float>(row) * cellHeight,
                    cellWidth,
                    cellHeight
                );
            }
        }
        return grid;
    }

    // Distance calculations
    float Rectangle::DistanceToPoint(const Point& point) const {
        float dx = SafeMax(0.0f, SafeMax(x - point.x, point.x - Right()));
        float dy = SafeMax(0.0f, SafeMax(y - point.y, point.y - Bottom()));
        return std::sqrt(dx * dx + dy * dy);
    }

    float Rectangle::DistanceTo(const Rectangle& other) const {
        if (!Intersection(other).IsEmpty()) return 0.0f; // Overlapping

        float dx = SafeMax(0.0f, SafeMax(x - other.Right(), other.x - Right()));
        float dy = SafeMax(0.0f, SafeMax(y - other.Bottom(), other.y - Bottom()));
        return std::sqrt(dx * dx + dy * dy);
    }

    // String representation
    std::string Rectangle::ToString() const {
        return "Rectangle(" + std::to_string(x) + ", " + std::to_string(y) +
            ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    }

    void Rectangle::DebugPrint() const {
        printf("Rectangle: x=%.2f, y=%.2f, width=%.2f, height=%.2f\n", x, y, width, height);
    }

} // namespace ImGuiForms