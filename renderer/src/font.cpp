#include "renderer/font.h"

namespace renderer {

Font::Font() {}

Font::~Font() {}

void Font::load_from_file(const std::string& filename) {
    // Placeholder implementation
}

void Font::set_size(float size) {
    this->size = size;
}

float Font::get_size() const {
    return size;
}

void Font::set_family(const std::string& family) {
    this->family = family;
}

std::string Font::get_family() const {
    return family;
}

} // namespace renderer
