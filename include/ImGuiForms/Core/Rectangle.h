#pragma once
#include <imgui.h>
#include <functional>
#include <vector>
#include <string>

namespace ImGuiForms {

    /**
     * @brief Represents a 2D rectangle with position and size
     *
     * Compatible with ImGui's ImVec2 and provides convenient methods for
     * layout calculations and drawing operations.
     */
    struct Rectangle {
        int x, y;           // Position
        int width, height;  // Size

        // Constructors
        constexpr Rectangle() : x(0), y(0), width(0), height(0) {}

        constexpr Rectangle(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {
        }

        constexpr Rectangle(const ImVec2& position, const ImVec2& size)
            : x(static_cast<int>(position.x))
            , y(static_cast<int>(position.y))
            , width(static_cast<int>(size.x))
            , height(static_cast<int>(size.y)) {
        }

        // Property accessors (to match C# style)
        constexpr int X() const { return x; }
        constexpr int Y() const { return y; }
        constexpr int Width() const { return width; }
        constexpr int Height() const { return height; }

        // Position and size as ImVec2 for ImGui compatibility
        ImVec2 Position() const { return ImVec2(static_cast<float>(x), static_cast<float>(y)); }
        ImVec2 Size() const { return ImVec2(static_cast<float>(width), static_cast<float>(height)); }

        // Calculated properties
        constexpr int Left() const { return x; }
        constexpr int Top() const { return y; }
        constexpr int Right() const { return x + width; }
        constexpr int Bottom() const { return y + height; }

        ImVec2 TopLeft() const { return ImVec2(static_cast<float>(x), static_cast<float>(y)); }
        ImVec2 TopRight() const { return ImVec2(static_cast<float>(x + width), static_cast<float>(y)); }
        ImVec2 BottomLeft() const { return ImVec2(static_cast<float>(x), static_cast<float>(y + height)); }
        ImVec2 BottomRight() const { return ImVec2(static_cast<float>(x + width), static_cast<float>(y + height)); }
        ImVec2 Center() const { return ImVec2(static_cast<float>(x + width / 2), static_cast<float>(y + height / 2)); }

        // Utility methods
        constexpr bool Contains(int px, int py) const {
            return px >= x && px < x + width && py >= y && py < y + height;
        }

        bool Contains(const ImVec2& point) const {
            return Contains(static_cast<int>(point.x), static_cast<int>(point.y));
        }

        constexpr bool IsEmpty() const {
            return width <= 0 || height <= 0;
        }

        constexpr bool Intersects(const Rectangle& other) const {
            return x < other.x + other.width && x + width > other.x &&
                y < other.y + other.height && y + height > other.y;
        }

        // Create a rectangle offset by the given amounts
        constexpr Rectangle Offset(int dx, int dy) const {
            return Rectangle(x + dx, y + dy, width, height);
        }

        constexpr Rectangle Offset(const ImVec2& offset) const {
            return Offset(static_cast<int>(offset.x), static_cast<int>(offset.y));
        }

        // Create a rectangle with adjusted size
        constexpr Rectangle Resize(int newWidth, int newHeight) const {
            return Rectangle(x, y, newWidth, newHeight);
        }

        constexpr Rectangle Resize(const ImVec2& newSize) const {
            return Resize(static_cast<int>(newSize.x), static_cast<int>(newSize.y));
        }

        // Inflate/deflate the rectangle
        constexpr Rectangle Inflate(int dx, int dy) const {
            return Rectangle(x - dx, y - dy, width + 2 * dx, height + 2 * dy);
        }

        constexpr Rectangle Inflate(int delta) const {
            return Inflate(delta, delta);
        }

        // Comparison operators
        constexpr bool operator==(const Rectangle& other) const {
            return x == other.x && y == other.y && width == other.width && height == other.height;
        }

        constexpr bool operator!=(const Rectangle& other) const {
            return !(*this == other);
        }

        // Hash support
        struct Hash {
            std::size_t operator()(const Rectangle& rect) const {
                std::size_t h1 = std::hash<int>{}(rect.x);
                std::size_t h2 = std::hash<int>{}(rect.y);
                std::size_t h3 = std::hash<int>{}(rect.width);
                std::size_t h4 = std::hash<int>{}(rect.height);
                return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
            }
        };

        /**
         * @brief Utility functions for rectangle layout and arrangement
         */
        namespace RectangleUtils {
            /**
             * @brief Arrange items in a grid layout within a container
             * @param container The container rectangle
             * @param itemCount Number of items to arrange
             * @param preferredColumns Preferred number of columns (0 for auto)
             * @param spacing Spacing between items
             * @param padding Padding around the container edges
             * @return Vector of rectangles for each item
             */
            std::vector<Rectangle> ArrangeInGrid(const Rectangle& container,
                int itemCount,
                int preferredColumns = 0,
                const ImVec2& spacing = ImVec2(5, 5),
                const ImVec2& padding = ImVec2(10, 10));

            /**
             * @brief Arrange items in a horizontal or vertical line
             * @param container The container rectangle
             * @param itemCount Number of items to arrange
             * @param horizontal True for horizontal, false for vertical arrangement
             * @param spacing Spacing between items
             * @param padding Padding around the container edges
             * @return Vector of rectangles for each item
             */
            std::vector<Rectangle> ArrangeInLine(const Rectangle& container,
                int itemCount,
                bool horizontal = true,
                const ImVec2& spacing = ImVec2(5, 5),
                const ImVec2& padding = ImVec2(10, 10));

            /**
             * @brief Calculate the bounding box that contains all given rectangles
             * @param rectangles Vector of rectangles to bound
             * @return Bounding rectangle
             */
            Rectangle CalculateBoundingBox(const std::vector<Rectangle>& rectangles);
        }

        // ========================================================================
        // Advanced Geometric Operations (implemented in Rectangle.cpp)
        // ========================================================================

        /**
         * @brief Calculate the union of this rectangle with another
         */
        Rectangle Union(const Rectangle& other) const;

        /**
         * @brief Calculate the intersection of this rectangle with another
         */
        Rectangle Intersection(const Rectangle& other) const;

        /**
         * @brief Calculate the area of the rectangle
         */
        float Area() const;

        /**
         * @brief Calculate the perimeter of the rectangle
         */
        float Perimeter() const;

        /**
         * @brief Calculate the aspect ratio (width/height)
         */
        float AspectRatio() const;

        /**
         * @brief Fit this rectangle inside a container, optionally maintaining aspect ratio
         */
        Rectangle FitInside(const Rectangle& container, bool maintainAspectRatio = true) const;

        /**
         * @brief Center this rectangle within a container
         */
        Rectangle CenterIn(const Rectangle& container) const;

        /**
         * @brief Clamp this rectangle to stay within bounds
         */
        Rectangle ClampTo(const Rectangle& bounds) const;

        // ========================================================================
        // Layout Operations
        // ========================================================================

        /**
         * @brief Subdivide rectangle horizontally using ratios
         */
        std::vector<Rectangle> SubdivideHorizontal(const std::vector<float>& ratios) const;

        /**
         * @brief Subdivide rectangle vertically using ratios
         */
        std::vector<Rectangle> SubdivideVertical(const std::vector<float>& ratios) const;

        /**
         * @brief Get a cell from a grid layout
         */
        Rectangle CreateGrid(int row, int col, int rows, int cols, int spacing = 0) const;

        // ========================================================================
        // Alignment Operations
        // ========================================================================

        Rectangle AlignLeft(const Rectangle& container, int margin = 0) const;
        Rectangle AlignRight(const Rectangle& container, int margin = 0) const;
        Rectangle AlignTop(const Rectangle& container, int margin = 0) const;
        Rectangle AlignBottom(const Rectangle& container, int margin = 0) const;
        Rectangle AlignCenterHorizontal(const Rectangle& container) const;
        Rectangle AlignCenterVertical(const Rectangle& container) const;

        // ========================================================================
        // Distance and Collision
        // ========================================================================

        /**
         * @brief Calculate distance to another rectangle
         */
        float DistanceTo(const Rectangle& other) const;

        /**
         * @brief Calculate distance to a point
         */
        float DistanceToPoint(const ImVec2& point) const;

        /**
         * @brief Check if this rectangle completely contains another
         */
        bool ContainsRectangle(const Rectangle& other) const;

        // ========================================================================
        // Transformations
        // ========================================================================

        /**
         * @brief Scale rectangle around an origin point
         */
        Rectangle Scale(float scaleX, float scaleY, const ImVec2& origin) const;

        /**
         * @brief Scale rectangle uniformly around its center
         */
        Rectangle Scale(float scale) const;

        /**
         * @brief Rotate rectangle 90 degrees clockwise
         */
        Rectangle Rotate90() const;

        // ========================================================================
        // Utility Functions
        // ========================================================================

        /**
         * @brief Convert rectangle to string representation
         */
        std::string ToString() const;

        /**
         * @brief Print rectangle information for debugging
         */
        void DebugPrint(const std::string& label = "Rectangle") const;

        // ========================================================================
        // Static Factory Methods
        // ========================================================================

        /**
         * @brief Create rectangle from two corner points
         */
        static Rectangle FromTwoPoints(const ImVec2& point1, const ImVec2& point2);

        /**
         * @brief Create rectangle from center point and dimensions
         */
        static Rectangle FromCenter(const ImVec2& center, int width, int height);
    };

} // namespace ImGuiForms