#pragma once

namespace Obj { class ObjHandler; }
namespace TGA { 
    class TGAImage; 
    struct TGAColor; 
}

namespace TGA::Renderer {

    void draw_obj(TGAImage& image, const Obj::ObjHandler& handler, const TGAColor& color);

    void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

} // namespace TGA::Renderer