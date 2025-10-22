#pragma once

#include <string>

namespace renderer {

class Font {
public:
    Font();
    ~Font();

    void load_from_file(const std::string& filename);
    void set_size(float size);
    float get_size() const;
    void set_family(const std::string& family);
    std::string get_family() const;

private:
    float size;
    std::string family;
};

} // namespace renderer
