#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace TGA {

struct TGAColor {
    std::uint8_t r = 0, g = 0, b = 0, a = 255;

    constexpr TGAColor() = default;
    constexpr TGAColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255)
        : b(b), g(g), r(r), a(a) {};
};

class TGAImage {
public:
    enum class PixelFormat : std::uint8_t {
        Gray = 1,
        RGB  = 3,
        RGBA = 4
    };

    TGAImage(int width, int height, PixelFormat format);
    ~TGAImage() = default;

    TGAImage(const TGAImage&) = delete;
    TGAImage& operator=(const TGAImage&) = delete;

    void set(int x, int y, const TGAColor& color);
    
    [[nodiscard]] bool write_file(const std::string& filename) const;

    int width() const  { return width_; }
    int height() const { return height_; }

private:
    int width_;
    int height_;
    PixelFormat format_;
    int bpp_;
    std::vector<std::uint8_t> data_;
};

} // namespace TGA