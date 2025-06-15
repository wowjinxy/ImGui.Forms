#pragma once
#include <algorithm>
#include <functional>

namespace ImGuiForms {

    /**
     * @brief Represents a coordinate in a text editor with line and column position
     *
     * Used by TextEditor to track cursor positions, selections, and other text coordinates.
     * Provides comparison operators for ordering and equality checking.
     */
    struct Coordinate {
        int line;
        int column;

        // Default constructor - initializes to invalid position
        Coordinate() : line(-1), column(-1) {}

        // Constructor with bounds checking
        Coordinate(int line, int column)
            : line(std::max(0, line))
            , column(std::max(0, column))
        {
        }

        // Equality operators
        bool operator==(const Coordinate& other) const {
            return line == other.line && column == other.column;
        }

        bool operator!=(const Coordinate& other) const {
            return !(*this == other);
        }

        // Comparison operators for ordering
        bool operator<(const Coordinate& other) const {
            if (line != other.line) {
                return line < other.line;
            }
            return column < other.column;
        }

        bool operator>(const Coordinate& other) const {
            if (line != other.line) {
                return line > other.line;
            }
            return column > other.column;
        }

        bool operator<=(const Coordinate& other) const {
            if (line != other.line) {
                return line < other.line;
            }
            return column <= other.column;
        }

        bool operator>=(const Coordinate& other) const {
            if (line != other.line) {
                return line > other.line;
            }
            return column >= other.column;
        }

        // Utility methods
        bool isValid() const {
            return line >= 0 && column >= 0;
        }

        // For use with std::unordered_map, std::unordered_set
        struct Hash {
            std::size_t operator()(const Coordinate& coord) const {
                return std::hash<int>{}(coord.line) ^ (std::hash<int>{}(coord.column) << 1);
            }
        };
    };

} // namespace ImGuiForms