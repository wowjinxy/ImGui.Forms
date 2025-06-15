#pragma once
#include "ImGuiForms/Core/Component.h"
#include <string>

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
        Size labelSize = Size::Content();
        ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White by default

    public:
        /**
         * @brief Constructs a Label with text
         * @param labelText The text to display
         */
        explicit Label(const std::string& labelText = "")
            : text(labelText) {
        }

        /**
         * @brief Sets the text to display
         * @param newText The new text
         */
        void SetText(const std::string& newText) {
            text = newText;
        }

        /**
         * @brief Gets the current text
         * @return The current text
         */
        const std::string& GetText() const {
            return text;
        }

        /**
         * @brief Sets the text color
         * @param color The new text color
         */
        void SetTextColor(const ImVec4& color) {
            textColor = color;
        }

        /**
         * @brief Gets the text color
         * @return The current text color
         */
        const ImVec4& GetTextColor() const {
            return textColor;
        }

        /**
         * @brief Sets the size of this label
         * @param size The size specification
         */
        void SetSize(const Size& size) {
            labelSize = size;
        }

        /**
         * @brief Gets the size specification for this label
         * @return The size specification
         */
        Size GetSize() const override {
            return labelSize;
        }

    protected:
        /**
         * @brief Renders the label text
         * @param contentRect The rectangle area to draw in
         */
        void UpdateInternal(const Rectangle& contentRect) override {
            if (text.empty()) {
                return;
            }

            // Calculate text position (top-left aligned by default)
            ImVec2 textPos = contentRect.TopLeft();

            // Get the current window's draw list for custom drawing
            auto* drawList = ImGui::GetWindowDrawList();

            // Draw the text
            drawList->AddText(textPos, ImGui::GetColorU32(textColor), text.c_str());
        }

        /**
         * @brief Calculates the width needed for the text
         */
        int GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const override {
            if (text.empty()) {
                return 0;
            }

            // Calculate text size using ImGui
            ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
            return static_cast<int>(textSize.x);
        }

        /**
         * @brief Calculates the height needed for the text
         */
        int GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection = 1.0f) const override {
            if (text.empty()) {
                return 0;
            }

            // Calculate text size using ImGui
            ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
            return static_cast<int>(textSize.y);
        }
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

} // namespace ImGuiForms