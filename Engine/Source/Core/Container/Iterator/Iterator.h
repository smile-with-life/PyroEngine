#pragma once

//输出迭代器
struct OutputIteratorTag {};
//输入迭代器
struct InputIteratorTag {};
//前向迭代器
struct ForwardIteratorTag : InputIteratorTag {};
//双向迭代器
struct BidirectionalIteratorTag : ForwardIteratorTag {};
//随机迭代器
struct RandomAccessIteratorTag : BidirectionalIteratorTag {};
//连续迭代器
struct ContiguousIteratorTag : RandomAccessIteratorTag {};