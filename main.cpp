#include <cmath>

#include "tgaimage.h"

using namespace TGA;

// BGRA
constexpr TGAColor white  = {255, 255, 255, 255};
constexpr TGAColor green  = {  0, 255,   0, 255};
constexpr TGAColor red    = {  0,   0, 255, 255};
constexpr TGAColor blue   = {255, 128,  64, 255};
constexpr TGAColor yellow = {  0, 200, 255, 255};


int main(int argc, char** argv) {
    constexpr int width  = 1028;
    constexpr int height = 1028;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    int ax =  7, ay =  3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;

    std::string filename = "../obj/diablo3.obj";
    framebuffer.load_obj(framebuffer, filename, red);

    framebuffer.write_tga_file("../framebuffer.tga");

    return 0;
}