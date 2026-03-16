#ifndef TGA_IMAGE_H
#define TGA_IMAGE_H

#include <fstream>
#include <cstdint>

namespace TGA {

    struct TGAColor {
        int b = 0, g = 0, r = 0, a = 255;
        constexpr TGAColor() : b(0), g(0), r(0), a(255) {};
        constexpr TGAColor(int b, int g, int r, int a)
            : b(b), g(g), r(r), a(a) {};
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
        enum TGAPixelFormat {
            GRAYSCALE = 1,
            RGB = 3,
            RGBA = 4
        };

        TGAImage(int width, int height, TGAPixelFormat format)
            : width(width), height(height), format(format)
        {
            bytes_per_pixel = static_cast<int>(format);
            total_size = width * height * bytes_per_pixel;
            data = new uint8_t[total_size]();

            header.width = width;
            header.height = height;
            header.pixel_depth = bytes_per_pixel * 8;
            header.image_type = (format == TGAPixelFormat::GRAYSCALE) ? 3 : 2;
            header.image_descriptor = (format == TGAPixelFormat::RGBA) ? 8 : 0;
        }

        ~TGAImage() {
            delete[] data;
        }

        void set(int x, int y, TGAColor color) {
            if (x < 0 || y < 0 || x >= width || y >= height) return;

            int index = (x + y * width) * bytes_per_pixel;

            switch (format) {
                case TGAPixelFormat::GRAYSCALE:
                    data[index] = static_cast<uint8_t>((color.r + color.g + color.b) / 3);
                    break;
                case TGAPixelFormat::RGB:
                    data[index]   = color.b;
                    data[index+1] = color.g;
                    data[index+2] = color.r;
                    break;
                case TGAPixelFormat::RGBA:
                    data[index]   = color.b;
                    data[index+1] = color.g;
                    data[index+2] = color.r;
                    data[index+3] = color.a;
                    break;
            }
        }

        void write_tga_file(const std::string& filename) {
            std::ofstream out(filename, std::ios::binary);
            if (!out) return;

            out.write(reinterpret_cast<char*>(&header), sizeof(TGAHeader));
            out.write(reinterpret_cast<char*>(data), total_size);

            out.close();
        }

    private:
        int width, height;
        TGAHeader header;
        uint8_t* data;
        TGAPixelFormat format;
        int bytes_per_pixel;
        int total_size;
    };
}

#endif