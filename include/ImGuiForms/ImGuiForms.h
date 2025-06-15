/**
 * @file ImGuiForms.h
 * @brief Main header for the ImGuiForms C++ framework
 * @version 1.0.0
 * @author ImGuiForms Community
 *
 * Object-oriented C++ framework around Dear ImGui, providing WinForms-style
 * declarative UI development with modern C++ features and ImGui's performance.
 *
 * Usage:
 * ```cpp
 * #include <ImGuiForms/ImGuiForms.h>
 * using namespace ImGuiForms;
 *
 * auto panel = CreatePanel(Size::Parent());
 * auto label = CreateLabel("Hello, ImGuiForms!");
 * panel->SetContent(std::move(label));
 * ```
 *
 * This single header includes the entire public API of ImGuiForms.
 */

#pragma once

 // Ensure ImGui is available
#include <imgui.h>

// Version information
#define IMGUIFORMS_VERSION_MAJOR 1
#define IMGUIFORMS_VERSION_MINOR 0
#define IMGUIFORMS_VERSION_PATCH 0
#define IMGUIFORMS_VERSION "1.0.0"

// Core framework types
#include "Core/Coordinate.h"
#include "Core/Size.h"
#include "Core/Rectangle.h"
#include "Core/Component.h"

// Basic components
#include "Components/Panel.h"
#include "Components/Label.h"

// Layout components (when implemented)
// #include "Layouts/StackLayout.h"
// #include "Layouts/TableLayout.h"
// #include "Layouts/ZLayout.h"

// Text editing components (when implemented)
// #include "Text/TextEditor.h"

// Event system (when implemented)
// #include "Events/EventArgs.h"
// #include "Events/MouseEvents.h"
// #include "Events/KeyboardEvents.h"
// #include "Events/DragDropEvents.h"

// Styling system (when implemented)
// #include "Styling/Theme.h"
// #include "Styling/Colors.h"
// #include "Styling/Fonts.h"

// Utility functions (when implemented)
// #include "Utils/IdManager.h"
// #include "Utils/TextMeasure.h"
// #include "Utils/MathUtils.h"

namespace ImGuiForms {

    /**
     * @brief Framework version information
     */
    struct Version {
        static constexpr int Major = IMGUIFORMS_VERSION_MAJOR;
        static constexpr int Minor = IMGUIFORMS_VERSION_MINOR;
        static constexpr int Patch = IMGUIFORMS_VERSION_PATCH;
        static constexpr const char* String = IMGUIFORMS_VERSION;

        /**
         * @brief Get the full version as a string
         */
        static const char* GetVersionString() {
            return String;
        }

        /**
         * @brief Check if the current version is at least the specified version
         */
        static constexpr bool IsAtLeast(int major, int minor, int patch = 0) {
            return (Major > major) ||
                (Major == major && Minor > minor) ||
                (Major == major && Minor == minor && Patch >= patch);
        }
    };

    /**
     * @brief Framework initialization and cleanup
     */
    class Framework {
    public:
        /**
         * @brief Initialize the ImGuiForms framework
         *
         * Call this once at application startup, after ImGui initialization.
         * Sets up internal systems and prepares the framework for use.
         */
        static void Initialize() {
            // TODO: Initialize ID management, theming, etc.
            initialized = true;
        }

        /**
         * @brief Cleanup the ImGuiForms framework
         *
         * Call this at application shutdown to clean up resources.
         */
        static void Shutdown() {
            // TODO: Cleanup ID management, release resources
            initialized = false;
        }

        /**
         * @brief Check if the framework is initialized
         */
        static bool IsInitialized() {
            return initialized;
        }

        /**
         * @brief Begin a new frame for ImGuiForms
         *
         * Call this at the beginning of each frame, before creating or updating components.
         * Handles frame-specific initialization and cleanup.
         */
        static void BeginFrame() {
            // TODO: Frame-specific initialization (ID cleanup, etc.)
        }

        /**
         * @brief End the current frame for ImGuiForms
         *
         * Call this at the end of each frame, after all components have been updated.
         */
        static void EndFrame() {
            // TODO: Frame-specific cleanup
        }

    private:
        static bool initialized;
    };

    // Static member definition (would go in a .cpp file in practice)
    inline bool Framework::initialized = false;

    /**
     * @brief Common alignment enumeration
     */
    enum class Alignment {
        Horizontal,
        Vertical
    };

    /**
     * @brief Horizontal alignment options
     */
    enum class HorizontalAlignment {
        Left,
        Center,
        Right
    };

    /**
     * @brief Vertical alignment options
     */
    enum class VerticalAlignment {
        Top,
        Center,
        Bottom
    };

    /**
     * @brief Utility macros for common operations
     */

     // Convenience macro for creating components
#define IMGUI_FORMS_COMPONENT(ClassName, ...) \
    std::make_unique<ClassName>(__VA_ARGS__)

// Macro for version checking
#define IMGUI_FORMS_VERSION_CHECK(major, minor, patch) \
    static_assert(ImGuiForms::Version::IsAtLeast(major, minor, patch), \
                  "ImGuiForms version requirement not met")

/**
 * @brief Common color constants (using ImGui's color system)
 */
    namespace Colors {
        constexpr ImVec4 White = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        constexpr ImVec4 Black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        constexpr ImVec4 Red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        constexpr ImVec4 Green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        constexpr ImVec4 Blue = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
        constexpr ImVec4 Yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        constexpr ImVec4 Cyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
        constexpr ImVec4 Magenta = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
        constexpr ImVec4 Gray = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        constexpr ImVec4 Transparent = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    /**
     * @brief Helper functions for common operations
     */
    namespace Utils {
        /**
         * @brief Convert ImGui color to packed uint32
         */
        inline ImU32 ColorToU32(const ImVec4& color) {
            return ImGui::GetColorU32(color);
        }

        /**
         * @brief Convert packed uint32 to ImGui color
         */
        inline ImVec4 U32ToColor(ImU32 color) {
            return ImGui::ColorConvertU32ToFloat4(color);
        }

        /**
         * @brief Clamp a value between min and max
         */
        template<typename T>
        constexpr T Clamp(T value, T min, T max) {
            return value < min ? min : (value > max ? max : value);
        }

        /**
         * @brief Linear interpolation between two values
         */
        template<typename T>
        constexpr T Lerp(T a, T b, float t) {
            return a + t * (b - a);
        }
    }

} // namespace ImGuiForms

/**
 * @brief Usage example in comments:
 *
 * ```cpp
 * #include <ImGuiForms/ImGuiForms.h>
 * using namespace ImGuiForms;
 *
 * int main() {
 *     // Initialize ImGui first...
 *
 *     // Initialize ImGuiForms
 *     Framework::Initialize();
 *
 *     // Create UI hierarchy
 *     auto mainPanel = CreatePanel(Size::Parent());
 *     mainPanel->showBorder = true;
 *
 *     auto titleLabel = CreateLabel("My Application", Colors::Yellow);
 *     titleLabel->SetSize(Size::WidthAlign());
 *
 *     mainPanel->SetContent(std::move(titleLabel));
 *
 *     // In your render loop:
 *     while (running) {
 *         Framework::BeginFrame();
 *
 *         if (ImGui::Begin("My Window")) {
 *             ImVec2 size = ImGui::GetContentRegionAvail();
 *             ImVec2 pos = ImGui::GetCursorScreenPos();
 *             Rectangle windowRect(pos, size);
 *
 *             mainPanel->Update(windowRect);
 *         }
 *         ImGui::End();
 *
 *         Framework::EndFrame();
 *
 *         // Render ImGui...
 *     }
 *
 *     Framework::Shutdown();
 *     return 0;
 * }
 * ```
 */