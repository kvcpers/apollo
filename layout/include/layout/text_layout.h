#pragma once

#include "layout_node.h"
#include "types.h"
#include <memory>
#include <vector>

namespace layout {

// Text layout algorithm implementation
class TextLayout {
public:
    TextLayout();
    ~TextLayout();

    // Layout text content
    void layoutText(LayoutNode* node, const LayoutConstraints& constraints);

    // Layout text runs
    void layoutTextRuns(LayoutNode* node);

    // Calculate text size
    Size calculateTextSize(LayoutNode* node, const LayoutConstraints& constraints);

    // Calculate text position
    Point calculateTextPosition(LayoutNode* node);

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
    Size measureText(const std::string& text, const FontMetrics& metrics);

    // Measure text width
    double measureTextWidth(const std::string& text, const FontMetrics& metrics);

    // Measure text height
    double measureTextHeight(const std::string& text, const FontMetrics& metrics);

    // Create text runs
    std::vector<TextRun> createTextRuns(const std::string& text, const FontMetrics& metrics);

    // Break text into lines
    std::vector<std::string> breakTextIntoLines(const std::string& text, double maxWidth, const FontMetrics& metrics);

    // Break text into words
    std::vector<std::string> breakTextIntoWords(const std::string& text);

    // Calculate text metrics
    FontMetrics calculateTextMetrics(const std::string& text, const FontMetrics& baseMetrics);

private:
    void layoutTextChild(LayoutNode* child, const LayoutConstraints& constraints);
    void positionTextChild(LayoutNode* child);
    void layoutTextContent(LayoutNode* node);
    void layoutTextRuns(LayoutNode* node);
    void breakTextLines(LayoutNode* node);
    void wrapTextWords(LayoutNode* node);
    void alignTextContent(LayoutNode* node);
    void alignTextVertically(LayoutNode* node);
    void alignTextBaseline(LayoutNode* node);
    void calculateTextLineHeight(LayoutNode* node);
    void calculateTextBaseline(LayoutNode* node);
    void measureTextContent(LayoutNode* node, const std::string& text);
    void measureTextWidth(LayoutNode* node, const std::string& text);
    void measureTextHeight(LayoutNode* node, const std::string& text);
    std::vector<TextRun> createTextRunsForNode(LayoutNode* node);
    std::vector<std::string> breakTextIntoLinesForNode(LayoutNode* node);
    std::vector<std::string> breakTextIntoWordsForNode(LayoutNode* node);
    FontMetrics calculateTextMetricsForNode(LayoutNode* node);
};

} // namespace layout
