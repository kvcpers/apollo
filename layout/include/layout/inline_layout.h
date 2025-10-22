#pragma once

#include "layout_node.h"
#include "types.h"
#include <memory>
#include <vector>

namespace layout {

// Inline layout algorithm implementation
class InlineLayout {
public:
    InlineLayout();
    ~InlineLayout();

    // Layout inline content
    void layoutInline(LayoutNode* node, const LayoutConstraints& constraints);

    // Layout inline children
    void layoutInlineChildren(LayoutNode* node, const LayoutConstraints& constraints);

    // Calculate inline size
    Size calculateInlineSize(LayoutNode* node, const LayoutConstraints& constraints);

    // Calculate inline position
    Point calculateInlinePosition(LayoutNode* node);

    // Handle text layout
    void layoutText(LayoutNode* node);

    // Handle line breaking
    void breakLines(LayoutNode* node);

    // Handle word wrapping
    void wrapWords(LayoutNode* node);

    // Handle text alignment
    void alignText(LayoutNode* node);

    // Handle vertical alignment
    void alignVertically(LayoutNode* node);

    // Handle baseline alignment
    void alignBaseline(LayoutNode* node);

    // Calculate line height
    double calculateLineHeight(LayoutNode* node);

    // Calculate baseline
    double calculateBaseline(LayoutNode* node);

    // Measure text
    Size measureText(LayoutNode* node, const std::string& text);

    // Measure text width
    double measureTextWidth(LayoutNode* node, const std::string& text);

    // Measure text height
    double measureTextHeight(LayoutNode* node, const std::string& text);

private:
    void layoutInlineChild(LayoutNode* child, const LayoutConstraints& constraints);
    void positionInlineChild(LayoutNode* child);
    void layoutInlineText(LayoutNode* node);
    void layoutInlineBlock(LayoutNode* node);
    void layoutInlineTable(LayoutNode* node);
    void layoutInlineFlex(LayoutNode* node);
    void layoutInlineGrid(LayoutNode* node);
    void breakInlineLines(LayoutNode* node);
    void breakInlineWords(LayoutNode* node);
    void alignInlineText(LayoutNode* node);
    void alignInlineVertically(LayoutNode* node);
    void alignInlineBaseline(LayoutNode* node);
    void calculateInlineLineHeight(LayoutNode* node);
    void calculateInlineBaseline(LayoutNode* node);
    void measureInlineText(LayoutNode* node, const std::string& text);
    void measureInlineTextWidth(LayoutNode* node, const std::string& text);
    void measureInlineTextHeight(LayoutNode* node, const std::string& text);
};

} // namespace layout
