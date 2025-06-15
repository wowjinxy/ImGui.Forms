#pragma once
#include <imgui.h>
#include <functional>

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
    };

} // namespace ImGuiForms