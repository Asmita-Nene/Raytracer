#include "ObjFileHandle.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "UtilityClasses.hpp"
#include "Scene.hpp"
#include "Material.hpp"

ObjFileHandle::ObjFileHandle(const std::string& filename) : filename(filename) {}

// 1-based -> 0-based, handling OBJ's negative/relative index convention.
static size_t resolveIndex(int rawIdx, size_t currentCount) {
    if (rawIdx > 0) return static_cast<size_t>(rawIdx - 1);
    return currentCount + static_cast<size_t>(rawIdx); // rawIdx negative
}

bool ObjFileHandle::constructScene(Scene& scene, std::vector<std::unique_ptr<Material>>& materialPool) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return false;
    }

    materialPool.push_back(std::make_unique<Diffuse>(Color(1.00, 0.50, 0.50))); // TODO: implement per-face materials later
    Material* defaultMaterial = materialPool.back().get();

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        std::istringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "v") {
            Point3 v;
            ss >> v.x >> v.y >> v.z;
            if (ss.fail()) { std::cerr << "Skipping malformed vertex: " << line << "\n"; continue; }
            verts.push_back(v);
        }
        else if (token == "vn") {
            Vector3 n;
            ss >> n.x >> n.y >> n.z;
            if (ss.fail()) { std::cerr << "Skipping malformed normal: " << line << "\n"; continue; }
            norms.push_back(n);
        }
        else if (token == "f") {
            std::vector<size_t> faceVertIdx;
            int faceNormRaw = 0;
            bool haveNormal = false, malformed = false;
            std::string corner;

            while (ss >> corner) {
                size_t firstSlash = corner.find('/');
                size_t secondSlash = corner.find('/', firstSlash + 1);
                if (firstSlash == std::string::npos || secondSlash == std::string::npos) {
                    std::cerr << "Skipping face, unexpected corner format \"" << corner << "\"\n";
                    malformed = true; break;
                }
                try {
                    int rawVertIdx = std::stoi(corner.substr(0, firstSlash));
                    faceVertIdx.push_back(resolveIndex(rawVertIdx, verts.size()));

                    std::string normToken = corner.substr(secondSlash + 1);
                    if (!normToken.empty() && !haveNormal) {
                        faceNormRaw = std::stoi(normToken); // first corner's normal, used for whole (flat) triangle
                        haveNormal = true;
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Skipping face, bad index \"" << corner << "\": " << e.what() << "\n";
                    malformed = true; break;
                }
            }

            if (malformed || faceVertIdx.size() != 3 || !haveNormal) {
                if (!malformed) std::cerr << "Skipping non-triangular/normal-less face: " << line << "\n";
                continue;
            }

            try {
                const Point3& V0 = verts.at(faceVertIdx[0]);
                const Point3& V1 = verts.at(faceVertIdx[1]);
                const Point3& V2 = verts.at(faceVertIdx[2]);
                const Vector3& normal = norms.at(resolveIndex(faceNormRaw, norms.size()));
                scene.addPrimitiveObject(std::make_unique<Triangle>(V0, V1, V2, normal, defaultMaterial, true));
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Face index out of range, skipping: " << line << " (" << e.what() << ")\n";
            }
        }
    }
    return true;
}