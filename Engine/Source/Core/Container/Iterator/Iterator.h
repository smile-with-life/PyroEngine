#pragma once

#include <iterator>

/// <summary>
/// 输出迭代器
/// </summary>
using OutputIteratorTag = std::output_iterator_tag;

/// <summary>
/// 输入迭代器
/// </summary>
using InputIteratorTag = std::input_iterator_tag;

/// <summary>
/// 前向迭代器
/// </summary>
using ForwardIteratorTag = std::forward_iterator_tag;

/// <summary>
/// 双向迭代器
/// </summary>
using BidirectionalIteratorTag = std::bidirectional_iterator_tag;

/// <summary>
/// 随机迭代器
/// </summary>
using RandomAccessIteratorTag = std::random_access_iterator_tag;

/// <summary>
/// 连续迭代器
/// </summary>
using ContiguousIteratorTag = std::contiguous_iterator_tag;


