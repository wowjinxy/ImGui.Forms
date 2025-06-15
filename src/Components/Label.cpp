#include "ImGuiForms/Components/Label.h"

namespace ImGuiForms {

    Label::Label(const std::string& text)
        : text(text)
        , labelSize(Size::Content)
        , textColor(1.0f, 1.0f, 1.0f, 1.0f)  // White by default
    {
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

        // Use the TopLeft method and convert to ImVec2
        ImVec2 textPos = contentRect.TopLeft().ToImVec2();

        // Get the current window's draw list
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        if (drawList == nullptr) {
            return;
        }

        // Convert color to ImU32
        ImU32 color = IM_COL32(
            static_cast<int>(textColor.x * 255),
            static_cast<int>(textColor.y * 255),
            static_cast<int>(textColor.z * 255),
            static_cast<int>(textColor.w * 255)
        );

        // Draw the text
        drawList->AddText(textPos, color, text.c_str());
    }

    int Label::GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection) const {
        if (text.empty()) {
            return 0;
        }

        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        return static_cast<int>(textSize.x * layoutCorrection);
    }

    int Label::GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection) const {
        if (text.empty()) {
            return 0;
        }

        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        return static_cast<int>(textSize.y * layoutCorrection);
    }

} // namespace ImGuiForms