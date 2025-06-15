#include "ImGuiForms/Components/Panel.h"

namespace ImGuiForms {

    Panel::Panel(std::unique_ptr<Component> content)
        : content(std::move(content))
        , panelSize(Size::Parent)
    {
    }

    void Panel::SetContent(std::unique_ptr<Component> content) {
        this->content = std::move(content);
    }

    Component* Panel::GetContent() const {
        return content.get();
    }

    void Panel::SetSize(const Size& size) {
        panelSize = size;
    }

    Size Panel::GetSize() const {
        return panelSize;
    }

    void Panel::UpdateInternal(const Rectangle& contentRect) {
        if (content && IsVisible()) {
            // Update the content component with the full rectangle
            content->Update(contentRect);
        }
    }

    int Panel::GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection) const {
        if (content) {
            return content->GetContentWidth(parentWidth, parentHeight, layoutCorrection);
        }
        return 0;
    }

    int Panel::GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection) const {
        if (content) {
            return content->GetContentHeight(parentWidth, parentHeight, layoutCorrection);
        }
        return 0;
    }

} // namespace ImGuiForms