#pragma once
#include "Core.h"
#include <map>

template<class KeyType, class ValueType>
class Map
{
public:
    using iterator = std::map<KeyType, ValueType>::iterator;
    using const_iterator = std::map<KeyType, ValueType>::const_iterator;
    using reverse_iterator = std::map<KeyType, ValueType>::reverse_iterator;
    using const_reverse_iterator = std::map<KeyType, ValueType>::const_reverse_iterator;
    using element_type = std::pair<KeyType, ValueType>;
public:
    Map() = default;

    ~Map() = default;

    Map(const Map& other)
        : m_data(other.m_data)
    {

    }

    Map& operator=(const Map& other)
    {
        if (this == &other) [[unlikely]]
        {
            return *this;
        }

        m_data = other.m_data;
    }

    Map(Map&& other) noexcept
        : m_data(std::move(other.m_data))
    {

    }

    Map& operator=(Map&& other) noexcept
    {
        if (this == &other) [[unlikely]]
        {
            return *this;
        }

        m_data = std::move(other.m_data);
    }

    template<class InputIt>
    constexpr Map(InputIt first, InputIt last)
        : m_data(first, last)
    {

    }

    constexpr Map(std::initializer_list<element_type> ilist)
        : m_data(ilist)
    {

    }

    constexpr Map& operator=(std::initializer_list<element_type> ilist)
    {
        m_data = ilist;
    }

    constexpr ValueType& At(const KeyType& key)
    {
        return m_data.at(key);
    }

    constexpr const ValueType& At(const KeyType& key) const
    {
        return m_data.at(key);
    }

    /*template<class Key>
    constexpr ValueType& At(const Key& key)
    {
        return m_data.at(key);
    }

    template<class Key>
    constexpr const ValueType& At(const Key& key) const
    {
        return m_data.at(key);
    }*/

    constexpr ValueType& operator[](const KeyType& key)
    {
        return m_data[key];
    }

    /*constexpr const ValueType& operator[](const KeyType& key) const
    {
        return m_data.at(key);
    }*/

    constexpr ValueType& operator[](KeyType&& key)
    {
        return m_data[key];
    }

    /*template<class Key>
    constexpr ValueType& operator[](Key&& key)
    {
        return m_data[key];
    }*/

    bool Contains(const KeyType& key) const
    {
        return m_data.contains(key);
    }

    constexpr bool IsEmpty() const
    {
        return m_data.empty();
    }

    constexpr int64 Size() const
    {
        return m_data.size();
    }

    constexpr int64 Max() const
    {
        return m_data.max_size();
    }

    constexpr void Clear()
    {
        m_data.clear();
    }

    constexpr iterator Insert(const element_type& value)
    {
        return m_data.insert(value).first;
    }

    constexpr iterator Insert(element_type&& value)
    {
        return m_data.insert(value).first;
    }

    template<class InputIt>
    constexpr void Insert(InputIt first, InputIt last)
    {
        return m_data.insert(first, last);
    }

    constexpr void Insert(std::initializer_list<element_type> ilist)
    {
        return m_data.insert(ilist);
    }

    constexpr iterator Insert(const_iterator iter, const element_type& value)
    {
        return m_data.insert(iter, value);
    }

    constexpr iterator Insert(const_iterator iter, element_type&& value)
    {
        return m_data.insert(iter, value);
    }

    template<class... Args>
    constexpr iterator Emplace(Args&&... args)
    {
        return m_data.try_emplace(std::forward<Args>(args)...).first;
    }

    constexpr int64 Erase(const KeyType& key)
    {
        return m_data.erase(key);
    }

    constexpr iterator Erase(const_iterator iter)
    {
        return m_data.erase(iter);
    }

    constexpr iterator Erase(const_iterator firstIter, const_iterator lastIter)
    {
        return m_data.erase(firstIter, lastIter);
    }

    constexpr void Merge(Map& other)
    {
        m_data.merge(other.m_data);
    }

    constexpr void Merge(Map&& other)
    {
        m_data.merge(std::forward<Map>(other.m_data));
    }

    constexpr void Swap(Map& other) noexcept
    {
        m_data.swap(other.m_data);
    }
public:
    template<Concept::EqualComparableType KeyType, Concept::EqualComparableType ValueType>
    bool operator==(const Map<KeyType, ValueType>& other)
    {
        return m_data == other.m_data;
    }

    template<Concept::EqualComparableType KeyType, Concept::EqualComparableType ValueType>
    bool operator!=(const Map<KeyType, ValueType>& other)
    {
        return m_data != other.m_data;
    }

    template<Concept::SortComparableType KeyType>
    bool operator>(const Map<KeyType, ValueType>& other)
    {
        return m_data > other.m_data;
    }

    template<Concept::SortComparableType KeyType>
    bool operator>=(const Map<KeyType, ValueType>& other)
    {
        return m_data >= other.m_data;
    }

    template<Concept::SortComparableType KeyType>
    bool operator<(const Map<KeyType, ValueType>& other)
    {
        return m_data < other.m_data;
    }

    template<Concept::SortComparableType KeyType>
    bool operator<=(const Map<KeyType, ValueType>& other)
    {
        return m_data <= other.m_data;
    }
public:
    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return m_data.begin();
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return m_data.begin();
    }

    [[nodiscard]] constexpr iterator end() noexcept
    {
        return m_data.end();
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return m_data.end();
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return m_data.cend();
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
    {
        return m_data.rbegin();
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
    {
        return m_data.rbegin();
    }

    [[nodiscard]] constexpr reverse_iterator rend() noexcept
    {
        return m_data.rend();
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
    {
        return m_data.rend();
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return m_data.crbegin();
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return m_data.crend();
    }
private:
    std::map<KeyType, ValueType> m_data;
};