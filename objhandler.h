#ifndef OBJ_HANDLER_H
#define OBJ_HANDLER_H

#include <fstream>
#include <vector>
#include <sstream>
#include <string>

namespace Obj {

    struct Point {
        float x = 0.0f, y = 0.0f, z = 0.0f;
        Point() = default;
        Point(float x, float y, float z) : x(x), y(y), z(z) {};
    };

    struct obj_index {
        int v = -1, vt = -1, vn = -1;
    };

    struct Face {
        std::vector<obj_index> vertices;
    };

    class ObjHandler {
    public:
        ObjHandler() = default;

        bool load(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                return false;
            }

            std::string line;
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string token;
                ss >> token;
                ObjTag tag = parse_tag(token);

                if (tag == ObjTag::BREAK || tag == ObjTag::UNKNOWN) continue;

                handle_line(ss, tag);
            }

            return true;
        }

        const std::vector<Point>& get_vertices() const { return vertices_; }
        const std::vector<Point>& get_textures() const { return textures_; }
        const std::vector<Point>& get_normals() const { return normals_; }
        const std::vector<Face>& get_faces() const { return faces_; }

    private:
        enum class ObjTag { V, VT, VN, F, BREAK, UNKNOWN };

        static ObjTag parse_tag(const std::string& tag) {
            if (tag == "f")  return ObjTag::F;
            if (tag == "v")  return ObjTag::V;
            if (tag == "vt") return ObjTag::VT;
            if (tag == "vn") return ObjTag::VN;
            if (tag == "#") return ObjTag::BREAK;
            return ObjTag::UNKNOWN;
        }

        void handle_line(std::stringstream& ss, const ObjTag tag) {
            if (tag == ObjTag::F) {
                handle_face(ss);
            } else {
                handle_vertex(ss, tag);
            }
        }

        void handle_face(std::stringstream& ss) {
            Face face;
            std::string vertex_group;

            while (ss >> vertex_group) {
                std::stringstream gs(vertex_group);
                std::string idx_str;
                obj_index idx;

                // Vertex index
                std::getline(gs, idx_str, '/');
                if (!idx_str.empty()) idx.v = std::stoi(idx_str);

                // Texture index
                if (gs.peek() != '/') { // check if there is a texture coord
                    std::getline(gs, idx_str, '/');
                    if (!idx_str.empty()) idx.vt = std::stoi(idx_str);
                } else {
                    gs.ignore();
                }

                // Normal index
                std::getline(gs, idx_str);
                if (!idx_str.empty()) idx.vn = std::stoi(idx_str);

                face.vertices.push_back(idx);
            }
            faces_.push_back(face);
        }

        void handle_vertex(std::stringstream& ss, const ObjTag tag) {
            float x, y, z;
            ss >> x >> y >> z;
            switch (tag) {
                case ObjTag::V:  vertices_.emplace_back(x, y, z); break;
                case ObjTag::VT: textures_.emplace_back(x, y, z); break;
                case ObjTag::VN: normals_.emplace_back(x, y, z);  break;
                default: break;
            }
        }

        std::vector<Point> vertices_;
        std::vector<Point> textures_;
        std::vector<Point> normals_;
        std::vector<Face> faces_;
    };
};

#endif