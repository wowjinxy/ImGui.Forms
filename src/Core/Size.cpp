#include "ImGuiForms/Core/Size.h"
#include <string>

namespace ImGuiForms {

    // Define static size constants
    // Use special values to indicate different sizing behaviors
    const Size Size::Content = Size(-1.0f, -1.0f);   // Negative values indicate content-based sizing
    const Size Size::Parent = Size(-2.0f, -2.0f);    // Use parent's size
    const Size Size::Fill = Size(-3.0f, -3.0f);      // Fill available space

    std::string Size::ToString() const {
        if (*this == Content) {
            return "Size::Content";
        }
        else if (*this == Parent) {
            return "Size::Parent";
        }
        else if (*this == Fill) {
            return "Size::Fill";
        }
        else {
            return "Size(" + std::to_string(width) + ", " + std::to_string(height) + ")";
        }
    }

} // namespace ImGuiForms