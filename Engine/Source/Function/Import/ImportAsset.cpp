#include "pch.h"

#include "ImportAsset.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

bool ImportObj(const String& path, Mesh& mesh)
{
    mesh.Vertices.Clear();
    mesh.Normals.Clear();
    mesh.UV.Clear();
    mesh.Tangents.Clear();
    mesh.Colors.Clear();
    mesh.Triangles.Clear();

    // 旧版 tinyobj 接口（兼容所有版本）
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.ToCString());

    if (!ret)
    {
        return false;
    }

    std::unordered_map<uint64, uint32> vertexMap;

    for (const auto& shape : shapes)
    {
        const auto& indices = shape.mesh.indices;

        for (size_t i = 0; i < indices.size(); i++)
        {
            const auto& idx = indices[i];

            uint64 key = (uint64)idx.vertex_index << 40
                | (uint64)idx.texcoord_index << 20
                | (uint64)idx.normal_index;

            if (vertexMap.count(key))
            {
                mesh.Triangles.Push(vertexMap[key]);
                continue;
            }

            uint32 newIndex = (uint32)mesh.Vertices.Size();
            vertexMap[key] = newIndex;

            // 顶点
            glm::vec3 v(
                attrib.vertices[3 * idx.vertex_index + 0],
                attrib.vertices[3 * idx.vertex_index + 1],
                attrib.vertices[3 * idx.vertex_index + 2]
            );

            // UV
            glm::vec2 uv(0.0f);
            if (idx.texcoord_index >= 0)
            {
                uv.x = attrib.texcoords[2 * idx.texcoord_index + 0];
                uv.y = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];
            }

            // 法线
            glm::vec3 n(0, 0, 1);
            if (idx.normal_index >= 0)
            {
                n.x = attrib.normals[3 * idx.normal_index + 0];
                n.y = attrib.normals[3 * idx.normal_index + 1];
                n.z = attrib.normals[3 * idx.normal_index + 2];
            }

            mesh.Vertices.Push(v);
            mesh.UV.Push(uv);
            mesh.Normals.Push(n);
            mesh.Colors.Push(glm::vec4(1.0f));
            mesh.Triangles.Push(newIndex);
        }
    }

    // 计算切线
    mesh.Tangents.Resize(mesh.Vertices.Size());

    uint32 triCount = (uint32)mesh.Triangles.Size() / 3;
    for (uint32 i = 0; i < triCount; i++)
    {
        uint32 i0 = mesh.Triangles[i * 3 + 0];
        uint32 i1 = mesh.Triangles[i * 3 + 1];
        uint32 i2 = mesh.Triangles[i * 3 + 2];

        glm::vec3 v0 = mesh.Vertices[i0];
        glm::vec3 v1 = mesh.Vertices[i1];
        glm::vec3 v2 = mesh.Vertices[i2];

        glm::vec2 uv0 = mesh.UV[i0];
        glm::vec2 uv1 = mesh.UV[i1];
        glm::vec2 uv2 = mesh.UV[i2];

        glm::vec3 dp1 = v1 - v0;
        glm::vec3 dp2 = v2 - v0;
        glm::vec2 duv1 = uv1 - uv0;
        glm::vec2 duv2 = uv2 - uv0;

        float r = 1.0f / (duv1.x * duv2.y - duv1.y * duv2.x + 1e-8f);
        glm::vec3 tan = (dp1 * duv2.y - dp2 * duv1.y) * r;

        mesh.Tangents[i0] += glm::vec4(tan, 0.0f);
        mesh.Tangents[i1] += glm::vec4(tan, 0.0f);
        mesh.Tangents[i2] += glm::vec4(tan, 0.0f);
    }

    for (auto& t : mesh.Tangents)
    {
        t = glm::normalize(t);
        t.w = 1.0f;
    }
}