#pragma once

#include <vector>
#include <string>

namespace Obj {

struct Point {
    float x = 0.0f, y = 0.0f, z = 0.0f;
    
    constexpr Point() = default;
    constexpr Point(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Index {
    int v = -1, vt = -1, vn = -1;
};

struct Face {
    std::vector<Index> vertices;
};

class ObjHandler {
public:
    ObjHandler() = default;

    [[nodiscard]] bool load(const std::string& filename);

    const std::vector<Point>& vertices() const { return vertices_; }
    const std::vector<Point>& textures() const { return textures_; }
    const std::vector<Point>& normals()  const { return normals_; }
    const std::vector<Face>&  faces()    const { return faces_; }

private:
    void handle_face(std::stringstream& ss);
    void handle_vertex(std::stringstream& ss, const std::string& tag);

    std::vector<Point> vertices_;
    std::vector<Point> textures_;
    std::vector<Point> normals_;
    std::vector<Face>  faces_;
};

} // namespace Obj