#pragma once
#include <string>

namespace ImGuiForms {

    /**
     * @brief Represents a 2D size with width and height
     */
    struct Size {
        float width, height;

        // Constructors
        Size() : width(0.0f), height(0.0f) {}
        Size(float width, float height) : width(width), height(height) {}

        // Static size constants for special behaviors
        static const Size Content;  // Size based on content
        static const Size Parent;   // Size based on parent
        static const Size Fill;     // Fill available space

        // Properties
        bool IsEmpty() const {
            return width <= 0.0f || height <= 0.0f;
        }

        float Area() const {
            return width * height;
        }

        float AspectRatio() const {
            return height != 0.0f ? width / height : 0.0f;
        }

        // Operators
        bool operator==(const Size& other) const {
            return width == other.width && height == other.height;
        }

        bool operator!=(const Size& other) const {
            return !(*this == other);
        }

        Size operator+(const Size& other) const {
            return Size(width + other.width, height + other.height);
        }

        Size operator-(const Size& other) const {
            return Size(width - other.width, height - other.height);
        }

        Size operator*(float factor) const {
            return Size(width * factor, height * factor);
        }

        Size operator/(float factor) const {
            if (factor != 0.0f) {
                return Size(width / factor, height / factor);
            }
            return Size();
        }

        // Utility methods
        Size Max(const Size& other) const {
            return Size(
                width > other.width ? width : other.width,
                height > other.height ? height : other.height
            );
        }

        Size Min(const Size& other) const {
            return Size(
                width < other.width ? width : other.width,
                height < other.height ? height : other.height
            );
        }

        // String representation
        std::string ToString() const;
    };

} // namespace ImGuiForms