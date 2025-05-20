#include <vector>
#include <memory>
#include "ArcBall.hpp"
#include "../Utils.hpp"



class Mesh {
public:
    std::vector<myVec3> vertices;
    std::vector<unsigned int> indices;
    size_t getSizeofShapeVertex() const {
        return vertices.size() * sizeof(myVec3);
    }
    size_t getSizeofShapeIndices() const {
        return indices.size() * sizeof(unsigned int);
    }
    static Mesh createCUBE(float size = 1.0f) {
        Mesh mesh;
        float h = (size * 0.5f) ;

        mesh.vertices = {
            {-h, -h, -h}, { h, -h, -h}, { h,  h, -h}, {-h,  h, -h}, // back
            {-h, -h,  h}, { h, -h,  h}, { h,  h,  h}, {-h,  h,  h}  // front
        };

        mesh.indices = {
            0, 1, 2,  2, 3, 0,  // back
            4, 5, 6,  6, 7, 4,  // front
            4, 5, 1,  1, 0, 4,  // bottom
            7, 6, 2,  2, 3, 7,  // top
            4, 0, 3,  3, 7, 4,  // left
            5, 1, 2,  2, 6, 5   // right
        };

        return mesh;
    }

    

    static Mesh createSPHERE(int segments = 16, int rings = 16, float radius = 1.0f) {
        Mesh mesh;
        for (int y = 0; y <= rings; ++y) {
            float v = float(y) / rings;
            float phi = float(v * PI);

            for (int x = 0; x <= segments; ++x) {
                float u = float(x) / segments;
                float theta = u * 2.0f * PI;

                float xPos = radius * std::sin(phi) * std::cos(theta);
                float yPos = radius * std::cos(phi);
                float zPos = radius * std::sin(phi) * std::sin(theta);

                mesh.vertices.emplace_back(xPos, yPos, zPos);
            }
        }

        for (int y = 0; y < rings; ++y) {
            for (int x = 0; x < segments; ++x) {
                int i0 = y * (segments + 1) + x;
                int i1 = i0 + 1;
                int i2 = i0 + (segments + 1);
                int i3 = i2 + 1;

                mesh.indices.push_back(i0);
                mesh.indices.push_back(i2);
                mesh.indices.push_back(i1);

                mesh.indices.push_back(i1);
                mesh.indices.push_back(i2);
                mesh.indices.push_back(i3);
            }
        }

        return mesh;
    }
};

class MeshLibrary{
public:
    std::unique_ptr<Mesh> Cube_shape_vertex;
    size_t INDICES_COUNT_CUBE;
    std::unique_ptr<Mesh> Sphere_shape_vertex;
    size_t INDICES_COUNT_SPHERE;

    MeshLibrary() {
        Cube_shape_vertex = std::make_unique<Mesh>(Mesh::createCUBE());
        INDICES_COUNT_CUBE = Cube_shape_vertex->indices.size();
        Sphere_shape_vertex = std::make_unique<Mesh>(Mesh::createSPHERE());
        INDICES_COUNT_SPHERE = Sphere_shape_vertex->indices.size();
    }
    
};
