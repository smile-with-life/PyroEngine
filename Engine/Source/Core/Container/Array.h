#pragma once
#include "Core/Core.h"
#include "Iterator/Iterator.h"
#include "Allocator/Allocator.h"


template <class Type>
class ArrayIterator
{


};

template<class Type>
class Array 
{
public:
    using iterator = ArrayIterator<Type>;
    using const_iterator = ArrayIterator<const Type>;
public:
    constexpr Array();

    constexpr ~Array();

    constexpr Array(const Array& other);

    constexpr Array& operator=(const Array& other);

    constexpr Array(Array&& other);

    constexpr Array& operator=(Array&& other);

    constexpr explicit Array(int32 count);

    constexpr Array(int32 count, const Type& value);

    template <std::random_access_iterator InputIt>
    constexpr Array(InputIt first, InputIt last);

    constexpr Array(std::initializer_list<Type> ilist);

    constexpr Array& operator=(std::initializer_list<Type> ilist);
public:
    constexpr Type& At(int32 index);

    constexpr const Type& At(int32 index) const;

    Type& Front();

    constexpr const Type& Front() const;

    constexpr Type& Back();

    constexpr const Type& Back() const;
private:
    int32 m_size = 0;
    int32 m_capacity = 0;
    Allocator m_alloc = Allocator();
    Type* m_data = nullptr;
};
