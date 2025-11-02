#include "gtest/gtest.h"

#include "Container/Array.h"

#include "Test_Struct/_Struct_Simple.h"

namespace UnitTest 
{
// 构造函数测试
TEST(Array_Test, 默认构造函数) 
{
    /* int32 */ 
    Array<int32> int_Array;

    EXPECT_TRUE(int_Array.IsEmpty());
    EXPECT_EQ(int_Array.Size(), 0);
    EXPECT_EQ(int_Array.Capacity(), 0);
    EXPECT_EQ(int_Array.Data(), nullptr);

    /* std::string */
    Array<std::string> string_Array;
    EXPECT_TRUE(string_Array.IsEmpty());
    EXPECT_EQ(string_Array.Size(), 0);
    EXPECT_EQ(string_Array.Capacity(), 0);
    EXPECT_EQ(string_Array.Data(), nullptr);

    /* _Test_Simple */
    Array<_Test_Simple> simple_Array;
    EXPECT_TRUE(simple_Array.IsEmpty());
    EXPECT_EQ(simple_Array.Size(), 0);
    EXPECT_EQ(simple_Array.Capacity(), 0);
    EXPECT_EQ(simple_Array.Data(), nullptr);
}
// 拷贝构造函数测试
TEST(Array_Test, 拷贝构造函数)
{
    /* int32 */
    Array<int32> int_Array = { 1, 2, 3 };
    Array<int32> int_Array_copy(int_Array);

    EXPECT_EQ(int_Array.Size(), int_Array_copy.Size());
    for (int i = 0; i < int_Array.Size(); ++i) {
        EXPECT_EQ(int_Array[i], int_Array_copy[i]);
    }

    int_Array_copy[0] = 100;
    EXPECT_EQ(int_Array[0], 1);
    EXPECT_EQ(int_Array_copy[0], 100);

    /* std::string */
    Array<std::string> string_Array = { "test1", "test2", "test3" };
    Array<std::string> string_Array_copy(string_Array);
    EXPECT_EQ(string_Array.Size(), string_Array_copy.Size());
    for (int i = 0; i < string_Array.Size(); ++i) {
        EXPECT_EQ(string_Array[i], string_Array_copy[i]);
    }

    string_Array_copy[0] = "";
    EXPECT_EQ(string_Array[0], "test1");
    EXPECT_EQ(string_Array_copy[0], "");

    Array<_Test_Simple> simple_Array = { {1,"test1"},{2,"test2"},{3,"test3"} };
    Array<_Test_Simple> simple_Array_copy(simple_Array);

    EXPECT_EQ(simple_Array.Size(), simple_Array_copy.Size());
    for (int i = 0; i < simple_Array.Size(); ++i) {
        EXPECT_EQ(simple_Array[i], simple_Array_copy[i]);
    }

    simple_Array_copy[0] = { 0, "" };
    EXPECT_EQ(simple_Array[0], "test1");
    EXPECT_EQ(simple_Array_copy[0], _Test_Simple(0, ""));

}
// 拷贝赋值运算符测试
TEST(Array_Test, 拷贝赋值运算符)
{
    /* int32 */
    Array<int32> int_Array1 = { 1, 2, 3 };
    Array<int32> int_Array2;
    int_Array2 = int_Array1;

    EXPECT_EQ(int_Array1.Size(), int_Array2.Size());
    for (int i = 0; i < int_Array1.Size(); ++i) {
        EXPECT_EQ(int_Array1[i], int_Array2[i]);
    }

    /* 自赋值测试 */
    int_Array1 = int_Array1;
    EXPECT_EQ(int_Array1.Size(), 3);
    EXPECT_EQ(int_Array1[0], 1);
}
// 移动构造函数测试
TEST(Array_Test, 移动构造函数)
{
    /* int32 */
    Array<int32> int_Array = { 1, 2, 3 };
    Array<int32> int_Array_moved(std::move(int_Array));

    EXPECT_EQ(int_Array_moved.Size(), 3);
    EXPECT_TRUE(int_Array.IsEmpty());
    EXPECT_EQ(int_Array.Capacity(), 0);
    EXPECT_EQ(int_Array.Data(), nullptr);

    /* std::string */
    Array<std::string> string_Array = { "test1", "test2", "test3" };
    Array<std::string> string_Array_moved(std::move(string_Array));

    EXPECT_EQ(string_Array_moved.Size(), 3);
    EXPECT_TRUE(string_Array.IsEmpty());

    /* _Test_Simple */
    Array<_Test_Simple> simple_Array = { {1,"test1"},{2,"test2"},{3,"test3"} };
    Array<_Test_Simple> simple_Array_moved(std::move(simple_Array));

    EXPECT_EQ(simple_Array_moved.Size(), 3);
    EXPECT_TRUE(simple_Array.IsEmpty());
}
// 移动赋值运算符测试
TEST(Array_Test, 移动赋值运算符)
{
    /* int32 */
    Array<int32> int_Array1 = { 1, 2, 3 };
    Array<int32> int_Array2;
    int_Array2 = std::move(int_Array1);

    EXPECT_EQ(int_Array2.Size(), 3);
    EXPECT_TRUE(int_Array1.IsEmpty());
}
// 初始化列表构造函数测试
TEST(Array_Test, 初始化列表构造函数)
{
    /* int32 */
    Array<int32> int_Array = { 1, 2, 3, 4, 5 };
    EXPECT_EQ(int_Array.Size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(int_Array[i], i + 1);
    }

    /* std::string */
    Array<std::string> string_Array = { "a", "bb", "ccc" };
    EXPECT_EQ(string_Array.Size(), 3);
    EXPECT_EQ(string_Array[0], "a");
    EXPECT_EQ(string_Array[1], "bb");
    EXPECT_EQ(string_Array[2], "ccc");

    /* _Test_Simple */
    Array<_Test_Simple> simple_Array = { {1,"a"}, {2,"bb"}, {3,"ccc"} };
    EXPECT_EQ(simple_Array.Size(), 3);
    EXPECT_EQ(simple_Array[0].id, 1);
    EXPECT_EQ(simple_Array[1].id, 2);
    EXPECT_EQ(simple_Array[2].id, 3);
}
// 构造函数，创建指定大小的数组
TEST(Array_Test, 构造函数，创建指定大小的数组)
{
    /* int32 */
    Array<int32> int_Array(5);
    EXPECT_EQ(int_Array.Size(), 5);
    EXPECT_GE(int_Array.Capacity(), 5);

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(int_Array[i], 0);
    }

    /* std::string */
    Array<std::string> string_Array(3);
    EXPECT_EQ(string_Array.Size(), 3);
    for (int i = 0; i < 3; ++i) {
        EXPECT_TRUE(string_Array[i].empty());
    }

    /* _Test_Simple */
    Array<_Test_Simple> simple_Array(2);
    EXPECT_EQ(simple_Array.Size(), 2);
    for (int i = 0; i < 2; ++i) {
        EXPECT_EQ(simple_Array[i].id, 0);
        EXPECT_EQ(simple_Array[i].name, "default");
    }
}
// 元素访问测试
TEST(Array_Test, 元素访问操作符)
{
    Array<int32> int_Array = { 10, 20, 30 };

    EXPECT_EQ(int_Array[0], 10);
    EXPECT_EQ(int_Array[1], 20);
    EXPECT_EQ(int_Array[2], 30);

    int_Array[1] = 200;
    EXPECT_EQ(int_Array[1], 200);
}

TEST(Array_Test, At方法边界检查)
{
    Array<int32> int_Array = { 1, 2, 3 };

    EXPECT_EQ(int_Array.At(0), 1);
    EXPECT_EQ(int_Array.At(1), 2);
    EXPECT_EQ(int_Array.At(2), 3);

    EXPECT_THROW(int_Array.At(3), std::out_of_range);
    EXPECT_THROW(int_Array.At(-1), std::out_of_range);
}

TEST(Array_Test, 首尾元素访问)
{
    Array<int32> int_Array = { 1, 2, 3 };

    EXPECT_EQ(int_Array.Front(), 1);
    EXPECT_EQ(int_Array.Back(), 3);

    Array<int32> empty_Array;
    EXPECT_THROW(empty_Array.Front(), std::out_of_range);
    EXPECT_THROW(empty_Array.Back(), std::out_of_range);
}

// 容量管理测试
TEST(Array_Test, 容量预留)
{
    Array<int32> int_Array;

    int_Array.Reserve(10);
    EXPECT_GE(int_Array.Capacity(), 10);
    EXPECT_EQ(int_Array.Size(), 0);

    for (int i = 0; i < 20; ++i) {
        int_Array.Add(i);
    }
    EXPECT_GE(int_Array.Capacity(), 20);
}

TEST(Array_Test, 调整大小)
{
    Array<int32> int_Array = { 1, 2, 3 };

    int_Array.Resize(2);
    EXPECT_EQ(int_Array.Size(), 2);
    EXPECT_EQ(int_Array[0], 1);
    EXPECT_EQ(int_Array[1], 2);

    int_Array.Resize(5);
    EXPECT_EQ(int_Array.Size(), 5);
    EXPECT_EQ(int_Array[0], 1);
    EXPECT_EQ(int_Array[1], 2);
    EXPECT_EQ(int_Array[2], 0);
    EXPECT_EQ(int_Array[3], 0);
    EXPECT_EQ(int_Array[4], 0);
}

TEST(Array_Test, 内存收缩)
{
    Array<int32> int_Array;

    int_Array.Reserve(100);
    EXPECT_GE(int_Array.Capacity(), 100);

    for (int i = 0; i < 10; ++i) {
        int_Array.Add(i);
    }

    int_Array.Shrink();
    EXPECT_EQ(int_Array.Capacity(), int_Array.Size());
}

// 添加元素测试
TEST(Array_Test, 添加元素)
{
    Array<int32> int_Array;

    int value = 42;
    int_Array.Add(value);
    EXPECT_EQ(int_Array.Size(), 1);
    EXPECT_EQ(int_Array[0], 42);

    int_Array.Add(100);
    EXPECT_EQ(int_Array.Size(), 2);
    EXPECT_EQ(int_Array[1], 100);

    for (int i = 0; i < 10; ++i) {
        int_Array.Add(i);
    }
    EXPECT_EQ(int_Array.Size(), 12);
}

TEST(Array_Test, 追加数组)
{
    Array<int32> int_Array1 = { 1, 2, 3 };
    Array<int32> int_Array2 = { 4, 5, 6 };

    int_Array1.Append(int_Array2);
    EXPECT_EQ(int_Array1.Size(), 6);

    for (int i = 0; i < 6; ++i) {
        EXPECT_EQ(int_Array1[i], i + 1);
    }
}

TEST(Array_Test, 就地构造)
{
    Array<_Test_Simple> simple_Array;

    auto iter = simple_Array.Emplace(1, "test");
    EXPECT_EQ(simple_Array.Size(), 1);
    EXPECT_EQ(simple_Array[0].id, 1);
    EXPECT_EQ(simple_Array[0].name, "test");
    EXPECT_EQ(iter, simple_Array.begin());
}

// 删除元素测试
TEST(Array_Test, 删除元素)
{
    Array<int32> int_Array = { 1, 2, 3, 4, 5 };

    int_Array.Erase(2);
    EXPECT_EQ(int_Array.Size(), 4);
    EXPECT_EQ(int_Array[0], 1);
    EXPECT_EQ(int_Array[1], 2);
    EXPECT_EQ(int_Array[2], 4);
    EXPECT_EQ(int_Array[3], 5);

    int_Array.Erase(1, 3);
    EXPECT_EQ(int_Array.Size(), 2);
    EXPECT_EQ(int_Array[0], 1);
    EXPECT_EQ(int_Array[1], 5);
}

TEST(Array_Test, 弹出操作)
{
    Array<int32> int_Array = { 1, 2, 3, 4, 5 };

    int_Array.Pop();
    EXPECT_EQ(int_Array.Size(), 4);
    EXPECT_EQ(int_Array.Back(), 4);

    int_Array.Shift();
    EXPECT_EQ(int_Array.Size(), 3);
    EXPECT_EQ(int_Array.Front(), 2);
}

// 查找操作测试
TEST(Array_Test, 包含检查)
{
    Array<int32> int_Array = { 1, 2, 3, 4, 5, 3 };

    EXPECT_TRUE(int_Array.Contains(3));
    EXPECT_FALSE(int_Array.Contains(6));

    EXPECT_EQ(int_Array.IndexOf(3), 2);
    EXPECT_EQ(int_Array.LastIndexOf(3), 5);
    EXPECT_EQ(int_Array.IndexOf(6), -1);
}

TEST(Array_Test, 条件查找)
{
    Array<int32> int_Array = { 1, 2, 3, 4, 5 };

    auto iter = int_Array.Find([](int x) { return x > 3; });
    EXPECT_NE(iter, int_Array.end());
    EXPECT_EQ(*iter, 4);

    int32 index = int_Array.FindIndex([](int x) { return x % 2 == 0; });
    EXPECT_EQ(index, 1);
}

// 功能操作测试
TEST(Array_Test, 过滤数组)
{
    Array<int32> int_Array = { 1, 2, 3, 4, 5, 6 };

    Array<int32> filtered = int_Array.Filter([](int x) { return x % 2 == 0; });
    EXPECT_EQ(filtered.Size(), 3);
    EXPECT_EQ(filtered[0], 2);
    EXPECT_EQ(filtered[1], 4);
    EXPECT_EQ(filtered[2], 6);

    EXPECT_EQ(int_Array.Size(), 6);
}

TEST(Array_Test, 排序操作)
{
    Array<int32> int_Array = { 5, 3, 1, 4, 2 };

    int_Array.Sort([](int a, int b) { return a < b; });

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(int_Array[i], i + 1);
    }
}

TEST(Array_Test, 交换操作)
{
    Array<int32> int_Array1 = { 1, 2, 3 };
    Array<int32> int_Array2 = { 4, 5, 6, 7 };

    int_Array1.Swap(int_Array2);

    EXPECT_EQ(int_Array1.Size(), 4);
    EXPECT_EQ(int_Array2.Size(), 3);
    EXPECT_EQ(int_Array1[0], 4);
    EXPECT_EQ(int_Array2[0], 1);
}

// 迭代器测试
TEST(Array_Test, 迭代器遍历)
{
    Array<int32> int_Array = { 1, 2, 3, 4, 5 };

    int sum = 0;
    for (auto it = int_Array.begin(); it != int_Array.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);

    sum = 0;
    for (int val : int_Array) {
        sum += val;
    }
    EXPECT_EQ(sum, 15);
}

// 运算符测试
TEST(Array_Test, 比较运算符)
{
    Array<int32> int_Array1 = { 1, 2, 3 };
    Array<int32> int_Array2 = { 1, 2, 3 };
    Array<int32> int_Array3 = { 4, 5, 6 };

    EXPECT_TRUE(int_Array1 == int_Array2);
    EXPECT_FALSE(int_Array1 == int_Array3);
    EXPECT_FALSE(int_Array1 != int_Array2);
    EXPECT_TRUE(int_Array1 != int_Array3);
}

TEST(Array_Test, 连接运算符)
{
    Array<int32> int_Array1 = { 1, 2, 3 };
    Array<int32> int_Array2 = { 4, 5, 6 };

    Array<int32> result = int_Array1 + int_Array2;
    EXPECT_EQ(result.Size(), 6);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[3], 4);

    int_Array1 += int_Array2;
    EXPECT_EQ(int_Array1.Size(), 6);
}

// 边界条件测试
TEST(Array_Test, 边界条件)
{
    Array<int32> empty_Array;
    EXPECT_NO_THROW(empty_Array.Clear());
    EXPECT_NO_THROW(empty_Array.Pop());
    EXPECT_NO_THROW(empty_Array.Shift());

    Array<int32> single_Array = { 42 };
    EXPECT_EQ(single_Array.Front(), single_Array.Back());
    single_Array.Pop();
    EXPECT_TRUE(single_Array.IsEmpty());
}
} // namespace UnitTest 
