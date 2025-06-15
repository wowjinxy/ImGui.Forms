/**
 * @file Component.cpp
 * @brief Implementation of the Component base class and Framework utilities
 */

#include "ImGuiForms/Core/Component.h"
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <typeinfo>

#ifdef __GNUG__
#include <cxxabi.h>
#include <cstdlib>
#endif

namespace ImGuiForms {

    // Static member definitions
    int Component::nextId = 0;
    bool Framework::initialized = false;

    // Internal framework state
    namespace Internal {
        // Track components for cleanup and validation
        static std::unordered_set<int> activeComponentIds;
        static std::unordered_set<int> frameUsedIds;

        // Performance tracking
        static size_t frameComponentCount = 0;
        static size_t totalComponentsCreated = 0;

        // Debug mode flag
        static bool debugMode = false;
    }

    // ============================================================================
    // Component Implementation
    // ============================================================================

    Component::Component() : componentId(++nextId) {
        Internal::activeComponentIds.insert(componentId);
        Internal::totalComponentsCreated++;

        if (Internal::debugMode) {
            std::cout << "[ImGuiForms] Component " << componentId << " created" << std::endl;
        }
    }

    Component::~Component() {
        Internal::activeComponentIds.erase(componentId);

        if (Internal::debugMode) {
            std::cout << "[ImGuiForms] Component " << componentId << " destroyed" << std::endl;
        }
    }

    void Component::Update(const Rectangle& contentRect) {
        // Handle visibility
        if (!visible) {
            tabInactive = false;
            return;
        }

        // Track this component as used this frame
        Internal::frameUsedIds.insert(componentId);
        Internal::frameComponentCount++;

        // Begin ImGui ID scope
        ImGui::PushID(componentId);

        try {
            // Apply component-specific styles
            ApplyStyles();

            // Render the component content
            UpdateInternal(contentRect);

            // Remove component-specific styles
            RemoveStyles();
        }
        catch (const std::exception& e) {
            // Log error but don't crash the application
            std::cerr << "[ImGuiForms] Error in component " << componentId
                << ": " << e.what() << std::endl;
        }

        // End ImGui ID scope
        ImGui::PopID();

        // Draw border if requested
        if (showBorder) {
            auto* drawList = ImGui::GetWindowDrawList();
            ImVec4 borderColor = ImGui::GetStyleColorVec4(ImGuiCol_Border);
            drawList->AddRect(contentRect.TopLeft(), contentRect.BottomRight(),
                ImGui::GetColorU32(borderColor));
        }

        // Handle drag and drop
        if (allowDragDrop && enabled) {
            HandleDragDrop(contentRect);
        }

        tabInactive = false;
    }

    void Component::HandleDragDrop(const Rectangle& contentRect) {
        // This is a placeholder implementation
        // In a real implementation, you'd integrate with your platform's drag-drop system

        // Check if mouse is over this component
        ImVec2 mousePos = ImGui::GetMousePos();
        if (contentRect.Contains(mousePos)) {
            // Example: Handle file drops
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILES")) {
                    // Handle the dropped files
                    std::vector<DragDropEvent> events;

                    // Parse the payload (this would be platform-specific)
                    const char* data = static_cast<const char*>(payload->Data);
                    std::string filePath(data, payload->DataSize);

                    events.push_back({ filePath });
                    OnDragDrop(events);
                }
                ImGui::EndDragDropTarget();
            }
        }
    }

    int Component::GetDimension(const SizeValue& sizeValue, int maxValue, float correction) {
        if (sizeValue.IsAbsolute()) {
            return static_cast<int>(std::min(sizeValue.value, static_cast<float>(maxValue)));
        }

        return static_cast<int>(std::floor(sizeValue.value * maxValue * correction));
    }

    void Component::OnDragDrop(const std::vector<DragDropEvent>& events) {
        if (onDragDrop) {
            onDragDrop(this, events);
        }
    }

    bool Component::ValidateHierarchy() const {
        // Basic validation - check if component ID is still active
        return Internal::activeComponentIds.find(componentId) != Internal::activeComponentIds.end();
    }

    // ============================================================================
    // Framework Implementation
    // ============================================================================

    void Framework::Initialize() {
        if (initialized) {
            std::cerr << "[ImGuiForms] Framework already initialized!" << std::endl;
            return;
        }

        std::cout << "[ImGuiForms] Initializing framework v" << Version::String << std::endl;

        // Initialize internal systems
        Internal::activeComponentIds.clear();
        Internal::frameUsedIds.clear();
        Internal::frameComponentCount = 0;
        Internal::totalComponentsCreated = 0;

        // TODO: Initialize other systems as they're implemented
        // - Theme manager
        // - Font manager  
        // - Event dispatcher
        // - Resource manager

        initialized = true;
        std::cout << "[ImGuiForms] Framework initialized successfully" << std::endl;
    }

    void Framework::Shutdown() {
        if (!initialized) {
            std::cerr << "[ImGuiForms] Framework not initialized!" << std::endl;
            return;
        }

        std::cout << "[ImGuiForms] Shutting down framework..." << std::endl;

        // Print statistics
        std::cout << "[ImGuiForms] Statistics:" << std::endl;
        std::cout << "  Total components created: " << Internal::totalComponentsCreated << std::endl;
        std::cout << "  Active components: " << Internal::activeComponentIds.size() << std::endl;

        // Warn about leaked components
        if (!Internal::activeComponentIds.empty()) {
            std::cerr << "[ImGuiForms] Warning: " << Internal::activeComponentIds.size()
                << " components still active at shutdown!" << std::endl;

            if (Internal::debugMode) {
                std::cerr << "  Active component IDs: ";
                for (int id : Internal::activeComponentIds) {
                    std::cerr << id << " ";
                }
                std::cerr << std::endl;
            }
        }

        // Cleanup internal state
        Internal::activeComponentIds.clear();
        Internal::frameUsedIds.clear();

        // TODO: Cleanup other systems

        initialized = false;
        std::cout << "[ImGuiForms] Framework shutdown complete" << std::endl;
    }

    void Framework::BeginFrame() {
        if (!initialized) {
            std::cerr << "[ImGuiForms] Framework not initialized! Call Framework::Initialize() first." << std::endl;
            return;
        }

        // Reset frame-specific tracking
        Internal::frameUsedIds.clear();
        Internal::frameComponentCount = 0;

        // TODO: Frame-specific initialization
        // - Reset input state
        // - Update animations
        // - Prepare theme system
    }

    void Framework::EndFrame() {
        if (!initialized) {
            return;
        }

        // TODO: Frame-specific cleanup
        // - Cleanup unused resources
        // - Update performance metrics
        // - Process deferred operations

        // Optional: Garbage collection of unused IDs
        // (In a more sophisticated implementation, you might want to
        //  recycle component IDs that haven't been used in many frames)
    }

    void Framework::SetDebugMode(bool enabled) {
        Internal::debugMode = enabled;

        if (enabled) {
            std::cout << "[ImGuiForms] Debug mode enabled" << std::endl;
        }
        else {
            std::cout << "[ImGuiForms] Debug mode disabled" << std::endl;
        }
    }

    bool Framework::IsDebugMode() {
        return Internal::debugMode;
    }

    Framework::Statistics Framework::GetStatistics() {
        Statistics stats;
        stats.activeComponents = Internal::activeComponentIds.size();
        stats.totalComponentsCreated = Internal::totalComponentsCreated;
        stats.frameComponentCount = Internal::frameComponentCount;
        stats.memoryUsageBytes = sizeof(Component) * Internal::activeComponentIds.size();

        return stats;
    }

    void Framework::PrintStatistics() {
        auto stats = GetStatistics();

        std::cout << "[ImGuiForms] Runtime Statistics:" << std::endl;
        std::cout << "  Active components: " << stats.activeComponents << std::endl;
        std::cout << "  Total created: " << stats.totalComponentsCreated << std::endl;
        std::cout << "  This frame: " << stats.frameComponentCount << std::endl;
        std::cout << "  Estimated memory: " << stats.memoryUsageBytes << " bytes" << std::endl;
    }

    // ============================================================================
    // Utility Functions
    // ============================================================================

    namespace Utils {

        std::string ComponentTypeToString(const Component* component) {
            if (!component) {
                return "nullptr";
            }

            // Use RTTI to get component type name
            // This is mainly for debugging purposes
            const char* typeName = typeid(*component).name();

            // Try to demangle the name (platform-specific)
#ifdef __GNUG__
            int status = 0;
            char* demangled = abi::__cxa_demangle(typeName, 0, 0, &status);
            if (status == 0 && demangled) {
                std::string result(demangled);
                free(demangled);

                // Extract just the class name (remove namespace)
                size_t lastColonPos = result.find_last_of(':');
                if (lastColonPos != std::string::npos) {
                    result = result.substr(lastColonPos + 1);
                }

                return result;
            }
#endif

            // Fallback to mangled name
            return std::string(typeName);
        }

        void DumpComponentHierarchy(const Component* root, int depth) {
            if (!root) {
                return;
            }

            // Print indentation
            for (int i = 0; i < depth; ++i) {
                std::cout << "  ";
            }

            // Print component info
            std::cout << "- " << ComponentTypeToString(root)
                << " (ID: " << root->GetId()
                << ", Visible: " << (root->visible ? "true" : "false")
                << ", Enabled: " << (root->enabled ? "true" : "false")
                << ")" << std::endl;

            // TODO: Recursively dump children when container components are implemented
            // For now, this just prints the single component
        }

        bool ValidateComponentTree(const Component* root) {
            if (!root) {
                return true;
            }

            // Validate this component
            if (!root->ValidateHierarchy()) {
                std::cerr << "[ImGuiForms] Invalid component found: ID " << root->GetId() << std::endl;
                return false;
            }

            // TODO: Validate children when container components are implemented

            return true;
        }

    } // namespace Utils

} // namespace ImGuiForms