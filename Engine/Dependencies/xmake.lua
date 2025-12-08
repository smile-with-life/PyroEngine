package("GoogleTest")
    -- package 项目主页
    set_homepage("https://github.com/google/googletest")
    -- package 描述
    set_description("Google Testing and Mocking Framework.")
    -- 获取资源方式：本地源码
    set_sourcedir(path.join(os.scriptdir(), "GoogleTest"))

    -- 包配置选项
    add_configs("main",  {description = "Link to the gtest_main entry point.", default = false, type = "boolean"})
    add_configs("gmock", {description = "Link to the googlemock library.", default = false, type = "boolean"})
    add_configs("cmake", {description = "Use cmake build system", default = false, type = "boolean"})

    -- 平台相关系统库链接
    if is_plat("linux", "bsd") then
        add_syslinks("pthread")  -- POSIX线程库
    end

    -- 包加载时回调
    on_load(function (package)
        -- 配置cmake构建系统时添加cmake依赖
        if package:config("cmake") then
            package:add("deps", "cmake")
        end

        -- Windows平台共享库特殊处理
        if package:config("shared") and package:is_plat("windows") then
            package:add("defines", "GTEST_LINKED_AS_SHARED_LIBRARY=1")  -- 定义共享库宏
        end
    end)

    -- 包安装逻辑
    on_install(function (package)
        -- 使用CMake构建路径
        if package:config("cmake") then
            local configs = {}
            table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:is_debug() and "Debug" or "Release"))
            table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
            table.insert(configs, "-DBUILD_GMOCK=" .. (package:config("gmock") and "ON" or "OFF"))
            import("package.tools.cmake").install(package, configs)  -- 调用CMake安装器
        
        -- 使用xmake原生构建路径
        else
            local std = "cxxlatest"
            -- 动态生成xmake构建脚本
            io.writefile("xmake.lua", format([[
                add_rules("utils.install.cmake_importfiles")  -- 导入CMake安装规则
                set_languages("cxxlatest")  -- 设置C++标准
                
                -- 定义gtest静态库目标
                target("gtest")
                    set_kind("$(kind)")  -- 动态/静态库类型
                    add_files("googletest/src/gtest-all.cc")  -- 源码文件
                    add_includedirs("googletest/include", "googletest")  -- 头文件目录
                    add_headerfiles("googletest/include/(**.h)")  -- 头文件安装
                    -- Windows共享库特殊宏定义
                    if is_kind("shared") and is_plat("windows") then
                        add_defines("GTEST_CREATE_SHARED_LIBRARY=1")
                    end

                -- 定义gtest_main目标
                target("gtest_main")
                    set_kind("$(kind)")
                    set_default(false)  -- 根据配置决定是否默认启用
                    add_files("googletest/src/gtest_main.cc")
                    add_includedirs("googletest/include", "googletest")
                    add_deps("gtest")  -- 依赖gtest目标

                -- 定义gmock目标
                target("gmock")
                    set_kind("$(kind)")
                    set_default(false)  -- 根据配置决定是否默认启用
                    add_files("googlemock/src/gmock-all.cc")
                    add_includedirs("googlemock/include", "googlemock", "googletest/include", "googletest")
                    add_headerfiles("googlemock/include/(**.h)")
                    -- Windows共享库特殊宏定义
                    if is_kind("shared") and is_plat("windows") then
                        add_defines("GTEST_CREATE_SHARED_LIBRARY=1")
                    end
                    add_deps("gtest")  -- 依赖gtest目标
            ]], std, tostring(package:config("main")), tostring(package:config("gmock"))))
            import("package.tools.xmake").install(package)  -- 调用xmake安装器
        end
    end)

    -- 包测试逻辑
    on_test(function (package)
        local std = "c++20"
        
        -- 基础功能测试
        assert(package:check_cxxsnippets({test = [[
            #include <gtest/gtest.h>
            TEST(FactorialTest, Zero) {
                EXPECT_EQ(1, 1);  // 基础断言验证
            }
        ]]}, {configs = {languages = std}}))

        -- gmock功能测试
        if package:config("gmock") then
            assert(package:check_cxxsnippets({test = [[
                #include <gmock/gmock.h>
                TEST(GMockTest, MockExample) {
                    using ::testing::AtLeast;
                    class MockClass {
                    public:
                        MOCK_METHOD0(Method, void());
                    };
                    MockClass mock;
                    EXPECT_CALL(mock, Method()).Times(AtLeast(1));
                }
            ]]}, {configs = {languages = std}}))
        end
    end)

package_end()