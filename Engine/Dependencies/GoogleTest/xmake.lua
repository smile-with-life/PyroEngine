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
            