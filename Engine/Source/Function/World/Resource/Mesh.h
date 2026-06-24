#pragma once

#include "Core.h"
#include "Container/Array.h"

#include <glm/glm.hpp>

/// <summary>
/// 网格
/// </summary>
struct Mesh
{
    Array<glm::vec3> Vertices;      // 顶点位置
    Array<glm::vec3> Normals;       // 法线
    Array<glm::vec2> UV;            // 网格的基础纹理坐标
    Array<glm::vec4> Tangents;      // 切线
    Array<glm::vec4> Colors;        // 顶点颜色
    Array<uint32> Triangles;        // 三角形索引
};

