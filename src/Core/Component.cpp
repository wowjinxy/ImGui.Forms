#include "ImGuiForms/Core/Component.h"
#include "ImGuiForms/Core/Point.h"
#include <algorithm>

namespace ImGuiForms {

    Component::Component()
        : enabled(true)
        , visible(true)
    {
    }

    Component::~Component() {
        // Default destructor
    }

    void Component::SetEnabled(bool enabled) {
        this->enabled = enabled;
    }

    bool Component::IsEnabled() const {
        return enabled;
    }

    void Component::SetVisible(bool visible) {
        this->visible = visible;
    }

    bool Component::IsVisible() const {
        return visible;
    }

    void Component::SetDragDropCallback(const DragDropCallback& callback) {
        onDragDrop = callback;
    }

    void Component::Update(const Rectangle& contentRect) {
        if (!visible) {
            return;
        }

        // Handle mouse input
        bool wasMouseHandled = HandleMouseInput(contentRect);

        // Handle drag and drop if enabled
        if (enabled) {
            HandleDragDrop();
        }

        // Call the derived class implementation
        if (enabled) {
            UpdateInternal(contentRect);
        }

        // Debug rendering (if needed)
#ifdef _DEBUG
// Uncomment to enable debug borders:
// RenderDebugBorder(contentRect);
#endif
    }

    int Component::GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection) const {
        // Default implementation - override in derived classes
        return static_cast<int>(static_cast<float>(parentWidth) * layoutCorrection);
    }

    int Component::GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection) const {
        // Default implementation - override in derived classes  
        return static_cast<int>(static_cast<float>(parentHeight) * layoutCorrection);
    }

    float Component::GetDimension(const Size& sizeSpec, float parentDimension, float contentDimension) const {
        // Handle special size specifications
        if (sizeSpec == Size::Content) {
            return contentDimension;
        }
        else if (sizeSpec == Size::Parent) {
            return parentDimension;
        }
        else if (sizeSpec == Size::Fill) {
            return parentDimension;
        }
        else {
            // Use explicit size (use width as the dimension value)
            return sizeSpec.width;
        }
    }

    void Component::RenderDebugBorder(const Rectangle& rect) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        if (drawList == nullptr) {
            return;
        }

        // Convert Rectangle to ImVec2 for ImGui
        ImVec2 topLeft(rect.x, rect.y);
        ImVec2 bottomRight(rect.x + rect.width, rect.y + rect.height);

        // Draw debug border
        ImU32 borderColor = IM_COL32(255, 0, 0, 128); // Semi-transparent red
        drawList->AddRect(topLeft, bottomRight, borderColor, 0.0f, ImDrawFlags_None, 1.0f);
    }

    bool Component::HandleMouseInput(const Rectangle& rect) {
        if (!enabled) {
            return false;
        }

        ImVec2 mousePos = ImGui::GetMousePos();

        // Convert ImVec2 to Point for Contains check
        Point mousePt(mousePos.x, mousePos.y);

        bool isHovered = rect.Contains(mousePt);

        if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            // Handle mouse click
            return true;
        }

        return isHovered;
    }

    void Component::HandleDragDrop() {
        if (!onDragDrop) {
            return;
        }

        // Handle drag and drop operations
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_TEXT")) {
                std::string droppedText(static_cast<const char*>(payload->Data), payload->DataSize);
                onDragDrop(droppedText);
            }
            ImGui::EndDragDropTarget();
        }
    }

} // namespace ImGuiForms