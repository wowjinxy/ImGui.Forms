#pragma once
#include <imgui.h>
#include <functional>
#include <cmath>
#include <algorithm>

// Forward declarations
namespace ImGuiForms {
    struct Size;
    struct SizeValue;
    struct Rectangle;
}

// Include our types
#include "Size.h"
#include "Rectangle.h"
#include <string>

namespace ImGuiForms {

    // Forward declarations for event system
    struct DragDropEvent;

    /**
     * @brief Base class for all UI components in ImGuiForms
     *
     * Provides the foundation for the object-oriented wrapper around ImGui.
     * All UI elements derive from this class and implement the UpdateInternal method.
     */
    class Component {
    private:
        bool tabInactive = false;
        static int nextId;  // Simple ID counter (much simpler than C# version)

    protected:
        int componentId;

    public:
        // Properties
        bool visible = true;
        bool enabled = true;
        bool allowDragDrop = false;
        bool showBorder = false;

        // Event types
        using DragDropHandler = std::function<void(Component*, const std::vector<DragDropEvent>&)>;

        // Events
        DragDropHandler onDragDrop;

        // Constructor
        Component() : componentId(++nextId) {}

        // Virtual destructor for proper cleanup
        virtual ~Component() = default;

        // ID accessor
        int GetId() const { return componentId; }

        /**
         * @brief Gets the size specification for this component
         * @return Size specification (absolute, relative, or content-aligned)
         */
        virtual Size GetSize() const = 0;

        /**
         * @brief Gets the final calculated width of this component
         * @param parentWidth Width of the parent container
         * @param parentHeight Height of the parent container
         * @param layoutCorrection Correction factor for layouts
         * @return Final width in pixels
         */
        int GetWidth(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const {
            auto size = GetSize();
            return size.width.IsContentAligned() ?
                GetContentWidth(parentWidth, parentHeight, layoutCorrection) :
                GetDimension(size.width, parentWidth, layoutCorrection);
        }

        /**
         * @brief Gets the final calculated height of this component
         * @param parentWidth Width of the parent container
         * @param parentHeight Height of the parent container
         * @param layoutCorrection Correction factor for layouts
         * @return Final height in pixels
         */
        int GetHeight(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const {
            auto size = GetSize();
            return size.height.IsContentAligned() ?
                GetContentHeight(parentWidth, parentHeight, layoutCorrection) :
                GetDimension(size.height, parentHeight, layoutCorrection);
        }

        /**
         * @brief Updates and renders this component
         * @param contentRect The rectangle area to draw the component in
         */
        void Update(const Rectangle& contentRect) {
            // Handle visibility
            if (!visible) {
                tabInactive = false;
                return;
            }

            // Begin ImGui ID scope
            ImGui::PushID(componentId);

            // Apply component-specific styles
            ApplyStyles();

            // Render the component content
            UpdateInternal(contentRect);

            // Remove component-specific styles
            RemoveStyles();

            // End ImGui ID scope
            ImGui::PopID();

            // Draw border if requested
            if (showBorder) {
                auto* drawList = ImGui::GetWindowDrawList();
                drawList->AddRect(contentRect.TopLeft(), contentRect.BottomRight(),
                    ImGui::GetColorU32(ImGuiCol_Border));
            }

            // Handle drag and drop
            if (allowDragDrop && enabled) {
                // TODO: Implement drag drop detection
                // This would need to be provided by the application framework
            }

            tabInactive = false;
        }

        /**
         * @brief Checks if the current ImGui item is hovered
         */
        bool IsHovered() const {
            return ImGui::IsItemHovered();
        }

        /**
         * @brief Checks if the current ImGui item is active
         */
        bool IsActive() const {
            return ImGui::IsItemActive() && ImGui::IsItemHovered();
        }

        /**
         * @brief Checks if this component is on an inactive tab page
         */
        bool IsTabInactive() const {
            return tabInactive;
        }

        /**
         * @brief Marks this component as being on an inactive tab page
         */
        void SetTabInactive() {
            tabInactive = true;
            SetTabInactiveCore();
        }

        /**
         * @brief Calculates pixel value from SizeValue
         * @param sizeValue The size value to calculate
         * @param maxValue The maximum available space
         * @param correction Layout correction factor
         * @return Calculated pixel value
         */
        static int GetDimension(const SizeValue& sizeValue, int maxValue, float correction = 1.0f) {
            if (sizeValue.IsAbsolute()) {
                return static_cast<int>(std::min(sizeValue.value, static_cast<float>(maxValue)));
            }

            return static_cast<int>(std::floor(sizeValue.value * maxValue * correction));
        }

    protected:
        /**
         * @brief Override this to implement component-specific rendering
         * @param contentRect The rectangle area to draw the component in
         */
        virtual void UpdateInternal(const Rectangle& contentRect) = 0;

        /**
         * @brief Override this to apply component-specific ImGui styles before rendering
         */
        virtual void ApplyStyles() {}

        /**
         * @brief Override this to remove component-specific ImGui styles after rendering
         */
        virtual void RemoveStyles() {}

        /**
         * @brief Override this to propagate tab inactive state to child components
         */
        virtual void SetTabInactiveCore() {}

        /**
         * @brief Override this to calculate content-based width
         * @param parentWidth Width of the parent container
         * @param parentHeight Height of the parent container
         * @param layoutCorrection Layout correction factor
         * @return Content-based width in pixels
         */
        virtual int GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const {
            return 0;
        }

        /**
         * @brief Override this to calculate content-based height
         * @param parentWidth Width of the parent container
         * @param parentHeight Height of the parent container
         * @param layoutCorrection Layout correction factor
         * @return Content-based height in pixels
         */
        virtual int GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const {
            return 0;
        }

    private:
        /**
         * @brief Internal method for setting tab inactive state
         */
        void SetTabInactiveInternal() {
            SetTabInactive();
        }

        /**
         * @brief Triggers the drag drop event
         * @param events The drag drop events to handle
         */
        void OnDragDrop(const std::vector<DragDropEvent>& events) {
            if (onDragDrop) {
                onDragDrop(this, events);
            }
        }

        friend class TabControl;  // Allow TabControl to call SetTabInactiveInternal
    };

    // Static member definition
    inline int Component::nextId = 0;

    // Placeholder for DragDropEvent - would need to be implemented based on platform
    struct DragDropEvent {
        std::string filePath;  // Example - actual implementation depends on platform
    };

} // namespace ImGuiForms