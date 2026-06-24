#pragma once

#include "Core.h"
#include <random>
#include <type_traits>
#include <algorithm>

/// <summary>
/// 随机数生成器
/// </summary>
/// <typeparam name="Engine">随机数引擎类型，默认使用std::mt19937</typeparam>
template<class Engine = std::mt19937>
class Random
{
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    Random() noexcept
    {
        std::random_device rd;
        m_engine.seed(rd());
    }
    /// <summary>
    /// 析构函数
    /// </summary>
    ~Random() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    Random(const Random& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    Random& operator=(const Random& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    Random(Random&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    Random& operator=(Random&& other) = default;
    /// <summary>
    /// 使用指定种子构造
    /// </summary>
    /// <param name="seed">种子值</param>
    explicit Random(uint64 seed) noexcept
    {
        m_engine.seed(seed);
    }
    /// <summary>
    /// 使用种子序列构造
    /// </summary>
    /// <typeparam name="SeedSeq">种子序列类型</typeparam>
    /// <param name="seq">种子序列</param>
    template<class SeedSeq>
    explicit Random(SeedSeq&& seq) noexcept
    {
        m_engine.seed(std::forward<SeedSeq>(seq));
    }
public:
    /// <summary>
    /// 设置随机数种子
    /// </summary>
    /// <param name="seed">种子值</param>
    void SetSeed(uint32 seed) noexcept
    {
        m_engine.seed(seed);
    }
    /// <summary>
    /// 使用种子序列设置种子
    /// </summary>
    /// <typeparam name="SeedSeq">种子序列类型</typeparam>
    /// <param name="seq">种子序列</param>
    template<class SeedSeq>
    void SetSeed(SeedSeq&& seq) noexcept
    {
        m_engine.seed(std::forward<SeedSeq>(seq));
    }
    /// <summary>
    /// 生成均匀分布的随机整数
    /// </summary>
    /// <returns>随机整数</returns>
    int64 Rand();
    /// <summary>
    /// 将引擎的状态推进指定量
    /// </summary>
    /// <param name="count"></param>
    void Discard(uint64 count);
    /// <summary>
    /// 生成指定范围内的随机整数
    /// </summary>
    /// <param name="min">最小值（包含）</param>
    /// <param name="max">最大值（包含）</param>
    /// <returns>随机整数</returns>
    int64 RandRange(int64 min, int64 max);
    /// <summary>
    /// 生成指定范围内的随机浮点数
    /// </summary>
    /// <param name="min">最小值（包含）</param>
    /// <param name="max">最大值（包含）</param>
    /// <returns>随机浮点数</returns>
    double RandRange(double min, double max);
    /// <summary>
    /// 以指定概率返回 true
    /// 可用于模拟概率事件
    /// </summary>
    /// <param name="probability">返回 true 的概率，范围[0.0, 1.0]</param>
    /// <returns>以指定概率返回 true</returns>
    bool RandChance(double probability = 0.5);
    /// <summary>
    /// 生成二项分布的随机数
    /// 模拟 n 次独立伯努利试验中成功的次数
    /// </summary>
    /// <param name="trials">试验次数</param>
    /// <param name="probability">每次试验成功的概率（p）</param>
    /// <returns>成功次数，范围[0, trials]</returns>
    int64 RandBinomial(int64 trials, double probability);
    /// <summary>
    /// 生成负二项分布的随机数
    /// 模拟在第r次成功之前失败的次数
    /// </summary>
    /// <param name="trials">所需成功次数（r）</param>
    /// <param name="probability">每次试验成功的概率（p）</param>
    /// <returns>在第r次成功之前失败的次数</returns>
    int64 RandNegativeBinomial(int64 trials, double probability);
    /// <summary>
    /// 生成几何分布的随机数
    /// 模拟第一次成功之前失败的次数
    /// </summary>
    /// <param name="probability">每次试验成功的概率（p）</param>
    /// <returns>第一次成功之前失败的次数</returns>
    int64 RandGeometric(double probability);
    /// <summary>
    /// 生成泊松分布的随机数
    /// 模拟单位时间内事件发生的次数
    /// </summary>
    /// <param name="mean">均值（λ）</param>
    /// <returns>事件发生次数</returns>
    int64 RandPoisson(double mean);
    /// <summary>
    /// 生成指数分布的随机数
    /// 模拟事件发生的时间间隔
    /// </summary>
    /// <param name="lambda">速率参数（λ），默认1.0</param>
    /// <returns>指数分布的随机数</returns>
    double RandExponential(double lambda = 1.0);
    /// <summary>
    /// 生成伽马分布的随机数
    /// 多个指数分布的和
    /// </summary>
    /// <param name="alpha">形状参数（α）</param>
    /// <param name="beta">尺度参数（β），默认1.0</param>
    /// <returns>伽马分布的随机数</returns>
    double RandGamma(double alpha, double beta = 1.0);
    /// <summary>
    /// 生成威布尔分布的随机数
    /// 常用于可靠性分析和寿命测试
    /// </summary>
    /// <param name="alpha">形状参数（α）</param>
    /// <param name="beta">尺度参数（β），默认1.0</param>
    /// <returns>威布尔分布的随机数</returns>
    double RandWeibull(double alpha, double beta = 1.0);
    /// <summary>
    /// 生成极值分布的随机数
    /// 又称Gumbel分布，用于极值理论
    /// </summary>
    /// <param name="a">位置参数</param>
    /// <param name="b">尺度参数，默认1.0</param>
    /// <returns>极值分布的随机数</returns>
    double RandExtreme(double a, double b = 1.0);
    /// <summary>
    /// 生成正态分布的随机数
    /// </summary>
    /// <param name="mean">均值（μ），默认0.0</param>
    /// <param name="stddev">标准差（σ），默认1.0</param>
    /// <returns>正态分布的随机数</returns>
    double RandNormal(double mean = 0.0, double stddev = 1.0);
    /// <summary>
    /// 生成对数正态分布的随机数
    /// 取对数后服从正态分布
    /// </summary>
    /// <param name="mean">均值（μ），默认0.0</param>
    /// <param name="stddev">标准差（σ），默认1.0</param>
    /// <returns>对数正态分布的随机数</returns>
    double RandLognormal(double mean = 0.0, double stddev = 1.0);
    /// <summary>
    /// 生成卡方分布的随机数
    /// 独立标准正态随机变量的平方和
    /// </summary>
    /// <param name="n">自由度（ν）</param>
    /// <returns>卡方分布的随机数</returns>
    double RandChiSquared(double n);
    /// <summary>
    /// 生成柯西分布的随机数
    /// 无均值和方差的分布
    /// </summary>
    /// <param name="a">位置参数</param>
    /// <param name="b">尺度参数，默认1.0</param>
    /// <returns>柯西分布的随机数</returns>
    double CauchyRand(double a, double b = 1.0);
    /// <summary>
    /// 从容器中随机选择一个元素
    /// 适用于支持 begin() 和 end() 的容器
    /// </summary>
    /// <typeparam name="Container">容器类型</typeparam>
    /// <param name="container">容器</param>
    /// <returns>随机选择的元素引用</returns>
    template<class Container>
    auto& PickElement(Container& container);
    /// <summary>
    /// 从容器中随机选择一个元素（const）
    /// 适用于支持 begin() 和 end() 的容器
    /// </summary>
    /// <typeparam name="Container">容器类型</typeparam>
    /// <param name="container">容器</param>
    /// <returns>随机选择的元素引用</returns>
    template<class Container>
    const auto& PickElement(const Container& container);
    /// <summary>
    /// 随机打乱容器中的元素
    /// 使用Fisher-Yates洗牌算法
    /// </summary>
    /// <typeparam name="Container">容器类型</typeparam>
    /// <param name="container">容器</param>
    template<class Container>
    void Shuffle(Container& container);
    /// <summary>
    /// 获取底层随机数引擎
    /// </summary>
    /// <returns>随机数引擎引用</returns>
    Engine& GetNativeEngine();
    /// <summary>
    /// 获取底层随机数引擎（const版本）
    /// </summary>
    /// <returns>随机数引擎引用</returns>
    const Engine& GetNativeEngine() const;
private:
    /// <summary>
    /// 随机数引擎
    /// </summary>
    Engine m_engine;
};

