/**
 * @file Label.cpp
 * @brief Implementation of the Label text display component
 */

#include "ImGuiForms/Components/Label.h"
#include <imgui.h>

namespace ImGuiForms {

    Label::Label(const std::string& labelText)
        : text(labelText), labelSize(Size::Content()), textColor(1.0f, 1.0f, 1.0f, 1.0f) {
    }

    void Label::SetText(const std::string& newText) {
        text = newText;
    }

    const std::string& Label::GetText() const {
        return text;
    }

    void Label::SetTextColor(const ImVec4& color) {
        textColor = color;
    }

    const ImVec4& Label::GetTextColor() const {
        return textColor;
    }

    void Label::SetSize(const Size& size) {
        labelSize = size;
    }

    Size Label::GetSize() const {
        return labelSize;
    }

    void Label::UpdateInternal(const Rectangle& contentRect) {
        if (text.empty()) {
            return;
        }

        // Calculate text position (top-left aligned by default)
        ImVec2 textPos = contentRect.TopLeft();

        // Get the current window's draw list for custom drawing
        auto* drawList = ImGui::GetWindowDrawList();

        // Convert color to ImGui format
        ImU32 colorU32 = ImGui::GetColorU32(textColor);

        // Draw the text
        drawList->AddText(textPos, colorU32, text.c_str());
    }

    int Label::GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection) const {
        if (text.empty()) {
            return 0;
        }

        // Calculate text size using ImGui
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        return static_cast<int>(textSize.x);
    }

    int Label::GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection) const {
        if (text.empty()) {
            return 0;
        }

        // For multi-line text, we need to calculate wrapped height
        if (text.find('\n') != std::string::npos) {
            // Use ImGui's text wrapping calculation
            ImVec2 textSize = ImGui::CalcTextSize(text.c_str(), nullptr, false,
                static_cast<float>(parentWidth));
            return static_cast<int>(textSize.y);
        }

        // Single line text
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        return static_cast<int>(textSize.y);
    }

    // Factory functions
    std::unique_ptr<Label> CreateLabel(const std::string& text) {
        return std::make_unique<Label>(text);
    }

    std::unique_ptr<Label> CreateLabel(const std::string& text, const Size& size) {
        auto label = std::make_unique<Label>(text);
        label->SetSize(size);
        return label;
    }

    std::unique_ptr<Label> CreateLabel(const std::string& text, const ImVec4& color) {
        auto label = std::make_unique<Label>(text);
        label->SetTextColor(color);
        return label;
    }

    std::unique_ptr<Label> CreateLabel(const std::string& text, const Size& size, const ImVec4& color) {
        auto label = std::make_unique<Label>(text);
        label->SetSize(size);
        label->SetTextColor(color);
        return label;
    }

} // namespace ImGuiForms