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
        void Update(const Rectangle& contentRect);

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
         * @brief Validates that this component is in a valid state
         * @return true if component is valid, false otherwise
         */
        bool ValidateHierarchy() const;

        /**
         * @brief Calculates pixel value from SizeValue
         * @param sizeValue The size value to calculate
         * @param maxValue The maximum available space
         * @param correction Layout correction factor
         * @return Calculated pixel value
         */
        static int GetDimension(const SizeValue& sizeValue, int maxValue, float correction = 1.0f);

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
        void OnDragDrop(const std::vector<DragDropEvent>& events);

        /**
         * @brief Handles platform-specific drag and drop detection
         * @param contentRect The component's content rectangle
         */
        void HandleDragDrop(const Rectangle& contentRect);

        friend class TabControl;  // Allow TabControl to call SetTabInactiveInternal
    };

    // Static member declaration (definition in .cpp)
    // Note: No longer inline since it's defined in Component.cpp

    // Placeholder for DragDropEvent - would need to be implemented based on platform
    struct DragDropEvent {
        std::string filePath;  // Example - actual implementation depends on platform
        // Could also include:
        // - File type/MIME type
        // - File size
        // - Mouse position
        // - Modifier keys
    };

    /**
     * @brief Utility functions for component debugging and inspection
     */
    namespace Utils {
        /**
         * @brief Get a human-readable string representation of a component's type
         * @param component The component to inspect
         * @return String representation of the component type
         */
        std::string ComponentTypeToString(const Component* component);

        /**
         * @brief Print the component hierarchy to console for debugging
         * @param root The root component to start from
         * @param depth Current depth in the hierarchy (for indentation)
         */
        void DumpComponentHierarchy(const Component* root, int depth = 0);

        /**
         * @brief Validate that a component tree is in a consistent state
         * @param root The root component to validate
         * @return true if the tree is valid, false if corruption is detected
         */
        bool ValidateComponentTree(const Component* root);
    }

} // namespace ImGuiForms