/**
 * @file RectangleDemo.cpp
 * @brief Demonstration of advanced Rectangle functionality
 * 
 * This example shows all the new Rectangle features:
 * - Geometric operations (union, intersection, area, etc.)
 * - Layout operations (subdivision, grid layout, alignment)
 * - Transformation operations (scaling, rotation)
 * - Utility functions (distance, collision detection)
 */

#include <ImGuiForms/ImGuiForms.h>
#include <iostream>
#include <cassert>

using namespace ImGuiForms;

void DemonstrateBasicOperations() {
    std::cout << "\n=== Basic Rectangle Operations ===" << std::endl;
    
    Rectangle rect1(10, 20, 100, 50);
    Rectangle rect2(60, 30, 80, 40);
    
    rect1.DebugPrint("Rectangle 1");
    rect2.DebugPrint("Rectangle 2");
    
    // Union and intersection
    Rectangle unionRect = rect1.Union(rect2);
    Rectangle intersectionRect = rect1.Intersection(rect2);
    
    unionRect.DebugPrint("Union");
    intersectionRect.DebugPrint("Intersection");
    
    // Properties
    std::cout << "Rect1 Area: " << rect1.Area() << std::endl;
    std::cout << "Rect1 Perimeter: " << rect1.Perimeter() << std::endl;
    std::cout << "Rect1 Aspect Ratio: " << rect1.AspectRatio() << std::endl;
    
    // Distance calculations
    std::cout << "Distance between rects: " << rect1.DistanceTo(rect2) << std::endl;
    std::cout << "Distance to point (0,0): " << rect1.DistanceToPoint(ImVec2(0, 0)) << std::endl;
}

void DemonstrateLayoutOperations() {
    std::cout << "\n=== Layout Operations ===" << std::endl;
    
    Rectangle container(0, 0, 400, 300);
    container.DebugPrint("Container");
    
    // Horizontal subdivision
    std::vector<float> ratios = {1.0f, 2.0f, 1.0f}; // 25%, 50%, 25%
    auto horizontalPanes = container.SubdivideHorizontal(ratios);
    
    std::cout << "Horizontal subdivision (1:2:1 ratio):" << std::endl;
    for (size_t i = 0; i < horizontalPanes.size(); ++i) {
        horizontalPanes[i].DebugPrint("  Pane " + std::to_string(i));
    }
    
    // Vertical subdivision
    auto verticalPanes = container.SubdivideVertical({1.0f, 1.0f}); // 50%, 50%
    
    std::cout << "Vertical subdivision (1:1 ratio):" << std::endl;
    for (size_t i = 0; i < verticalPanes.size(); ++i) {
        verticalPanes[i].DebugPrint("  Pane " + std::to_string(i));
    }
    
    // Grid layout
    std::cout << "3x3 Grid cells:" << std::endl;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            Rectangle cell = container.CreateGrid(row, col, 3, 3, 5);
            cell.DebugPrint("  Cell [" + std::to_string(row) + "," + std::to_string(col) + "]");
        }
    }
}

void DemonstrateAlignment() {
    std::cout << "\n=== Alignment Operations ===" << std::endl;
    
    Rectangle container(0, 0, 400, 300);
    Rectangle element(0, 0, 100, 50);
    
    container.DebugPrint("Container");
    element.DebugPrint("Original Element");
    
    // Different alignments
    element.AlignLeft(container, 10).DebugPrint("Left-aligned (margin 10)");
    element.AlignRight(container, 10).DebugPrint("Right-aligned (margin 10)");
    element.AlignTop(container, 10).DebugPrint("Top-aligned (margin 10)");
    element.AlignBottom(container, 10).DebugPrint("Bottom-aligned (margin 10)");
    element.AlignCenterHorizontal(container).DebugPrint("Horizontally centered");
    element.AlignCenterVertical(container).DebugPrint("Vertically centered");
    element.CenterIn(container).DebugPrint("Fully centered");
}

void DemonstrateTransformations() {
    std::cout << "\n=== Transformation Operations ===" << std::endl;
    
    Rectangle original(50, 50, 100, 60);
    original.DebugPrint("Original");
    
    // Scaling
    original.Scale(1.5f).DebugPrint("Scaled 1.5x (around center)");
    original.Scale(2.0f, 0.5f, ImVec2(0, 0)).DebugPrint("Scaled 2x width, 0.5x height (around origin)");
    
    // Rotation
    original.Rotate90().DebugPrint("Rotated 90 degrees");
    
    // Fitting operations
    Rectangle largeContainer(0, 0, 400, 300);
    original.FitInside(largeContainer, true).DebugPrint("Fit inside large container (maintain aspect)");
    original.FitInside(largeContainer, false).DebugPrint("Fit inside large container (stretch)");
    
    Rectangle smallContainer(0, 0, 80, 40);
    original.ClampTo(smallContainer).DebugPrint("Clamped to small container");
}

void DemonstrateUtilities() {
    std::cout << "\n=== Utility Functions ===" << std::endl;
    
    // Factory methods
    Rectangle fromPoints = Rectangle::FromTwoPoints(ImVec2(10, 20), ImVec2(110, 80));
    fromPoints.DebugPrint("From two points (10,20) to (110,80)");
    
    Rectangle fromCenter = Rectangle::FromCenter(ImVec2(200, 150), 100, 60);
    fromCenter.DebugPrint("From center (200,150) with size 100x60");
    
    // Collision detection
    Rectangle rect1(50, 50, 100, 100);
    Rectangle rect2(75, 75, 50, 50);
    Rectangle rect3(200, 200, 50, 50);
    
    std::cout << "Collision tests:" << std::endl;
    std::cout << "  rect1 intersects rect2: " << rect1.Intersects(rect2) << std::endl;
    std::cout << "  rect1 contains rect2: " << rect1.ContainsRectangle(rect2) << std::endl;
    std::cout << "  rect1 intersects rect3: " << rect1.Intersects(rect3) << std::endl;
    
    // String representation
    std::cout << "String representation: " << rect1.ToString() << std::endl;
}

void DemonstrateLayoutUtils() {
    std::cout << "\n=== Layout Utility Functions ===" << std::endl;
    
    Rectangle container(10, 10, 400, 300);
    
    // Grid arrangement
    auto gridRects = RectangleUtils::ArrangeInGrid(container, 8, 3, ImVec2(5, 5), ImVec2(10, 10));
    std::cout << "Grid arrangement (8 items in 3 columns):" << std::endl;
    for (size_t i = 0; i < gridRects.size(); ++i) {
        gridRects[i].DebugPrint("  Item " + std::to_string(i));
    }
    
    // Line arrangement
    auto lineRects = RectangleUtils::ArrangeInLine(container, 4, true, ImVec2(10, 0), ImVec2(15, 15));
    std::cout << "Horizontal line arrangement (4 items):" << std::endl;
    for (size_t i = 0; i < lineRects.size(); ++i) {
        lineRects[i].DebugPrint("  Item " + std::to_string(i));
    }
    
    // Bounding box
    std::vector<Rectangle> scattered = {
        Rectangle(10, 20, 50, 30),
        Rectangle(100, 50, 40, 60),
        Rectangle(200, 10, 30, 40)
    };
    Rectangle boundingBox = RectangleUtils::CalculateBoundingBox(scattered);
    boundingBox.DebugPrint("Bounding box of scattered rectangles");
}

void RunValidationTests() {
    std::cout << "\n=== Validation Tests ===" << std::endl;
    
    // Test empty rectangle handling
    Rectangle empty;
    Rectangle normal(10, 10, 100, 50);
    
    assert(empty.IsEmpty());
    assert(!normal.IsEmpty());
    assert(empty.Union(normal) == normal);
    assert(normal.Union(empty) == normal);
    assert(empty.Intersection(normal).IsEmpty());
    
    // Test aspect ratio edge cases
    Rectangle square(0, 0, 100, 100);
    Rectangle wide(0, 0, 200, 50);
    Rectangle tall(0, 0, 50, 200);
    Rectangle zeroHeight(0, 0, 100, 0);
    
    assert(square.AspectRatio() == 1.0f);
    assert(wide.AspectRatio() == 4.0f);
    assert(tall.AspectRatio() == 0.25f);
    assert(zeroHeight.AspectRatio() == 0.0f);
    
    // Test point containment
    Rectangle rect(50, 50, 100, 100);
    assert(rect.Contains(75, 75));   // Inside
    assert(!rect.Contains(25, 25));  // Outside
    assert(rect.Contains(50, 50));   // Top-left corner
    assert(!rect.Contains(150, 150)); // Bottom-right corner (exclusive)
    
    std::cout << "All validation tests passed!" << std::endl;
}

int main() {
    std::cout << "=== ImGuiForms Rectangle Advanced Features Demo ===" << std::endl;
    std::cout << "Demonstrating comprehensive Rectangle operations" << std::endl;
    
    // Run all demonstrations
    DemonstrateBasicOperations();
    DemonstrateLayoutOperations();
    DemonstrateAlignment();
    DemonstrateTransformations();
    DemonstrateUtilities();
    DemonstrateLayoutUtils();
    
    // Run validation tests
    RunValidationTests();
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "Rectangle class provides comprehensive 2D geometry operations" << std::endl;
    std::cout << "Perfect for GUI layout calculations and component positioning!" << std::endl;
    
    return 0;
}