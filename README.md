# ImGuiForms C++

**A modern, object-oriented C++ framework around Dear ImGui**

ImGuiForms brings the simplicity of WinForms-style declarative UI development to C++ while maintaining ImGui's excellent performance and immediate-mode benefits. Unlike the original C# version, this C++ implementation fills a real gap in the ecosystem - providing a lightweight, pleasant-to-use GUI framework for C++ developers.

## ⚡ Why ImGuiForms C++?

- **🎯 Fills a real need**: C++ lacks lightweight, object-oriented GUI frameworks
- **🚀 Native performance**: Direct ImGui calls, zero binding overhead
- **💡 Clean API**: WinForms-style declarative components with modern C++
- **🔧 Flexible sizing**: Absolute pixels, relative percentages, content-based, and parent-aligned
- **🎨 ImGui integration**: Direct use of `ImVec2`, `ImVec4`, `ImU32` types
- **📦 RAII ownership**: Smart pointers and automatic memory management

## 🚀 Quick Start

### Prerequisites

- C++17 or later
- CMake 3.16+
- Dear ImGui
- OpenGL/DirectX (for ImGui backend)

### Building

```bash
git clone https://github.com/your-repo/ImGuiForms.git
cd ImGuiForms
git submodule update --init --recursive  # For Dear ImGui

mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Basic Usage

```cpp
#include <ImGuiForms/ImGuiForms.h>
using namespace ImGuiForms;

int main() {
    // Initialize ImGui first...
    
    Framework::Initialize();
    
    // Create UI hierarchy
    auto mainPanel = CreatePanel(Size::Parent());
    auto titleLabel = CreateLabel("Hello, ImGuiForms!", Colors::Yellow);
    mainPanel->SetContent(std::move(titleLabel));
    
    // In your render loop:
    while (running) {
        Framework::BeginFrame();
        
        if (ImGui::Begin("My App")) {
            ImVec2 size = ImGui::GetContentRegionAvail();
            ImVec2 pos = ImGui::GetCursorScreenPos();
            Rectangle windowRect(pos, size);
            
            mainPanel->Update(windowRect);
        }
        ImGui::End();
        
        Framework::EndFrame();
        // Render ImGui...
    }
    
    Framework::Shutdown();
    return 0;
}
```

## 🏗️ Architecture

### Core Components

- **Component**: Base class for all UI elements
- **Panel**: Container for other components
- **Label**: Text display with automatic sizing
- **Size System**: Flexible sizing with absolute, relative, content, and parent modes

### Sizing System

```cpp
// Absolute pixel sizes
Size(400, 300)              // 400x300 pixels
SizeValue::Absolute(200)    // 200 pixels

// Relative to parent (0.0 - 1.0)
Size(0.5f, 0.8f)           // 50% width, 80% height
SizeValue::Relative(0.3f)   // 30% of parent

// Special alignment modes
Size::Content()             // Auto-size to component content
Size::Parent()              // Fill entire parent container
Size::WidthAlign()          // Fill width, auto height
Size::HeightAlign()         // Auto width, fill height
```

### Component Hierarchy

```cpp
auto rootPanel = CreatePanel(Size::Parent());

// Header with fixed height
auto header = CreatePanel(Size(SizeValue::Parent(), SizeValue::Absolute(50)));
header->showBorder = true;

auto headerText = CreateLabel("Application Title");
header->SetContent(std::move(headerText));

// Content area fills remaining space
auto content = CreatePanel(Size::Parent());
auto contentText = CreateLabel("Main content area");
content->SetContent(std::move(contentText));

// In a real app, you'd use StackLayout or similar to arrange header + content
rootPanel->SetContent(std::move(header));
```

## 📁 Project Structure

```
ImGuiForms/
├── include/ImGuiForms/     # Public headers
│   ├── Core/               # Core types (Size, Rectangle, Component)
│   ├── Components/         # UI components (Panel, Label, Button)
│   ├── Layouts/           # Layout containers (StackLayout, TableLayout)
│   └── ImGuiForms.h       # Main include header
├── src/                   # Implementation files
├── examples/              # Example applications
├── tests/                 # Unit tests
└── docs/                 # Documentation
```

## 🎨 Examples

### Layout Demo

```cpp
// Create a responsive layout
auto mainPanel = CreatePanel(Size::Parent());

// Header: fixed height, full width
auto header = CreatePanel(Size(SizeValue::Parent(), SizeValue::Absolute(60)));
header->showBorder = true;
auto headerLabel = CreateLabel("My Application", Colors::White);
header->SetContent(std::move(headerLabel));

// Content: remaining space
auto content = CreatePanel(Size::Parent());
auto contentLabel = CreateLabel("Content goes here...");
content->SetContent(std::move(contentLabel));

// Future: Use StackLayout to arrange header + content vertically
// auto stack = CreateStackLayout(Alignment::Vertical);
// stack->AddItem(std::move(header));
// stack->AddItem(std::move(content));
```

### Event Handling

```cpp
auto panel = CreatePanel(Size::Parent());
panel->allowDragDrop = true;

// Set up drag & drop event
panel->onDragDrop = [](Component* sender, const std::vector<DragDropEvent>& events) {
    std::cout << "Files dropped:" << std::endl;
    for (const auto& event : events) {
        std::cout << "  " << event.filePath << std::endl;
    }
};
```

## 🔧 Building Your Own Components

```cpp
class Button : public Component {
private:
    std::string text;
    Size buttonSize = Size::Content();
    
public:
    std::function<void()> onClick;
    
    explicit Button(const std::string& buttonText) : text(buttonText) {}
    
    Size GetSize() const override { return buttonSize; }
    
protected:
    void UpdateInternal(const Rectangle& contentRect) override {
        if (ImGui::Button(text.c_str(), contentRect.Size())) {
            if (onClick) onClick();
        }
    }
    
    int GetContentWidth(int parentWidth, int parentHeight, float layoutCorrection) const override {
        return static_cast<int>(ImGui::CalcTextSize(text.c_str()).x + 20); // padding
    }
    
    int GetContentHeight(int parentWidth, int parentHeight, float layoutCorrection) const override {
        return static_cast<int>(ImGui::GetTextLineHeight() + 10); // padding
    }
};
```

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guide](docs/CONTRIBUTING.md) for details.

### Development Setup

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Submit a pull request

### Running Tests

```bash
cd build
ctest --output-on-failure
```

## 📚 Documentation

- [Getting Started Guide](docs/tutorials/01-getting-started.md)
- [API Reference](docs/API.md)
- [Example Gallery](docs/Examples.md)
- [Migration from C# ImGui.Forms](docs/Migration.md)

## 🔄 Roadmap

### Current Status ✅
- [x] Core architecture (Component, Size, Rectangle)
- [x] Basic components (Panel, Label)
- [x] Flexible sizing system
- [x] RAII memory management

### Coming Soon 🚧
- [ ] Layout containers (StackLayout, TableLayout, ZLayout)
- [ ] More components (Button, TextBox, Image)
- [ ] Text editor with syntax highlighting
- [ ] Event system improvements
- [ ] Theme and styling system
- [ ] Complete examples and documentation

### Future 🔮
- [ ] Advanced layouts (DockLayout, GridLayout)
- [ ] Rich text editing
- [ ] Data binding system
- [ ] Animation framework
- [ ] Designer tool

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Dear ImGui** by Omar Cornut - The excellent immediate-mode GUI foundation
- **Original ImGui.Forms** by onepiecefreak - Inspiration for the object-oriented approach
- **ImGui.NET** - Reference for C# ImGui bindings

## 🆚 Comparison with C# ImGui.Forms

| Feature | C# ImGui.Forms | C++ ImGuiForms |
|---------|----------------|----------------|
| **Performance** | .NET overhead | Native speed |
| **Memory** | GC pressure | RAII + smart pointers |
| **Type Safety** | Runtime checking | Compile-time safety |
| **ImGui Types** | Wrapper types | Direct ImVec2/ImVec4 |
| **Ecosystem Fit** | C# has WinForms/WPF | Fills C++ GUI gap |
| **API Style** | C# properties/events | Modern C++ |

---

**ImGuiForms C++** - Object-oriented ImGui development that actually makes sense! 🎯