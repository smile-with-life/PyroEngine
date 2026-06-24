#version 460
#extension GL_EXT_buffer_reference : require
#extension GL_EXT_scalar_block_layout : require

layout(location = 0) in vec3 pos;

layout(buffer_reference, scalar) buffer MyData {
    mat4 Proj;
    mat4 View;
    mat4 Model;
};

layout(push_constant) uniform Push {
    MyData data;  // 64位设备地址
};

void main()
{
    // 位置变换
    gl_Position = vec4(pos.x * 0.5, pos.y * 0.5, pos.z * 0.5, 1.0);
}