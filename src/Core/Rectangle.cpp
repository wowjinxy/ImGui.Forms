/**
 * @file Rectangle.cpp
 * @brief Implementation of advanced Rectangle operations and utilities
 */
#include "ImGuiForms/Core/Rectangle.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <imgui.h>

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

    // ============================================================================
    // Advanced Rectangle Operations
    // ============================================================================

    Rectangle Rectangle::Union(const Rectangle& other) const {
        if (IsEmpty()) return other;
        if (other.IsEmpty()) return *this;

        int left = std::min(x, other.x);
        int top = std::min(y, other.y);
        int right = std::max(x + width, other.x + other.width);
        int bottom = std::max(y + height, other.y + other.height);

        return Rectangle(left, top, right - left, bottom - top);
    }

    Rectangle Rectangle::Intersection(const Rectangle& other) const {
        int left = std::max(x, other.x);
        int top = std::max(y, other.y);
        int right = std::min(x + width, other.x + other.width);
        int bottom = std::min(y + height, other.y + other.height);

        if (left >= right || top >= bottom) {
            return Rectangle(); // Empty rectangle
        }

        return Rectangle(left, top, right - left, bottom - top);
    }

    bool Rectangle::ContainsPoint(const Point& point) const {
        return point.x >= x && point.x <= Right() &&
            point.y >= y && point.y <= Bottom();
    }

    bool Rectangle::ContainsRectangle(const Rectangle& other) const {
        return other.x >= x && other.y >= y &&
            other.Right() <= Right() && other.Bottom() <= Bottom();
    }

    // String representation


    float Rectangle::Area() const {
        return static_cast<float>(width * height);
    }

    float Rectangle::Perimeter() const {
        return 2.0f * (width + height);
    }

    float Rectangle::AspectRatio() const {
        if (height == 0) return 0.0f;
        return static_cast<float>(width) / static_cast<float>(height);
    }

    Rectangle Rectangle::CenterIn(const Rectangle& container) const {
        int newX = container.x + (container.width - width) / 2;
        int newY = container.y + (container.height - height) / 2;

        return Rectangle(newX, newY, width, height);
    }

    Rectangle Rectangle::ClampTo(const Rectangle& bounds) const {
        int newX = std::clamp(x, bounds.x, bounds.x + bounds.width - width);
        int newY = std::clamp(y, bounds.y, bounds.y + bounds.height - height);
        int newWidth = std::min(width, bounds.width);
        int newHeight = std::min(height, bounds.height);

        return Rectangle(newX, newY, newWidth, newHeight);
    }

    // ============================================================================
    // Layout Helper Functions
    // ============================================================================

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

    // ============================================================================
    // Alignment Operations
    // ============================================================================

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

    // ============================================================================
    // Distance and Collision Detection
    // ============================================================================

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

    bool Rectangle::ContainsRectangle(const Rectangle& other) const {
        return other.x >= x &&
            other.y >= y &&
            other.x + other.width <= x + width &&
            other.y + other.height <= y + height;
    }

    // ============================================================================
    // Transformation Operations
    // ============================================================================

    Rectangle Rectangle::Scale(float factor) const {
        return Rectangle(x * factor, y * factor, width * factor, height * factor);
    }

    Rectangle Rectangle::Scale(float xFactor, float yFactor) const {
        return Rectangle(x * xFactor, y * yFactor, width * xFactor, height * yFactor);
    }

    Rectangle Rectangle::Rotate90() const {
        // Rotate 90 degrees clockwise (swap width/height, adjust position)
        return Rectangle(x, y, height, width);
    }

    // ============================================================================
    // Utility and Debug Functions
    // ============================================================================

    std::string Rectangle::ToString() const {
        return "Rectangle(" + std::to_string(x) + ", " + std::to_string(y) +
            ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
    }

    void Rectangle::DebugPrint() const {
        printf("Rectangle: x=%.2f, y=%.2f, width=%.2f, height=%.2f\n", x, y, width, height);
    }

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

    // ============================================================================
    // Layout Calculation Helpers
    // ============================================================================

    namespace RectangleUtils {

        std::vector<Rectangle> ArrangeInGrid(const Rectangle& container,
            int itemCount,
            int preferredColumns,
            const ImVec2& spacing,
            const ImVec2& padding) {
            std::vector<Rectangle> result;

            if (itemCount <= 0) return result;

            // Calculate optimal grid dimensions
            int columns = preferredColumns > 0 ? preferredColumns :
                static_cast<int>(std::ceil(std::sqrt(itemCount)));
            int rows = (itemCount + columns - 1) / columns; // Round up division

            // Calculate available space after padding
            int availableWidth = container.width - static_cast<int>(padding.x * 2);
            int availableHeight = container.height - static_cast<int>(padding.y * 2);

            // Calculate cell dimensions
            int totalSpacingX = static_cast<int>(spacing.x * (columns - 1));
            int totalSpacingY = static_cast<int>(spacing.y * (rows - 1));

            int cellWidth = (availableWidth - totalSpacingX) / columns;
            int cellHeight = (availableHeight - totalSpacingY) / rows;

            // Generate rectangles
            for (int i = 0; i < itemCount; ++i) {
                int row = i / columns;
                int col = i % columns;

                int cellX = container.x + static_cast<int>(padding.x) + col * (cellWidth + static_cast<int>(spacing.x));
                int cellY = container.y + static_cast<int>(padding.y) + row * (cellHeight + static_cast<int>(spacing.y));

                result.emplace_back(cellX, cellY, cellWidth, cellHeight);
            }

            return result;
        }

        std::vector<Rectangle> ArrangeInLine(const Rectangle& container,
            int itemCount,
            bool horizontal,
            const ImVec2& spacing,
            const ImVec2& padding) {
            std::vector<Rectangle> result;

            if (itemCount <= 0) return result;

            if (horizontal) {
                // Horizontal arrangement
                int availableWidth = container.width - static_cast<int>(padding.x * 2);
                int totalSpacing = static_cast<int>(spacing.x * (itemCount - 1));
                int itemWidth = (availableWidth - totalSpacing) / itemCount;
                int itemHeight = container.height - static_cast<int>(padding.y * 2);

                for (int i = 0; i < itemCount; ++i) {
                    int itemX = container.x + static_cast<int>(padding.x) + i * (itemWidth + static_cast<int>(spacing.x));
                    int itemY = container.y + static_cast<int>(padding.y);

                    result.emplace_back(itemX, itemY, itemWidth, itemHeight);
                }
            }
            else {
                // Vertical arrangement
                int availableHeight = container.height - static_cast<int>(padding.y * 2);
                int totalSpacing = static_cast<int>(spacing.y * (itemCount - 1));
                int itemHeight = (availableHeight - totalSpacing) / itemCount;
                int itemWidth = container.width - static_cast<int>(padding.x * 2);

                for (int i = 0; i < itemCount; ++i) {
                    int itemX = container.x + static_cast<int>(padding.x);
                    int itemY = container.y + static_cast<int>(padding.y) + i * (itemHeight + static_cast<int>(spacing.y));

                    result.emplace_back(itemX, itemY, itemWidth, itemHeight);
                }
            }

            return result;
        }

        Rectangle CalculateBoundingBox(const std::vector<Rectangle>& rectangles) {
            if (rectangles.empty()) {
                return Rectangle();
            }

            int minX = rectangles[0].x;
            int minY = rectangles[0].y;
            int maxX = rectangles[0].x + rectangles[0].width;
            int maxY = rectangles[0].y + rectangles[0].height;

            for (size_t i = 1; i < rectangles.size(); ++i) {
                const auto& rect = rectangles[i];
                minX = SafeMin(minX, rect.x);
                minY = SafeMin(minY, rect.y);
                maxX = SafeMax(maxX, rect.x + rect.width);
                maxY = SafeMax(maxY, rect.y + rect.height);
            }

            return Rectangle(minX, minY, maxX - minX, maxY - minY);
        }

    } // namespace RectangleUtils

} // namespace ImGuiForms