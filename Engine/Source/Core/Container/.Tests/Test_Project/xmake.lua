-- 集成Google Test依赖
add_requires("GoogleTest", {configs = {shared = false}})

-- 定义测试目标
target("TestArray")
    set_kind("binary")        -- 生成可执行文件
    set_languages("c++11")    -- 使用C++11标准
    add_files("Test_Project/Test_Array.cpp")  -- 测试代码
    add_files("Test_Project/Test.cpp")         -- 包含main函数的入口文件
    add_includedirs("Test_Project")  -- 头文件搜索路径
    add_packages("GoogleTest")            -- 链接Google Test库
    add_rules("mode.Test", "mode.Debug", "mode.Release", "mode.Dist")

    -- 构建后自动执行测试
    after_build(function (target)
        os.exec("$(BUILD_PATH)/TestArray")
    end)