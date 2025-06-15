#pragma once
#include <algorithm>
#include <functional>

namespace ImGuiForms {

    /**
     * @brief Represents a size value that can be absolute (pixels) or relative (0.0-1.0)
     *
     * Special values:
     * - Content: Auto-size to component's content (-1 absolute)
     * - Parent: Fill entire parent container (1.0 relative)
     */
    struct SizeValue {
        float value;
        bool isRelative;

        // Special constants
        static constexpr SizeValue Content() { return SizeValue(-1.0f, false); }
        static constexpr SizeValue Parent() { return SizeValue(1.0f, true); }

        // Default constructor - creates Content-aligned size
        constexpr SizeValue() : value(-1.0f), isRelative(false) {}

        // Private constructor for internal use
        constexpr SizeValue(float val, bool relative)
            : value(val), isRelative(relative) {
        }

        // Static factory methods
        static constexpr SizeValue Absolute(int pixels) {
            return SizeValue(static_cast<float>(std::max(pixels, -1)), false);
        }

        static constexpr SizeValue Relative(float factor) {
            return SizeValue(std::clamp(factor, 0.0f, 1.0f), true);
        }

        // Type checking
        constexpr bool IsAbsolute() const { return !isRelative; }
        constexpr bool IsRelative() const { return isRelative; }
        constexpr bool IsContentAligned() const { return IsAbsolute() && static_cast<int>(value) == -1; }
        constexpr bool IsParentAligned() const { return IsRelative() && static_cast<int>(value) == 1; }
        constexpr bool IsVisible() const { return value != 0.0f; }

        // Comparison operators
        constexpr bool operator==(const SizeValue& other) const {
            return value == other.value && isRelative == other.isRelative;
        }

        constexpr bool operator!=(const SizeValue& other) const {
            return !(*this == other);
        }

        // Implicit conversions for convenience
        constexpr SizeValue(int pixels) : SizeValue(Absolute(pixels)) {}
        constexpr SizeValue(float factor) : SizeValue(Relative(factor)) {}

        // Hash support
        struct Hash {
            std::size_t operator()(const SizeValue& sv) const {
                return std::hash<float>{}(sv.value) ^ (std::hash<bool>{}(sv.isRelative) << 1);
            }
        };
    };

    /**
     * @brief Represents a 2D size with width and height SizeValues
     *
     * Provides convenient constants and utility methods for component sizing.
     */
    struct Size {
        SizeValue width;
        SizeValue height;

        // Common size constants
        static constexpr Size Empty() { return Size(0, 0); }
        static constexpr Size Parent() { return Size(SizeValue::Parent(), SizeValue::Parent()); }
        static constexpr Size Content() { return Size(SizeValue::Content(), SizeValue::Content()); }
        static constexpr Size WidthAlign() { return Size(SizeValue::Parent(), SizeValue::Content()); }
        static constexpr Size HeightAlign() { return Size(SizeValue::Content(), SizeValue::Parent()); }

        // Constructors
        constexpr Size() : width(SizeValue::Content()), height(SizeValue::Content()) {}

        constexpr Size(SizeValue w, SizeValue h) : width(w), height(h) {}

        // Convenience constructors
        constexpr Size(int w, int h) : width(w), height(h) {}
        constexpr Size(float w, float h) : width(w), height(h) {}
        constexpr Size(int w, SizeValue h) : width(w), height(h) {}
        constexpr Size(SizeValue w, int h) : width(w), height(h) {}

        // Type checking
        constexpr bool IsContentAligned() const {
            return width.IsContentAligned() && height.IsContentAligned();
        }

        constexpr bool IsParentAligned() const {
            return width.IsParentAligned() && height.IsParentAligned();
        }

        constexpr bool IsVisible() const {
            return width.IsVisible() && height.IsVisible();
        }

        // Comparison operators
        constexpr bool operator==(const Size& other) const {
            return width == other.width && height == other.height;
        }

        constexpr bool operator!=(const Size& other) const {
            return !(*this == other);
        }

        // Hash support
        struct Hash {
            std::size_t operator()(const Size& size) const {
                return SizeValue::Hash{}(size.width) ^ (SizeValue::Hash{}(size.height) << 1);
            }
        };
    };

} // namespace ImGuiForms