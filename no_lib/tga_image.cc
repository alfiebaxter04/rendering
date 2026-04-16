#include "tga_image.h"
#include <fstream>
#include <algorithm>

namespace TGA {
namespace {

#pragma pack(push, 1)
struct TGAHeader {
    std::uint8_t  id_length = 0;
    std::uint8_t  color_map_type = 0;
    std::uint8_t  image_type = 0;
    std::uint16_t color_map_origin = 0;
    std::uint16_t color_map_length = 0;
    std::uint8_t  color_map_depth = 0;
    std::uint16_t x_origin = 0;
    std::uint16_t y_origin = 0;
    std::uint16_t width = 0;
    std::uint16_t height = 0;
    std::uint8_t  pixel_depth = 0;
    std::uint8_t  image_descriptor = 0;
};
#pragma pack(pop)

} // namespace

TGAImage::TGAImage(int width, int height, PixelFormat format) 
    : width_(width), height_(height), format_(format),
      bpp_(static_cast<int>(format)),
      data_(static_cast<std::size_t>(width) * height * bpp_, 0) {}

void TGAImage::set(int x, int y, const TGAColor& color) {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) return;

    int index = (x + y * width_) * bpp_;
    
    if (format_ == PixelFormat::Gray) {
        data_[index] = static_cast<std::uint8_t>(color.r * 0.299 + color.g * 0.587 + color.b * 0.114);
    } else {
        data_[index]     = color.b;
        data_[index + 1] = color.g;
        data_[index + 2] = color.r;
        if (format_ == PixelFormat::RGBA) data_[index + 3] = color.a;
    }
}

[[nodiscard]] bool TGAImage::write_file(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) return false;

    TGAHeader header;
    header.width = static_cast<std::uint16_t>(width_);
    header.height = static_cast<std::uint16_t>(height_);
    header.pixel_depth = static_cast<std::uint8_t>(bpp_ * 8);
    header.image_type = (format_ == PixelFormat::Gray) ? 3 : 2;
    header.image_descriptor = (format_ == PixelFormat::RGBA) ? 8 : 0;

    out.write(reinterpret_cast<const char*>(&header), sizeof(TGAHeader));
    out.write(reinterpret_cast<const char*>(data_.data()), data_.size());
    
    return out.good();
}

} // namespace TGA