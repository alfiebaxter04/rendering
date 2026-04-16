#include <cmath>
#include <algorithm>

#include "renderer.h"
#include "tga_image.h"
#include "obj_handler.h"

namespace TGA::Renderer {

void draw_obj(TGAImage& image, const Obj::ObjHandler& handler, const TGAColor& color) {
    const int w = image.width();
    const int h = image.height();

    for (const auto& face : handler.faces()) {
        for (int i = 0; i < 3; ++i) {
            
            const auto& v0 = handler.vertices()[face.vertices[i].v - 1];
            const auto& v1 = handler.vertices()[face.vertices[(i + 1) % 3].v - 1];

            int x0 = static_cast<int>((v0.x + 1.0f) * w / 2.0f);
            int y0 = static_cast<int>((v0.y + 1.0f) * h / 2.0f);
            int x1 = static_cast<int>((v1.x + 1.0f) * w / 2.0f);
            int y1 = static_cast<int>((v1.y + 1.0f) * h / 2.0f);

            line(x0, y0, x1, y1, image, color);
        }
    }
}

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color) {
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    const int dx = x1 - x0;
    const int dy = std::abs(y1 - y0);
    int error = dx / 2;
    const int y_step = (y0 < y1 ? 1 : -1);
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        
        error -= dy;
        if (error < 0) {
            y += y_step;
            error += dx;
        }
    }
}

} // namespace TGA::Renderer