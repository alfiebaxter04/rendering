#include <cmath>

#include "tgaimage.h"

using namespace TGA;

// BGRA
constexpr TGAColor white  = {255, 255, 255, 255};
constexpr TGAColor green  = {  0, 255,   0, 255};
constexpr TGAColor red    = {  0,   0, 255, 255};
constexpr TGAColor blue   = {255, 128,  64, 255};
constexpr TGAColor yellow = {  0, 200, 255, 255};

void line(int x1, int y1, int x2, int y2, TGAImage& image, TGAColor color) {
    bool steep = std::abs(y2 - y1) > std::abs(x2 - x1);
    if (steep) { std::swap(x1, y1); std::swap(x2, y2); }
    if (x1 > x2) { std::swap(x1, x2); std::swap(y1, y2); }

    int dx = x2 - x1;
    int dy = std::abs(y2 - y1);
    int error = 0;
    int derror2 = dy * 2;
    int dx2 = dx * 2;
    int y = y1;
    int y_step = (y2 > y1 ? 1 : -1);

    if (steep) {
        for (int x = x1; x <= x2; x++) {
            image.set(y, x, color);
            error += derror2;
            if (error > dx) {
                y += y_step;
                error -= dx2;
            }
        }
    } else {
        for (int x = x1; x <= x2; x++) {
            image.set(x, y, color);
            error += derror2;
            if (error > dx) {
                y += y_step;
                error -= dx2;
            }
        }
    }
}

int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    int ax =  7, ay =  3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;

    line(ax, ay, bx, by, framebuffer, red);
    line(bx, by, cx, cy, framebuffer, green);
    line(cx, cy, ax, ay, framebuffer, blue);

    framebuffer.write_tga_file("../framebuffer.tga");

    return 0;
}