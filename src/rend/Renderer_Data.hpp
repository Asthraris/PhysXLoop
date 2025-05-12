#include <vector>
#include <memory>
#include <Eigen/Dense>

constexpr double PI = 3.14159265358979323846;

class Mesh {
public:
    std::vector<Eigen::Vector3f> vertices;
    std::vector<uint32_t> indices;

    static Mesh CUBE(float size = 1.0f) {
        Mesh mesh;
        float h = size * 0.5f;

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

    static Mesh CUBOID(const float size[3]) {
        Mesh mesh;
        float hx = size[0] * 0.5f;
        float hy = size[1] * 0.5f;
        float hz = size[2] * 0.5f;

        mesh.vertices = {
            {-hx, -hy, -hz}, { hx, -hy, -hz}, { hx,  hy, -hz}, {-hx,  hy, -hz}, // back
            {-hx, -hy,  hz}, { hx, -hy,  hz}, { hx,  hy,  hz}, {-hx,  hy,  hz}  // front
        };

        mesh.indices = {
            0, 1, 2,  2, 3, 0,
            4, 5, 6,  6, 7, 4,
            4, 5, 1,  1, 0, 4,
            7, 6, 2,  2, 3, 7,
            4, 0, 3,  3, 7, 4,
            5, 1, 2,  2, 6, 5
        };

        return mesh;
    }

    static Mesh SPHERE(int segments = 16, int rings = 16, float radius = 1.0f) {
        Mesh mesh;

        for (int y = 0; y <= rings; ++y) {
            float v = float(y) / rings;
            float phi = v * PI;

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
    std::shared_ptr<Mesh> Cube;
    std::shared_ptr<Mesh> Sphere;
    MeshLibrary() {
        Cube = std::make_shared<Mesh>(Mesh::CUBE);
        Sphere = std::make_shared<Mesh>(Mesh::SPHERE);
    }
    std::shared_ptr<Mesh> CreateCuboid(const float size[3]) {
        std::shared_ptr<Mesh> cuboid = std::make_shared<Mesh>(Mesh::CUBOID(size));
        return cuboid;
    }

};
