#include <iostream>
#include "tga_image.h"
#include "obj_handler.h"
#include "renderer.h"

constexpr TGA::TGAColor white  {255, 255, 255};
constexpr TGA::TGAColor green  {0, 255, 0};
constexpr TGA::TGAColor red    {255, 0, 0};
constexpr TGA::TGAColor blue   {64, 128, 255};
constexpr TGA::TGAColor yellow {255, 200, 0};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.obj>" << std::endl;
        return 1;
    }

    constexpr int width  = 5000;
    constexpr int height = 5000;

    Obj::ObjHandler handler;
    if (!handler.load(argv[1])) {
        std::cerr << "Failed to load model: " << argv[1] << std::endl;
        return 1;
    }

    TGA::TGAImage framebuffer(width, height, TGA::TGAImage::PixelFormat::RGB);

    TGA::Renderer::draw_obj(framebuffer, handler, red);

    if (framebuffer.write_file("framebuffer.tga")) {
        std::cout << "Framebuffer saved to framebuffer.tga" << std::endl;
    } else {
        std::cerr << "Failed to write framebuffer." << std::endl;
        return 1;
    }

    return 0;
}