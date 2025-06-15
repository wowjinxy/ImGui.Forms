#pragma once
#include <memory>        // For std::unique_ptr
#include <string>        // For std::string
#include <imgui.h>       // For ImVec4, etc.
#include "ImGuiForms/Core/Component.h"   // Base class
#include "ImGuiForms/Core/Size.h"        // For Size type
#include "ImGuiForms/Core/Rectangle.h"   // For Rectangle type

namespace ImGuiForms {

    /**
     * @brief A container component that holds other components
     *
     * Panel provides a simple container for other UI components.
     * It can manage layout and rendering of child components.
     */
    class Panel : public Component {
    private:
        std::unique_ptr<Component> content;
        Size panelSize;

    public:
        /**
         * @brief Constructs a Panel with content
         * @param content The component to contain
         */
        Panel(std::unique_ptr<Component> content);

        /**
         * @brief Sets the content of this panel
         * @param content The new content component
         */
        void SetContent(std::unique_ptr<Component> content);

        /**
         * @brief Gets the content component
         * @return Pointer to the content component
         */
        Component* GetContent() const;

        /**
         * @brief Sets the size of this panel
         * @param size The size specification
         */
        void SetSize(const Size& size);

        /**
         * @brief Gets the size specification for this panel
         * @return The size specification
         */
        Size GetSize() const override;

    protected:
        /**
         * @brief Renders the panel and its content
         * @param contentRect The rectangle area to draw in
         */
        void UpdateInternal(const Rectangle& contentRect) override;

        /**
         * @brief Calculates the width needed for the panel content
         */
        int GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const override;

        /**
         * @brief Calculates the height needed for the panel content
         */
        int GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const override;
    };

    /**
     * @brief Helper function to create a Panel
     * @param content The content component
     * @return Unique pointer to the new Panel
     */
    inline std::unique_ptr<Panel> CreatePanel(std::unique_ptr<Component> content) {
        return std::make_unique<Panel>(std::move(content));
    }

    /**
     * @brief Helper function to create a Panel with specific size
     * @param size The size specification for the panel
     * @param content The content component
     * @return Unique pointer to the new Panel
     */
    inline std::unique_ptr<Panel> CreatePanel(const Size& size, std::unique_ptr<Component> content) {
        auto panel = std::make_unique<Panel>(std::move(content));
        panel->SetSize(size);
        return panel;
    }

} // namespace ImGuiForms