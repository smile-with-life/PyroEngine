#pragma once

#include "Core.h"

/// <summary>
/// 图形 API 类型
/// </summary>
enum class GraphicsAPI
{
    None,
    Vulkan,         // Vulkan API
    DirectX12,      // D3D12 API
    Metal           // Metal API
};

/// <summary>
/// 渲染硬件设备类型
/// </summary>
enum class RHIDeviceType
{
    None,
    CPU,            // 软件渲染设备
    DiscreteGPU,    // 独立显卡
    VirtualGPU,     // 虚拟显卡
    IntegratedGPU   // 集成显卡
};

/// <summary>
/// GPU 队列类型
/// </summary>
enum class RHIQueueType
{
    Graphics,       // 图形队列 (支持图形、计算、传输)
    Compute,        // 计算队列(仅支持计算)
    Transfer,       // 传输队列 (仅支持内存传输)
    TileMapping     // 瓦片映射队列 (用于稀疏资源)
};

/// <summary>
/// 资源类型
/// </summary>
enum class RHIResourceUsage
{
    Buffer,
    Shader,
    Texture,
    Pipeline,
    Fence,
    Semaphore,
    Swapchain,
    CommandList
};

/// <summary>
/// Buffer 用途
/// </summary>
enum class RHIBufferUsageFlag : uint32
{
    None = 0,
    Vertex = 1 << 0,
    Index = 1 << 1,
    Uniform = 1 << 2,
    Storage = 1 << 3,
    Indirect = 1 << 4,
    Instance = 1 << 5
};

/// <summary>
/// Shader 用途
/// </summary>
enum class ShaderUsage
{
    None = 0,
    Vertex = 1,
    Mesh = 2, 
    Pixel = 3,
    Compute = 4,
    Geometry = 5,
    Amplification = 6
};

/// <summary>
/// 
/// </summary>
enum class ConstantRangeFlag : uint32
{
    None = 0,
    Vertex = 1 << 1,
    Mesh = 1 << 2,
    Pixel = 1 << 3,
    Compute = 1 << 4,

    All = Vertex | Mesh | Pixel | Compute
};

/// <summary>
/// 命令执行类型
/// </summary>
enum class RHICommandType
{
    Single,             // 只会被提交执行一次
    Simultaneous,       // 可多次提交、多队列并行执行，
};

/// <summary>
/// 图元拓扑类型
/// </summary>
enum class PrimitiveTopology
{
    PointList,      // 点列表
    LineList,       // 线列表
    LineStrip,      // 线带
    TriangleList,   // 三角形列表
    TriangleStrip,  // 三角形带
    PatchList       // 面片列表 (用于曲面细分)
};

/// <summary>
/// 多边形渲染模式
/// </summary>
enum class PolygonMode
{
    Fill,          // 填充模式 - 多边形内部完全填充
    Line,          // 线框模式 - 只绘制多边形边线
    Point,         // 点模式 - 只绘制多边形顶点
};

/// <summary>
/// 面剔除模式
/// </summary>
enum class CullMode
{
    None,          // 不禁用面剔除，渲染所有三角形
    Front,         // 剔除正面（观察者能看到的）
    Back,          // 剔除背面（观察者看不到的）
};

/// <summary>
/// 正面朝向定义
/// </summary>
enum class FrontFace
{
    Clockwise,          // 顺时针方向为正面
    CounterClockwise    // 逆时针方向为正面   
};

/// <summary>
/// 每像素采样次数
/// </summary>
enum class SampleCount
{
    X1,     // 无抗锯齿，锯齿明显
    X2,     // 轻微平滑，边缘改善有限
    X4,     // 质量较好，多数情况下锯齿不明显
    X8      // 非常平滑，接近完美抗锯齿
};

/// <summary>
/// 深度/模板比较函数
/// </summary>
enum class CompareOperation
{
    Never,              // 永不通过
    Less,               // 小于时通过 (src < ref)
    Equal,              // 等于时通过 (src == ref)
    LessEqual,          // 小于等于时通过 (src <= ref)
    Greater,            // 大于时通过 (src > ref)
    NotEqual,           // 不等于时通过 (src != ref)
    GreaterEqual,       // 大于等于时通过 (src >= ref)
    Always,             // 总是通过
};

/// <summary>
/// 颜色混合因子(暂不使用)
/// </summary>
enum class BlendFactor
{
    Zero,               // 0.0
    One,                // 1.0
    SrcColor,           // 源颜色 (Rs, Gs, Bs, As)
    InvSrcColor,        // 1 - 源颜色
    DestColor,          // 目标颜色 (Rd, Gd, Bd, Ad)
    InvDestColor,       // 1 - 目标颜色
    SrcAlpha,           // 源Alpha值 (As, As, As, As)
    InvSrcAlpha,        // 1 - 源Alpha值
    DestAlpha,          // 目标Alpha值 (Ad, Ad, Ad, Ad)
    InvDestAlpha,       // 1 - 目标Alpha值
    ConstantColor,      // 常量颜色 (Rc, Gc, Bc, Ac)
    InvConstantColor,   // 1 - 常量颜色
    ConstantAlpha,      // 常量Alpha值 (Ac, Ac, Ac, Ac)
    InvConstantAlpha,   // 1 - 常量Alpha值
    SrcAlphaSaturate,   // 源Alpha饱和 (min(As, 1 - Ad))
};

/// <summary>
/// 混合操作(暂不使用)
/// </summary>
enum class BlendOperation
{
    Add,              // 源 + 目标
    Subtract,         // 源 - 目标
    ReverseSubtract,  // 目标 - 源
    Min,              // 取最小值
    Max,              // 取最大值
};

/// <summary>
/// 模板操作
/// </summary>
enum class StencilOperation
{
    Keep,               // 保持当前值不变
    Zero,               // 设置为0
    Replace,            // 设置为参考值
    IncrementClamp,     // 增加并钳制到最大值
    DecrementClamp,     // 减少并钳制到0
    Invert,             // 按位取反
    IncrementWrap,      // 增加并回绕
    DecrementWrap,      // 减少并回绕
};

/// <summary>
/// 像素格式
/// </summary>
enum class PixelFormat
{
    None = 0,                      // 未定义/无效格式

    // --------------------------
    // 8位 单通道（遮罩、强度、PBR贴图）
    // --------------------------
    R8Unorm,                       // 单通道 0~1   → 粗糙度、金属度、AO、灰度图、遮罩
    R8Snorm,                       // 单通道 -1~1  → 极小范围向量/偏移（极少用）
    R8Uint,                        // 单通道 整数  → ID纹理、索引、模板数据
    R8Sint,                        // 单通道 有符号整数 → 极少用
    R8SRGB,                        // 单通道 sRGB  → 灰度图（伽马校正）


    // --------------------------
    // 8位 双通道（法线贴图、二维向量）
    // --------------------------
    RG8Unorm,                      // 双通道 0~1   → 法线贴图(存XY)、MotionVector
    RG8Snorm,                      // 双通道 -1~1  → 法线贴图、高精度向量
    RG8Uint,                       // 双通道 整数  → 极少用
    RG8Sint,                       // 双通道 有符号整数 → 极少用
    RG8SRGB,                       // 双通道 sRGB  → 极少用


    // --------------------------
    // 8位 四通道（最常用：颜色、UI、贴图）
    // --------------------------
    RGBA8Unorm,                    // 标准RGBA颜色 → UI、渲染目标、非sRGB贴图
    RGBA8Snorm,                    // RGBA -1~1  → 极少用
    RGBA8Uint,                     // RGBA 整数  → 数据纹理
    RGBA8Sint,                     // RGBA 有符号整数 → 极少用
    RGBA8SRGB,                     // RGBA sRGB  →  diffuse/BaseColor贴图（美术贴图必用）

    BGRA8Unorm,                    // BGRA格式     → 窗口交换链、系统图片（安卓/iOS常用）
    BGRA8SRGB,                     // BGRA sRGB    → 交换链+sRGB


    // --------------------------
    // 16位 浮点/整数（HDR、高精度RT）
    // --------------------------
    R16Float,                      // 单通道半精度浮点 → 高度图、阴影、体积纹理
    R16Unorm,                      // 单通道16位 0~1
    R16Snorm,                      // 单通道16位 -1~1
    R16Uint,                       // 单通道16位 整数
    R16Sint,                       // 单通道16位 有符号整数

    RG16Uint,
    RG16Sint,
    RG16Float,                     // 双通道半精度浮点 → 高精度法线、运动向量
    RGBA16Float,                   // 四通道半精度浮点 → HDR渲染目标、延迟渲染RT


    // --------------------------
    // 32位 浮点（超高精度：深度、计算缓冲）
    // --------------------------
    R32Float,                      // 单通道32位浮点 → 高精度深度、距离场、计算缓冲
    R32Uint,                       // 单通道32位整数 → 索引、ID
    R32Sint,                       // 单通道32位有符号整数

    RG32Float,                     // 双通道32位浮点 → 高精度向量场
    RGBA32Float,                   // 四通道32位浮点 → 超高精度RT、GBuffer


    // --------------------------
    // 打包格式（高性能、省带宽、HDR必备）
    // --------------------------
    RGB10A2Unorm,                  // R10G10B10A2   → 高精度颜色、HDR、法线、GBuffer（比RGBA16F更快）
    RGB10A2Uint,                   // 整数打包格式 → 数据纹理

    R11G11B10Float,                // 无符号浮点RGB → 高性能HDR、光照缓冲（最常用HDR格式）
    R9G9B9E5Float,                 // 共享指数RGB  → HDR环境贴图、高光


    // --------------------------
    // 深度 / 模板（渲染核心！）
    // --------------------------
    Depth16Unorm,                  // 16位深度     → 低配、移动端阴影
    Depth24UnormStencil8,          // 24位深度+8位模板 → 99%场景默认深度缓冲
    Depth32Float,                  // 32位浮点深度 → 高精度深度、远距离物体
    Depth32FloatStencil8,          // 32位深度+8位模板 → 高质量渲染


    // --------------------------
    // 压缩纹理（PC/主机/高端iOS）
    // --------------------------
    BC1Unorm,          // BC1(DXT1)  → RGB无Alpha、低质量贴图
    BC1SRGB,           // BC1 sRGB

    BC2Unorm,          // BC2(DXT3)  → RGB+尖锐Alpha
    BC2SRGB,           // BC2 sRGB

    BC3Unorm,          // BC3(DXT5)  → RGBA通用压缩（最常用）
    BC3SRGB,           // BC3 sRGB

    BC4Unorm,          // BC4        → 单通道压缩（粗糙度/AO）
    BC4Snorm,         // BC4 有符号

    BC5Unorm,          // BC5        → 双通道压缩（法线贴图专用！）
    BC5Snorm,         // BC5 有符号

    BC6HSFloat,        // BC6H       → HDR纹理压缩

    BC7Unorm,          // BC7        → 高质量RGBA压缩（UI、角色贴图）
    BC7SRGB,           // BC7 sRGB


    // --------------------------
    // ASTC （现代移动/iOS 最优压缩格式）
    // --------------------------
    ASTC4x4Unorm,      // 4x4块 → 高质量
    ASTC4x4SRGB,

    ASTC6x6Unorm,      // 6x6块 → 质量/体积平衡
    ASTC6x6SRGB,

    ASTC8x8Unorm,      // 8x8块 → 高压缩比
    ASTC8x8SRGB,


    // --------------------------
    // ETC2 / EAC （安卓 Vulkan 通用兼容压缩）
    // --------------------------
    ETC2RGB8Unorm,     // RGB 无Alpha
    ETC2RGB8SRGB,

    ETC2RGBA8Unorm,    // RGBA 带Alpha
    ETC2RGBA8SRGB,

    ETC2EACR11Unorm,   // 单通道压缩（粗糙度/AO）
    ETC2EACR11Snorm,

    ETC2EACRG11Unorm,  // 双通道压缩（法线贴图）
    ETC2EACRG11Snorm,
};

// 只支持32位索引，面向未来的设计，16位索引不够用了
enum class RHIIndexType
{
    Uint16,
    Uint32
};

inline RHIBufferUsageFlag operator|(RHIBufferUsageFlag left, RHIBufferUsageFlag right)
{
    return static_cast<RHIBufferUsageFlag>(static_cast<uint32>(left) | static_cast<uint32>(right));
}

inline ConstantRangeFlag operator|(ConstantRangeFlag left, ConstantRangeFlag right)
{
    return static_cast<ConstantRangeFlag>(static_cast<uint32>(left) | static_cast<uint32>(right));
}
