#ifndef OBJ_HANDLER_H
#define OBJ_HANDLER_H

#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

namespace Obj {

    enum class ObjTag { V, VT, VN, F, BREAK, UNKNOWN };
    ObjTag parse_tag(const std::string& tag) {
        if (tag == "f")  return ObjTag::F;
        if (tag == "v")  return ObjTag::V;
        if (tag == "vt") return ObjTag::VT;
        if (tag == "vn") return ObjTag::VN;
        if (tag == "#") return ObjTag::BREAK;
        return ObjTag::UNKNOWN;
    }

    struct Point {
        float x, y, z;
        constexpr Point() : x(0), y(0), z(0) {};
        constexpr Point(float x, float y, float z) :
            x(x), y(y), z(z) {};
    };

    struct obj_index {
        int v = -1, vt = -1, vn = -1;
    };

    struct Face {
        obj_index vertices[3];
    };

    class ObjHandler {
    public:
        std::vector<Point> vertices;
        std::vector<Point> textures;
        std::vector<Point> normals;
        std::vector<Face> faces;

        ObjHandler(std::string& filename) {
            file = std::ifstream(filename);

            if (!file.is_open()) {
                std::cout << "[OBJ] Failed to open file: " << filename << std::endl;
                return;
            }

            std::string line;
            while (std::getline(file, line)) {
                std::cout << "[OBJ] Processing line: " << line << std::endl;
                std::stringstream ss(line);
                std::string token;
                ss >> token;
                ObjTag tag = parse_tag(token); 

                if (tag == ObjTag::BREAK || tag == ObjTag::UNKNOWN) continue;

                handle_line(ss, tag);
            }

            std::cout << "[OBJ] Loaded " << vertices.size() << " vertices, " << textures.size() << " textures, " << normals.size() << " normals, " << faces.size() << " faces." << std::endl;
        }

    private:
        std::ifstream file;

        void handle_line(std::stringstream& ss, const ObjTag tag) {
            if (tag == ObjTag::F) handle_face(ss);
            else handle_vertex(ss, tag);
        }

        void handle_face(std::stringstream& ss) {
            Face face;
            std::string vertex_group;

            for (int i = 0; i < 3; i++) {
                ss >> vertex_group;
                std::stringstream gs(vertex_group);
                std::string idx;

                std::getline(gs, idx, '/');
                if (!idx.empty()) face.vertices[i].v = stoi(idx);
                std::getline(gs, idx, '/');
                if (!idx.empty()) face.vertices[i].vt = stoi(idx);
                std::getline(gs, idx, '/');
                if (!idx.empty()) face.vertices[i].vn = stoi(idx);
            }
            faces.push_back(face);

           
            std::cout << "[OBJ] Added face: " << face.vertices[0].v << "/" << face.vertices[0].vt << "/" << face.vertices[0].vn << " "
                << face.vertices[1].v << "/" << face.vertices[1].vt << "/" << face.vertices[1].vn << " "
                << face.vertices[2].v << "/" << face.vertices[2].vt << "/" << face.vertices[2].vn << std::endl;
        }

        void handle_vertex(std::stringstream& ss, const ObjTag tag) {
            float x, y, z;
            ss >> x >> y >> z;

            std::string type;
            switch (tag) {
                case ObjTag::V:
                    vertices.push_back(Point(x, y, z));
                    type = "vertex";
                    break;
                case ObjTag::VT:
                    textures.push_back(Point(x, y, z));
                    type = "texture";
                    break;
                case ObjTag::VN:
                    normals.push_back(Point(x, y, z));
                    type = "normal";
                    break;
                default:
                    type = "unknown";
                    break;
            }
            std::cout << "[OBJ] Added " << type << ": " << x << " " << y << " " << z << std::endl;
        }
    };
};

#endif