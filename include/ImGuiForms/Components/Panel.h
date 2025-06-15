#pragma once
#include <memory>        // For std::unique_ptr
#include <string>        // For std::string
#include <imgui.h>       // For ImVec4, etc.
#include "ImGuiForms/Core/Component.h"   // Base class
#include "ImGuiForms/Core/Size.h"        // For Size type
#include "ImGuiForms/Core/Rectangle.h"   // For Rectangle type

namespace ImGuiForms {

    /**
     * @brief A simple container component that holds a single child component
     *
     * Panel is the most basic container - it simply renders its child component
     * within its bounds. Useful for grouping and applying consistent sizing.
     */
    class Panel : public Component {
    private:
        std::unique_ptr<Component> content;
        Size panelSize = Size::Parent();

    public:
        /**
         * @brief Constructs a Panel with optional content
         * @param child The child component to contain (takes ownership)
         */
        Panel(std::unique_ptr<Component> content);

        /**
         * @brief Sets the child component (takes ownership)
         * @param child The new child component
         */
        void SetContent(std::unique_ptr<Component> child);

        /**
         * @brief Gets a pointer to the child component (retains ownership)
         * @return Pointer to child component, or nullptr if no child
         */
        Component* GetContent();

        /**
         * @brief Sets the size of this panel
         * @param size The size specification
         */
        void SetSize(const Size& size);

        /**
         * @brief Gets the size specification for this panel
         * @return The size specification
         */
        Size GetSize();

    protected:
        /**
         * @brief Renders the panel by rendering its child component
         * @param contentRect The rectangle area to draw in
         */
        void UpdateInternal(const Rectangle& contentRect) override {
            if (content && content->visible) {
                content->Update(contentRect);
            }
        }

        /**
         * @brief Propagates tab inactive state to child component
         */
        void SetTabInactiveCore() override {
            if (content) {
                content->SetTabInactive();
            }
        }

        /**
         * @brief Calculates content width if size is content-aligned
         */
        int GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const override {
            if (content) {
                return content->GetWidth(parentWidth, parentHeight, layoutCorrection);
            }
            return 0;
        }

        /**
         * @brief Calculates content height if size is content-aligned
         */
        int GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const override {
            if (content) {
                return content->GetHeight(parentWidth, parentHeight, layoutCorrection);
            }
            return 0;
        }
    };

    /**
     * @brief Helper function to create a Panel with content
     * @param child The child component (takes ownership)
     * @return Unique pointer to the new Panel
     */
    inline std::unique_ptr<Panel> CreatePanel(std::unique_ptr<Component> child = nullptr) {
        return std::make_unique<Panel>(std::move(child));
    }

    /**
     * @brief Helper function to create a Panel with specific size
     * @param size The size specification for the panel
     * @param child Optional child component (takes ownership)
     * @return Unique pointer to the new Panel
     */
    inline std::unique_ptr<Panel> CreatePanel(const Size& size, std::unique_ptr<Component> child = nullptr) {
        auto panel = std::make_unique<Panel>(std::move(child));
        panel->SetSize(size);
        return panel;
    }

} // namespace ImGuiForms