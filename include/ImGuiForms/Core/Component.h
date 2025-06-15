#pragma once
#include <memory>        // For std::unique_ptr
#include <string>        // For std::string
#include <functional>    // For std::function
#include <imgui.h>       // For ImVec4, etc.
#include "ImGuiForms/Core/Size.h"        // For Size type
#include "ImGuiForms/Core/Rectangle.h"   // For Rectangle type

namespace ImGuiForms {

    /**
     * @brief Base class for all UI components
     *
     * Component provides the core functionality for UI elements including
     * layout, rendering, and event handling.
     */
    class Component {
    public:
        // Event callback type
        using DragDropCallback = std::function<void(const std::string&)>;

    private:
        bool enabled = true;
        bool visible = true;
        DragDropCallback onDragDrop;

    public:
        /**
         * @brief Constructor
         */
        Component();

        /**
         * @brief Virtual destructor
         */
        virtual ~Component();

        /**
         * @brief Sets whether this component is enabled
         */
        void SetEnabled(bool enabled);

        /**
         * @brief Gets whether this component is enabled
         */
        bool IsEnabled() const;

        /**
         * @brief Sets whether this component is visible
         */
        void SetVisible(bool visible);

        /**
         * @brief Gets whether this component is visible
         */
        bool IsVisible() const;

        /**
         * @brief Gets the size specification for this component
         */
        virtual Size GetSize() const = 0;

        /**
         * @brief Sets the drag and drop callback
         */
        void SetDragDropCallback(const DragDropCallback& callback);

        /**
         * @brief Main update method - handles rendering and input
         */
        void Update(const Rectangle& contentRect);

        /**
         * @brief Gets the width needed for this component's content
         */
        virtual int GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const;

        /**
         * @brief Gets the height needed for this component's content
         */
        virtual int GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const;

    protected:
        /**
         * @brief Internal update implementation - override in derived classes
         */
        virtual void UpdateInternal(const Rectangle& contentRect) = 0;

        /**
         * @brief Helper method to calculate dimension based on size specification
         */
        float GetDimension(const Size& sizeSpec, float parentDimension, float contentDimension) const;

    private:
        /**
         * @brief Handles debug rendering
         */
        void RenderDebugBorder(const Rectangle& rect);

        /**
         * @brief Handles mouse input
         */
        bool HandleMouseInput(const Rectangle& rect);

        /**
         * @brief Handles drag and drop
         */
        void HandleDragDrop();
    };

} // namespace ImGuiForms