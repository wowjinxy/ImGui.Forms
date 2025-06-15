/**
 * @file Panel.cpp
 * @brief Implementation of the Panel container component
 */

#include "ImGuiForms/Components/Panel.h"
#include <algorithm>

namespace ImGuiForms {

    Panel::Panel(std::unique_ptr<Component> child)
        : content(std::move(child)), panelSize(Size::Parent()) {
    }

    void Panel::SetContent(std::unique_ptr<Component> child) {
        content = std::move(child);
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
        if (content && content->visible) {
            content->Update(contentRect);
        }
    }

    void Panel::SetTabInactiveCore() {
        if (content) {
            content->SetTabInactive();
        }
    }

    int Panel::GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection) const {
        if (content) {
            return content->GetWidth(parentWidth, parentHeight, layoutCorrection);
        }
        return 0;
    }

    int Panel::GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection) const {
        if (content) {
            return content->GetHeight(parentWidth, parentHeight, layoutCorrection);
        }
        return 0;
    }

    // Factory functions
    std::unique_ptr<Panel> CreatePanel(std::unique_ptr<Component> child) {
        return std::make_unique<Panel>(std::move(child));
    }

    std::unique_ptr<Panel> CreatePanel(const Size& size, std::unique_ptr<Component> child) {
        auto panel = std::make_unique<Panel>(std::move(child));
        panel->SetSize(size);
        return panel;
    }

} // namespace ImGuiForms