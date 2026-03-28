#ifndef TGA_IMAGE_H
#define TGA_IMAGE_H

#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>

namespace TGA {

    struct TGAColor {
        int b = 0, g = 0, r = 0, a = 255;
        constexpr TGAColor() = default;
        constexpr TGAColor(int r, int g, int b, int a = 255)
            : b(b), g(g), r(r), a(a) {};

        TGAColor randomColor() {
            std::srand(std::time(){})
        }
    };

    #pragma pack(push, 1)
    struct TGAHeader {
        char  id_length            = 0;
        char  color_map_type       = 0;
        char  image_type;
        short first_entry_index    = 0;
        short color_map_length     = 0;
        char  color_map_entry_size = 0;
        short x_origin             = 0;
        short y_origin             = 0;
        short width;
        short height;
        char  pixel_depth;
        char  image_descriptor;
    };
    #pragma pack(pop)

    class TGAImage {
    public:
        enum class TGAPixelFormat {
            GRAYSCALE = 1,
            RGB = 3,
            RGBA = 4
        };

        TGAImage(int width, int height, TGAPixelFormat format)
            : width_(width), height_(height), format_(format),
              bytes_per_pixel_(static_cast<int>(format)),
              image_data_(static_cast<std::size_t>(width) * height * bytes_per_pixel_, 0)
        {
            header_.width = width;
            header_.height = height;
            header_.pixel_depth = bytes_per_pixel_ * 8;
            header_.image_type = (format_ == TGAPixelFormat::GRAYSCALE) ? 3 : 2;
            header_.image_descriptor = (format_ == TGAPixelFormat::RGBA) ? 8 : 0;
        }

        void set(int x, int y, const TGAColor& color) {
            if (x < 0 || y < 0 || x >= width_ || y >= height_) return;

            int index = (x + y * width_) * bytes_per_pixel_;
            std::copy_n(&color.b, bytes_per_pixel_, image_data_.begin() + index);
        }

        bool write_tga_file(const std::string& filename) {
            std::ofstream out(filename, std::ios::binary);
            if (!out) return false;

            out.write(reinterpret_cast<const char*>(&header_), sizeof(TGAHeader));
            out.write(reinterpret_cast<const char*>(image_data_.data()), image_data_.size());

            return true;
        }

        int get_width() const { return width_; }
        int get_height() const { return height_; }

    private:
        int width_;
        int height_;
        TGAHeader header_{};
        TGAPixelFormat format_;
        int bytes_per_pixel_;
        std::vector<uint8_t> image_data_;
    };
}

#endif