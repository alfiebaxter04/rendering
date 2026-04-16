#include <algorithm>
#include <fstream>
#include <sstream>

#include "obj_handler.h"

namespace Obj {
namespace { 
    enum class ObjTag { V, VT, VN, F, Comment, Unknown };

    ObjTag parse_tag(const std::string& tag) {
        if (tag == "v")  return ObjTag::V;
        if (tag == "vt") return ObjTag::VT;
        if (tag == "vn") return ObjTag::VN;
        if (tag == "f")  return ObjTag::F;
        if (tag == "#")  return ObjTag::Comment;
        return ObjTag::Unknown;
    }
}

bool ObjHandler::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    // Clear existing data in case of re-use
    vertices_.clear();
    textures_.clear();
    normals_.clear();
    faces_.clear();

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string tag_str;
        ss >> tag_str;

        ObjTag tag = parse_tag(tag_str);
        
        switch (tag) {
            case ObjTag::V:
            case ObjTag::VT:
            case ObjTag::VN: handle_vertex(ss, tag_str); break;
            case ObjTag::F:  handle_face(ss);           break;
            default: break; // Ignore comments and unknown tags
        }
    }
    return true;
}

void ObjHandler::handle_vertex(std::stringstream& ss, const std::string& tag) {
    float x, y, z = 0.0f;
    ss >> x >> y;
    
    if (tag != "vt" || (ss >> z)) {
        // Only try to read z if it's not a 2D texture or if z is provided
    }

    if (tag == "v")       vertices_.emplace_back(x, y, z);
    else if (tag == "vt") textures_.emplace_back(x, y, z);
    else if (tag == "vn") normals_.emplace_back(x, y, z);
}

void ObjHandler::handle_face(std::stringstream& ss) {
    Face face;
    std::string segment;

    while (ss >> segment) {
        Index idx;
        std::replace(segment.begin(), segment.end(), '/', ' ');
        std::stringstream seg_ss(segment);
        
        seg_ss >> idx.v;
        if (segment.find("  ") != std::string::npos) {
             seg_ss >> idx.vn;
        } else {
             if (seg_ss >> idx.vt) {
                 seg_ss >> idx.vn;
             }
        }
        face.vertices.push_back(idx);
    }
    faces_.push_back(std::move(face));
}

} // namespace Obj