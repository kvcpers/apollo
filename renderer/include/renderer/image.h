#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace renderer {

class Image {
public:
    Image();
    ~Image();

    void load_from_file(const std::string& filename);
    void load_from_data(const std::vector<uint8_t>& data);

    int get_width() const;
    int get_height() const;
    void set_width(int width);
    void set_height(int height);

private:
    int width;
    int height;
};

} // namespace renderer
