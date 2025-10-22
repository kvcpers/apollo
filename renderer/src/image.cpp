#include "renderer/image.h"

namespace renderer {

Image::Image() : width(0), height(0) {}

Image::~Image() {}

void Image::load_from_file(const std::string& filename) {
    // Placeholder implementation
}

void Image::load_from_data(const std::vector<uint8_t>& data) {
    // Placeholder implementation
}

int Image::get_width() const {
    return width;
}

int Image::get_height() const {
    return height;
}

void Image::set_width(int w) {
    width = w;
}

void Image::set_height(int h) {
    height = h;
}

} // namespace renderer
