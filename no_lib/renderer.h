#ifndef RENDERER_H
#define RENDERER_H

#include "tgaimage.h"
#include "objhandler.h"

namespace TGA {

    class Renderer {
    public:
        static void draw_obj(TGAImage& image, const Obj::ObjHandler& handler, const TGAColor& color) {
            for (const auto& face : handler.get_faces()) {
                for (int i = 0; i < 3; ++i) {
                    const auto& v0 = handler.get_vertices()[face.vertices[i].v - 1];
                    const auto& v1 = handler.get_vertices()[face.vertices[(i + 1) % 3].v - 1];

                    int x0 = (v0.x + 1.0f) * image.get_width() / 2.0f;
                    int y0 = (v0.y + 1.0f) * image.get_height() / 2.0f;
                    int x1 = (v1.x + 1.0f) * image.get_width() / 2.0f;
                    int y1 = (v1.y + 1.0f) * image.get_height() / 2.0f;

                    line(x0, y0, x1, y1, image, color);
                }
            }
        }

    private:
        static void line(int x1, int y1, int x2, int y2, TGAImage& image, const TGAColor& color) {
            bool steep = std::abs(y2 - y1) > std::abs(x2 - x1);
            if (steep) {
                std::swap(x1, y1);
                std::swap(x2, y2);
            }
            if (x1 > x2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            int dx = x2 - x1;
            int dy = std::abs(y2 - y1);
            int error = 0;
            int derror2 = dy * 2;
            int dx2 = dx * 2;
            int y = y1;
            int y_step = (y2 > y1 ? 1 : -1);

            for (int x = x1; x <= x2; x++) {
                if (steep) {
                    image.set(y, x, color);
                } else {
                    image.set(x, y, color);
                }
                error += derror2;
                if (error > dx) {
                    y += y_step;
                    error -= dx2;
                }
            }
        }
    };

} // namespace TGA

#endif // RENDERER_H
