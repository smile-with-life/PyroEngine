#pragma once

#include "Core.h"

#include "Runtime.h"
#include "Event/EventSystem.h"
#include "Container/Array.h"
#include "Logger/Logger.h"
#include "RHI/RHI.h"
#include "FileSystem/File.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

//#include "tiny_obj_loader.h"

//// 顶点结构体定义
//struct Vertex
//{
//    glm::vec3 Position;     // 位置
//    glm::vec3 Normal;       // 法线
//    glm::vec2 UV;           // 纹理坐标
//    glm::vec4 Color;        // 顶点色
//};
//
//struct ShaderData 
//{
//    glm::mat4 Projection;
//    glm::mat4 View;
//    glm::mat4 Model;
//    glm::vec4 LightPosition{ 0.0f, -10.0f, 10.0f, 0.0f };
//    uint32 Selected{ 1 };
//};
//
//
//constexpr uint32 maxFramesInFlight{ 2 };
//
//class RenderTest
//{
//public:
//    void RenderInit(ViewPtr<Window> window)
//    {
//        m_window = window;
//        GEventSystem->Subscribe("WindowResizeEvent", MemberFuncBind(OnEvent));
//
//
//        // 1. 定义变量
//        tinyobj::attrib_t attrib;          // 顶点数据（位置/法线/纹理坐标）
//        std::vector<tinyobj::shape_t> shapes; // 模型网格
//        std::vector<tinyobj::material_t> materials; // 材质
//        std::string warn, err;
//
//        // 2. 加载 OBJ 文件（替换成你的模型路径）
//        bool result = tinyobj::LoadObj(
//            &attrib, &shapes, &materials, &warn, &err,
//            "D:/Workspace/Develop/PyroEngine/Engine/Source/Module/Render/cube.obj"  // <-- 你的模型文件名
//        );
//
//        // 3. 检查是否加载成功
//        if (!result)
//        {
//            GLog->Error("无法加载模型文件！{}", err);
//            return;
//        }
//        GLog->Info("模型加载成功！");
//        GLog->Info("顶点数量: {}", attrib.vertices.size());
//        // 正确加载 OBJ 顶点 + 索引
//        Array<Vertex> vertices;
//        Array<uint16> indices;
//
//        // 1. 先遍历所有面，生成唯一顶点（去重，避免重复顶点）
//        std::unordered_map<size_t, uint16> vertexMap;
//        for (auto& index : shapes[0].mesh.indices)
//        {
//            // 用顶点索引、法线索引、UV索引组合作为唯一键
//            size_t key = (size_t)index.vertex_index << 32 | (size_t)index.normal_index << 16 | index.texcoord_index;
//            if (vertexMap.find(key) == vertexMap.end())
//            {
//                Vertex vertex;
//                // 正确读取坐标，不翻转Y
//                vertex.Position = {
//                    attrib.vertices[3 * index.vertex_index + 0],
//                    attrib.vertices[3 * index.vertex_index + 1],
//                    attrib.vertices[3 * index.vertex_index + 2]
//                };
//                // 正确读取法线
//                vertex.Normal = {
//                    attrib.normals[3 * index.normal_index + 0],
//                    attrib.normals[3 * index.normal_index + 1],
//                    attrib.normals[3 * index.normal_index + 2]
//                };
//                // 正确读取UV，翻转Y轴（纹理坐标标准）
//                vertex.UV = {
//                    attrib.texcoords[2 * index.texcoord_index + 0],
//                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
//                };
//                vertex.Color = glm::vec4(1.0f);
//
//                vertexMap[key] = (uint16)vertices.Size();
//                vertices.Add(vertex);
//            }
//            // 用真实索引，而不是自己生成！
//            indices.Add(vertexMap[key]);
//        }
//
//        vertexCount = vertices.Size();
//        indexCount = indices.Size();
//
//        uint64 capacity = vertices.Size() * sizeof(Vertex) + indices.Size() * sizeof(uint16);
//        RHIBufferUsageFlag usageFlag = RHIBufferUsageFlag::Vertex | RHIBufferUsageFlag::Index;
//        m_buffer = GRHI->CreateDynamicBuffer(usageFlag, capacity);
//        void* mappedPtr = m_buffer->Map();
//
//        memcpy(mappedPtr, vertices.Data(), vertices.Size() * sizeof(Vertex));
//        memcpy(((char*)mappedPtr) + vertices.Size() * sizeof(Vertex), indices.Data(), indices.Size() * sizeof(uint16));
//
//        m_commandPool = GRHI->CreateCommandPool(RHIQueueType::Graphics);
//        for (int32 i = 0; i < FrameInFlight; ++i)
//        {
//            m_fences.Add(GRHI->CreateFence(true));
//            m_imageAvaliableSemaphores.Add(GRHI->CreateSemaphore());
//            m_renderFinishSemaphores.Add(GRHI->CreateSemaphore());
//            m_commmandLists.Add(GRHI->CreatePrimaryCommandList(m_commandPool));
//        }
//        
//
//        m_swapchain = GRHI->CreateSwapchain(m_window.RawPtr());
//
//        
//        File vertexFile("D:/Workspace/Develop/PyroEngine/Engine/Asset/Shader/cube_vertex.spv");
//        File fragmentFile("D:/Workspace/Develop/PyroEngine/Engine/Asset/Shader/cube_pixel.spv");
//        auto vertexFileStream = vertexFile.Open(OpenMode::Binary);
//        auto fragmentFileStream = fragmentFile.Open(OpenMode::Binary);
//        
//        vertexFileStream.ReadAll(vertexSource);
//        fragmentFileStream.ReadAll(fragmentSource);
//        vertexFileStream.Close();
//        fragmentFileStream.Close();
//
//        auto vertexShader = GRHI->CreateShader(ShaderUsage::Vertex, vertexSource);
//        auto fragmentShader = GRHI->CreateShader(ShaderUsage::Pixel, fragmentSource);
//        RHIGraphicsPipelineState pipelineState{};
//        pipelineState.Shaders.Add(vertexShader);
//        pipelineState.Shaders.Add(fragmentShader);
//        pipelineState.Stride = sizeof(Vertex);
//        m_pipeline = GRHI->CreateGraphicsPipeline(pipelineState);
//
//        // uniform
//        m_uniformBuffer = GRHI->CreateDynamicBuffer(RHIBufferUsageFlag::Uniform, sizeof(ShaderData));
//    }
//
//    void RenderTick()
//    {
//        // 1. 先检查窗口是否有效
//        if (m_window.IsExpired())
//        {
//            GLog->Debug("Window is invalid, skipping render");
//            GRHI->WaitIdle();
//            return;  // 直接返回，不尝试渲染
//        }
//
//        try
//        {
//            m_fences[currentFrame]->Wait();
//
//            uint32 imageIndex = m_swapchain->AcquireNextImageIndex(m_imageAvaliableSemaphores[currentFrame]);
//
//            m_fences[currentFrame]->Reset();
//
//            m_commmandLists[currentFrame]->Reset();
//
//            // ===================== 核心：立方体旋转逻辑 =====================
//            static float angle = 0.0f;
//            angle += 0.01f; // 旋转速度
//
//            glm::mat4 model = glm::mat4(1.0f);
//            model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f)); // 绕轴旋转
//            model = glm::scale(model, glm::vec3(1.0f)); // 缩放
//
//            glm::mat4 view = glm::lookAt(
//                glm::vec3(3, 3, 5),   // 相机位置
//                glm::vec3(0, 0, 0),   // 观察目标
//                glm::vec3(0, 1, 0)    // 上方向
//            );
//
//            glm::mat4 proj = glm::perspective(
//                glm::radians(60.0f),
//                (float)m_swapchain->GetWidth() / m_swapchain->GetHeight(),
//                0.1f, 100.0f
//            );
//
//            // 写入Uniform数据
//            ShaderData data = {};
//            data.Model = model;
//            data.View = view;
//            data.Projection = proj;
//            data.LightPosition = glm::vec4(3, -3, 5, 0);
//            data.Selected = 1;
//
//            void* uniformMapped = m_uniformBuffer->Map();
//            memcpy(uniformMapped, &data, sizeof(ShaderData));
//            m_uniformBuffer->Unmap();
//            // 2. 获取 GPU 设备地址
//            uint64 deviceAddress = m_uniformBuffer->GetDeviceAddress();
//            // =================================================================
//
//            m_commmandLists[currentFrame]->Begin(RHICommandType::Single);
//            {
//                m_commmandLists[currentFrame]->SetViewport(0.0f, 0.0f, m_swapchain->GetWidth(), m_swapchain->GetHeight(), 0.0f, 1.0f);
//                m_commmandLists[currentFrame]->SetScissor(0, 0, m_swapchain->GetWidth(), m_swapchain->GetHeight());
//                m_commmandLists[currentFrame]->SetPrimitiveTopology(PrimitiveTopology::TriangleList);
//                m_commmandLists[currentFrame]->SetCullMode(CullMode::Back);
//                m_commmandLists[currentFrame]->SetFrontFace(FrontFace::CounterClockwise);
//                m_commmandLists[currentFrame]->BeginRendering(m_swapchain);
//                {
//                    m_commmandLists[currentFrame]->BindGraphicsPipeline(m_pipeline);
//                    // 绑定顶点缓冲区 + 索引缓冲区
//                    uint64 offset = 0;
//                    m_commmandLists[currentFrame]->BindVertexBuffer(m_buffer, 0, offset);
//                    m_commmandLists[currentFrame]->BindIndexBuffer(m_buffer, vertexCount * sizeof(Vertex), RHIIndexType::Uint16);
//
//                    m_commmandLists[currentFrame]->PushConstant(ConstantRangeFlag::Vertex, 0, sizeof(uint64), &deviceAddress);
//
//                    // 绘制
//                    m_commmandLists[currentFrame]->DrawIndexed(indexCount, 1, 0, 0, 0);
//                }
//                m_commmandLists[currentFrame]->EndRendering(imageIndex, m_swapchain);
//            }
//            m_commmandLists[currentFrame]->End();
//
//            RHISubmitInfo submitInfo;
//            submitInfo.CommandList = m_commmandLists[currentFrame];
//            submitInfo.Fence = m_fences[currentFrame];
//            submitInfo.ImageAvailableSemaphore = m_imageAvaliableSemaphores[currentFrame];
//            submitInfo.RenderFinishSemaphore = m_renderFinishSemaphores[currentFrame];
//
//            GRHI->Submit(RHIQueueType::Graphics, submitInfo);
//
//            m_swapchain->Present(imageIndex, m_renderFinishSemaphores[currentFrame]);
//
//            // 循环帧索引
//            currentFrame = (currentFrame + 1) % maxFramesInFlight;
//        }
//        catch (std::exception exception)
//        {
//            GLog->Error("Render Error: {}", exception.what());
//        }
//        
//    }
//
//    void RenderExit()
//    {
//        GEventSystem->Unsubscribe("WindowResizeEvent", MemberFuncBind(OnEvent));
//        delete m_pipeline;
//        m_fences.Clear();
//        m_imageAvaliableSemaphores.Clear();
//        m_renderFinishSemaphores.Clear();
//        m_commmandLists.Clear();
//        m_commandPool.Destroy();
//        m_swapchain.Destroy();
//        m_buffer.Destroy();
//        m_uniformBuffer.Destroy();
//
//    }
//
//    void OnEvent(Event& event)
//    {
//        switch (event.GetType())
//        {
//        case EventType("WindowResizeEvent"):
//            auto& resizeEvent = static_cast<WindowResizeEvent&>(event);
//            m_swapchain->RecreateSwapchain();
//            break;
//        }
//    }
//public:
//    static RenderTest& GetInstance()
//    {
//        static RenderTest instance;
//        return instance;
//    }
//public:
//    uint32 FrameInFlight = 2;
//    uint32 currentFrame = 0;
//    Array<RefPtr<RHIFence>> m_fences;
//    Array<RefPtr<RHISemaphore>> m_imageAvaliableSemaphores;
//    Array<RefPtr<RHISemaphore>> m_renderFinishSemaphores;
//    Array<RefPtr<RHIPrimaryCommandList>> m_commmandLists;
//
//    RefPtr<RHICommandPool> m_commandPool;
//    RefPtr<RHISwapchain> m_swapchain;
//    RHIGraphicsPipeline* m_pipeline;
//    RefPtr<RHIDynamicBuffer> m_buffer;
//    RefPtr<RHIDynamicBuffer> m_uniformBuffer; // 新增：Uniform缓冲区
//
//    ByteArray vertexSource;
//    ByteArray fragmentSource;
//
//    uint32 vertexCount = 0;
//    uint32 indexCount = 0;
//    ViewPtr<Window> m_window;
//};