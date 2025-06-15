/**
 * @file Rectangle.cpp
 * @brief Implementation of advanced Rectangle operations and utilities
 */

#include "ImGuiForms/Core/Rectangle.h"
#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>

namespace ImGuiForms {

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

    Rectangle Rectangle::FitInside(const Rectangle& container, bool maintainAspectRatio) const {
        if (container.IsEmpty() || IsEmpty()) {
            return Rectangle();
        }

        if (!maintainAspectRatio) {
            return container;
        }

        float containerAspect = container.AspectRatio();
        float thisAspect = AspectRatio();

        int newWidth, newHeight;

        if (thisAspect > containerAspect) {
            // Fit to width
            newWidth = container.width;
            newHeight = static_cast<int>(newWidth / thisAspect);
        }
        else {
            // Fit to height
            newHeight = container.height;
            newWidth = static_cast<int>(newHeight * thisAspect);
        }

        // Center in container
        int newX = container.x + (container.width - newWidth) / 2;
        int newY = container.y + (container.height - newHeight) / 2;

        return Rectangle(newX, newY, newWidth, newHeight);
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

    std::vector<Rectangle> Rectangle::SubdivideHorizontal(const std::vector<float>& ratios) const {
        std::vector<Rectangle> result;

        if (ratios.empty()) return result;

        // Normalize ratios
        float totalRatio = 0.0f;
        for (float ratio : ratios) {
            totalRatio += ratio;
        }

        if (totalRatio <= 0.0f) return result;

        int currentX = x;

        for (size_t i = 0; i < ratios.size(); ++i) {
            float normalizedRatio = ratios[i] / totalRatio;
            int sectionWidth = static_cast<int>(width * normalizedRatio);

            // Adjust last section to account for rounding
            if (i == ratios.size() - 1) {
                sectionWidth = (x + width) - currentX;
            }

            result.emplace_back(currentX, y, sectionWidth, height);
            currentX += sectionWidth;
        }

        return result;
    }

    std::vector<Rectangle> Rectangle::SubdivideVertical(const std::vector<float>& ratios) const {
        std::vector<Rectangle> result;

        if (ratios.empty()) return result;

        // Normalize ratios
        float totalRatio = 0.0f;
        for (float ratio : ratios) {
            totalRatio += ratio;
        }

        if (totalRatio <= 0.0f) return result;

        int currentY = y;

        for (size_t i = 0; i < ratios.size(); ++i) {
            float normalizedRatio = ratios[i] / totalRatio;
            int sectionHeight = static_cast<int>(height * normalizedRatio);

            // Adjust last section to account for rounding
            if (i == ratios.size() - 1) {
                sectionHeight = (y + height) - currentY;
            }

            result.emplace_back(x, currentY, width, sectionHeight);
            currentY += sectionHeight;
        }

        return result;
    }

    Rectangle Rectangle::CreateGrid(int row, int col, int rows, int cols, int spacing) const {
        if (rows <= 0 || cols <= 0 || row < 0 || col < 0 || row >= rows || col >= cols) {
            return Rectangle();
        }

        int totalSpacingX = spacing * (cols - 1);
        int totalSpacingY = spacing * (rows - 1);

        int cellWidth = (width - totalSpacingX) / cols;
        int cellHeight = (height - totalSpacingY) / rows;

        int cellX = x + col * (cellWidth + spacing);
        int cellY = y + row * (cellHeight + spacing);

        return Rectangle(cellX, cellY, cellWidth, cellHeight);
    }

    // ============================================================================
    // Alignment Operations
    // ============================================================================

    Rectangle Rectangle::AlignLeft(const Rectangle& container, int margin) const {
        return Rectangle(container.x + margin, y, width, height);
    }

    Rectangle Rectangle::AlignRight(const Rectangle& container, int margin) const {
        return Rectangle(container.x + container.width - width - margin, y, width, height);
    }

    Rectangle Rectangle::AlignTop(const Rectangle& container, int margin) const {
        return Rectangle(x, container.y + margin, width, height);
    }

    Rectangle Rectangle::AlignBottom(const Rectangle& container, int margin) const {
        return Rectangle(x, container.y + container.height - height - margin, width, height);
    }

    Rectangle Rectangle::AlignCenterHorizontal(const Rectangle& container) const {
        int centeredX = container.x + (container.width - width) / 2;
        return Rectangle(centeredX, y, width, height);
    }

    Rectangle Rectangle::AlignCenterVertical(const Rectangle& container) const {
        int centeredY = container.y + (container.height - height) / 2;
        return Rectangle(x, centeredY, width, height);
    }

    // ============================================================================
    // Distance and Collision Detection
    // ============================================================================

    float Rectangle::DistanceTo(const Rectangle& other) const {
        // Calculate the closest distance between two rectangles
        int dx = std::max({ 0, x - (other.x + other.width), other.x - (x + width) });
        int dy = std::max({ 0, y - (other.y + other.height), other.y - (y + height) });

        return std::sqrt(static_cast<float>(dx * dx + dy * dy));
    }

    float Rectangle::DistanceToPoint(const ImVec2& point) const {
        float px = point.x;
        float py = point.y;

        // Find closest point on rectangle to the given point
        float closestX = std::clamp(px, static_cast<float>(x), static_cast<float>(x + width));
        float closestY = std::clamp(py, static_cast<float>(y), static_cast<float>(y + height));

        float dx = px - closestX;
        float dy = py - closestY;

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

    Rectangle Rectangle::Scale(float scaleX, float scaleY, const ImVec2& origin) const {
        // Scale around a specific origin point
        float newWidth = width * scaleX;
        float newHeight = height * scaleY;

        float newX = origin.x + (x - origin.x) * scaleX;
        float newY = origin.y + (y - origin.y) * scaleY;

        return Rectangle(static_cast<int>(newX), static_cast<int>(newY),
            static_cast<int>(newWidth), static_cast<int>(newHeight));
    }

    Rectangle Rectangle::Scale(float scale) const {
        return Scale(scale, scale, Center());
    }

    Rectangle Rectangle::Rotate90() const {
        // Rotate 90 degrees clockwise (swap width/height, adjust position)
        return Rectangle(x, y, height, width);
    }

    // ============================================================================
    // Utility and Debug Functions
    // ============================================================================

    std::string Rectangle::ToString() const {
        std::ostringstream oss;
        oss << "Rectangle(" << x << ", " << y << ", " << width << ", " << height << ")";
        return oss.str();
    }

    void Rectangle::DebugPrint(const std::string& label) const {
        std::cout << label << ": " << ToString()
            << " [Area: " << Area()
            << ", Aspect: " << AspectRatio() << "]" << std::endl;
    }

    Rectangle Rectangle::FromTwoPoints(const ImVec2& point1, const ImVec2& point2) {
        int left = static_cast<int>(std::min(point1.x, point2.x));
        int top = static_cast<int>(std::min(point1.y, point2.y));
        int right = static_cast<int>(std::max(point1.x, point2.x));
        int bottom = static_cast<int>(std::max(point1.y, point2.y));

        return Rectangle(left, top, right - left, bottom - top);
    }

    Rectangle Rectangle::FromCenter(const ImVec2& center, int width, int height) {
        int x = static_cast<int>(center.x - width / 2);
        int y = static_cast<int>(center.y - height / 2);

        return Rectangle(x, y, width, height);
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
                minX = std::min(minX, rect.x);
                minY = std::min(minY, rect.y);
                maxX = std::max(maxX, rect.x + rect.width);
                maxY = std::max(maxY, rect.y + rect.height);
            }

            return Rectangle(minX, minY, maxX - minX, maxY - minY);
        }

    } // namespace RectangleUtils

} // namespace ImGuiForms