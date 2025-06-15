/**
 * @file main.cpp
 * @brief Basic example demonstrating ImGuiForms C++ framework
 * 
 * This example shows:
 * - Framework initialization and cleanup
 * - Component creation and management
 * - Basic rendering loop
 * - Debug features and statistics
 */

#include <ImGuiForms/ImGuiForms.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

using namespace ImGuiForms;

/**
 * @brief Example application demonstrating ImGuiForms features
 */
class BasicExampleApp {
private:
    GLFWwindow* window;
    std::unique_ptr<Panel> rootPanel;
    std::unique_ptr<Label> titleLabel;
    std::unique_ptr<Label> infoLabel;
    std::unique_ptr<Panel> contentPanel;
    
    bool showDemo = true;
    bool showDebugInfo = false;
    bool enableDebugMode = false;

public:
    /**
     * @brief Initialize GLFW, OpenGL, and ImGui
     */
    bool Initialize() {
        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        // Create window
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(1280, 720, "ImGuiForms Basic Example", nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Setup ImGui style
        ImGui::StyleColorsDark();

        // Setup backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        std::cout << "OpenGL/ImGui initialization complete!" << std::endl;
        return true;
    }

    /**
     * @brief Initialize ImGuiForms components
     */
    void InitializeUI() {
        // Initialize ImGuiForms framework
        Framework::Initialize();
        Framework::SetDebugMode(enableDebugMode);

        // Create the main title
        titleLabel = CreateLabel("🎯 ImGuiForms C++ Demo", Colors::Yellow);
        titleLabel->SetSize(Size::WidthAlign());

        // Create info text
        infoLabel = CreateLabel(
            "Welcome to ImGuiForms - Object-oriented ImGui for C++!\n"
            "• Clean component hierarchy\n"
            "• Flexible sizing system\n" 
            "• RAII memory management\n"
            "• Native ImGui performance"
        );
        infoLabel->SetSize(Size::Content());

        // Create content panel
        contentPanel = CreatePanel(Size::Parent());
        contentPanel->showBorder = true;
        contentPanel->allowDragDrop = true;
        contentPanel->SetContent(std::move(infoLabel));

        // Set up drag & drop event
        contentPanel->onDragDrop = [this](Component* sender, const std::vector<DragDropEvent>& events) {
            std::cout << "\n=== Drag & Drop Event ===" << std::endl;
            std::cout << "Component ID: " << sender->GetId() << std::endl;
            for (const auto& event : events) {
                std::cout << "File dropped: " << event.filePath << std::endl;
            }
            std::cout << "========================\n" << std::endl;
        };

        // Create root panel
        rootPanel = CreatePanel(Size::Parent());
        rootPanel->SetContent(std::move(titleLabel));

        std::cout << "ImGuiForms UI initialized!" << std::endl;
    }

    /**
     * @brief Main application loop
     */
    void Run() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            // Start ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Start ImGuiForms frame
            Framework::BeginFrame();

            // Render our UI
            RenderMainWindow();
            
            if (showDemo) {
                RenderDemoWindow();
            }
            
            if (showDebugInfo) {
                RenderDebugWindow();
            }

            // End ImGuiForms frame
            Framework::EndFrame();

            // Render ImGui
            ImGui::Render();
            
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
        }
    }

    /**
     * @brief Render the main application window
     */
    void RenderMainWindow() {
        if (ImGui::Begin("ImGuiForms Demo", nullptr, ImGuiWindowFlags_MenuBar)) {
            
            // Menu bar
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("View")) {
                    ImGui::MenuItem("Show Demo", nullptr, &showDemo);
                    ImGui::MenuItem("Show Debug Info", nullptr, &showDebugInfo);
                    ImGui::Separator();
                    if (ImGui::MenuItem("Toggle Debug Mode", nullptr, &enableDebugMode)) {
                        Framework::SetDebugMode(enableDebugMode);
                    }
                    ImGui::EndMenuBar();
                }
                ImGui::EndMenuBar();
            }

            // Get window content area
            ImVec2 contentSize = ImGui::GetContentRegionAvail();
            ImVec2 contentPos = ImGui::GetCursorScreenPos();
            
            Rectangle windowRect(
                static_cast<int>(contentPos.x),
                static_cast<int>(contentPos.y),
                static_cast<int>(contentSize.x),
                static_cast<int>(contentSize.y)
            );

            // Split the window: title at top, content below
            Rectangle titleRect(windowRect.x, windowRect.y, windowRect.width, 60);
            Rectangle mainContentRect(windowRect.x, windowRect.y + 70, windowRect.width, windowRect.height - 70);

            // Render ImGuiForms components
            if (rootPanel) {
                rootPanel->Update(titleRect);
            }
            
            if (contentPanel) {
                contentPanel->Update(mainContentRect);
            }
        }
        ImGui::End();
    }

    /**
     * @brief Render the demo features window
     */
    void RenderDemoWindow() {
        if (ImGui::Begin("Demo Features", &showDemo)) {
            ImGui::Text("ImGuiForms Sizing System Demo:");
            ImGui::Separator();
            
            ImGui::BulletText("Size::Content() - Auto-size to component content");
            ImGui::BulletText("Size::Parent() - Fill entire parent container");
            ImGui::BulletText("Size::WidthAlign() - Fill width, auto height");
            ImGui::BulletText("Size::HeightAlign() - Auto width, fill height");
            ImGui::BulletText("Size(400, 200) - Fixed 400x200 pixels");
            ImGui::BulletText("Size(0.5f, 0.3f) - 50% width, 30% height of parent");
            
            ImGui::Separator();
            ImGui::Text("Framework Features:");
            ImGui::BulletText("RAII memory management with std::unique_ptr");
            ImGui::BulletText("Event system with std::function callbacks");
            ImGui::BulletText("Automatic component ID management");
            ImGui::BulletText("Debug mode with validation and logging");
            ImGui::BulletText("Runtime statistics and profiling");
            
            ImGui::Separator();
            ImGui::Text("Try dragging files onto the content panel above!");
        }
        ImGui::End();
    }

    /**
     * @brief Render debug information window
     */
    void RenderDebugWindow() {
        if (ImGui::Begin("Debug Information", &showDebugInfo)) {
            
            // Framework statistics
            auto stats = Framework::GetStatistics();
            ImGui::Text("Framework Statistics:");
            ImGui::Text("  Active Components: %zu", stats.activeComponents);
            ImGui::Text("  Total Created: %zu", stats.totalComponentsCreated);
            ImGui::Text("  This Frame: %zu", stats.frameComponentCount);
            ImGui::Text("  Memory Usage: %zu bytes", stats.memoryUsageBytes);
            
            ImGui::Separator();
            
            // Component hierarchy
            ImGui::Text("Component Hierarchy:");
            if (ImGui::Button("Dump to Console")) {
                std::cout << "\n=== Component Hierarchy ===" << std::endl;
                Utils::DumpComponentHierarchy(rootPanel.get());
                Utils::DumpComponentHierarchy(contentPanel.get());
                std::cout << "===========================\n" << std::endl;
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Validate Tree")) {
                bool valid = Utils::ValidateComponentTree(rootPanel.get()) &&
                            Utils::ValidateComponentTree(contentPanel.get());
                std::cout << "Component tree validation: " 
                         << (valid ? "PASSED" : "FAILED") << std::endl;
            }
            
            ImGui::Separator();
            
            // Manual statistics
            if (ImGui::Button("Print Statistics")) {
                Framework::PrintStatistics();
            }
            
            // Debug mode toggle
            if (ImGui::Checkbox("Debug Mode", &enableDebugMode)) {
                Framework::SetDebugMode(enableDebugMode);
            }
            
            ImGui::Separator();
            ImGui::Text("Debug mode provides:");
            ImGui::BulletText("Component creation/destruction logging");
            ImGui::BulletText("Enhanced error messages");
            ImGui::BulletText("Validation warnings");
            ImGui::BulletText("Performance profiling");
        }
        ImGui::End();
    }

    /**
     * @brief Cleanup and shutdown
     */
    void Shutdown() {
        std::cout << "\nShutting down application..." << std::endl;
        
        // Cleanup ImGuiForms components (RAII will handle this automatically)
        contentPanel.reset();
        rootPanel.reset();
        
        // Shutdown ImGuiForms
        Framework::Shutdown();
        
        // Cleanup ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        // Cleanup GLFW
        glfwDestroyWindow(window);
        glfwTerminate();
        
        std::cout << "Application shutdown complete!" << std::endl;
    }
};

/**
 * @brief Application entry point
 */
int main() {
    std::cout << "=== ImGuiForms C++ Basic Example ===" << std::endl;
    std::cout << "Framework Version: " << Version::GetVersionString() << std::endl;
    std::cout << "Initializing..." << std::endl;
    
    BasicExampleApp app;
    
    if (!app.Initialize()) {
        std::cerr << "Failed to initialize application!" << std::endl;
        return -1;
    }
    
    app.InitializeUI();
    
    std::cout << "Starting main loop..." << std::endl;
    app.Run();
    
    app.Shutdown();
    
    std::cout << "Example completed successfully!" << std::endl;
    return 0;
}