#pragma once
#include <memory>        // For std::unique_ptr
#include <string>        // For std::string
#include <imgui.h>       // For ImVec4, etc.
#include "ImGuiForms/Core/Component.h"   // Base class
#include "ImGuiForms/Core/Size.h"        // For Size type
#include "ImGuiForms/Core/Rectangle.h"   // For Rectangle type

namespace ImGuiForms {

    /**
     * @brief A component that displays text
     *
     * Label is a simple leaf component that renders text using ImGui.
     * Demonstrates how to implement content-based sizing and text rendering.
     */
    class Label : public Component {
    private:
        std::string text;
        Size labelSize;
        ImVec4 textColor;

    public:
        /**
         * @brief Constructs a Label with text
         * @param text The text to display
         */
        Label(const std::string& text);

        /**
         * @brief Sets the text to display
         * @param newText The new text
         */
        void SetText(const std::string& newText);

        /**
         * @brief Gets the current text
         * @return The current text
         */
        const std::string& GetText() const;

        /**
         * @brief Sets the text color
         * @param color The new text color
         */
        void SetTextColor(const ImVec4& color);

        /**
         * @brief Gets the text color
         * @return The current text color
         */
        const ImVec4& GetTextColor() const;

        /**
         * @brief Sets the size of this label
         * @param size The size specification
         */
        void SetSize(const Size& size);

        /**
         * @brief Gets the size specification for this label
         * @return The size specification
         */
        Size GetSize() const override;

    protected:
        /**
         * @brief Renders the label text
         * @param contentRect The rectangle area to draw in
         */
        void UpdateInternal(const Rectangle& contentRect) override;

        /**
         * @brief Calculates the width needed for the text
         */
        int GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const override;

        /**
         * @brief Calculates the height needed for the text
         */
        int GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const override;
    };

    /**
     * @brief Helper function to create a Label
     * @param text The text to display
     * @return Unique pointer to the new Label
     */
    inline std::unique_ptr<Label> CreateLabel(const std::string& text) {
        return std::make_unique<Label>(text);
    }

    /**
     * @brief Helper function to create a Label with specific size
     * @param text The text to display
     * @param size The size specification for the label
     * @return Unique pointer to the new Label
     */
    inline std::unique_ptr<Label> CreateLabel(const std::string& text, const Size& size) {
        auto label = std::make_unique<Label>(text);
        label->SetSize(size);
        return label;
    }

    /**
     * @brief Helper function to create a colored Label
     * @param text The text to display
     * @param color The text color
     * @return Unique pointer to the new Label
     */
    inline std::unique_ptr<Label> CreateLabel(const std::string& text, const ImVec4& color) {
        auto label = std::make_unique<Label>(text);
        label->SetTextColor(color);
        return label;
    }

    /**
     * @brief Helper function to create a colored Label with specific size
     * @param text The text to display
     * @param size The size specification for the label
     * @param color The text color
     * @return Unique pointer to the new Label
     */
    inline std::unique_ptr<Label> CreateLabel(const std::string& text, const Size& size, const ImVec4& color) {
        auto label = std::make_unique<Label>(text);
        label->SetSize(size);
        label->SetTextColor(color);
        return label;
    }

} // namespace ImGuiForms