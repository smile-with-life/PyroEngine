project "Engine" --项目
    location "Engine" --项目文件的目录
    kind "SharedLib" --生成类型
    language "C++" --语言
    cppdialect "C++23" --C++标准
    staticruntime "on" --是否将运行时库静态链接运行时库（dll属性的文件需要关闭）

    targetdir (outputDir .. "/Bin") --输出目录
    objdir (outputDir .. "/Obj}") --中间目录

    pchheader "pch.h" --预编译头文件
    pchsource "pch.cpp" --VS下的预编译文件-

    files --将文件添加到项目中
    {
        --"%{prj.name}/Source/starpch.h",
        --"%{prj.name}/Source/starpch.cpp",
    }

    includedirs --指定文件搜索路径
    {
        --"%{prj.name}/Source",
    }

    libdirs --库搜索路径
    {

    }

    links --链接库
    {

    }

    filter "configurations:Test" --Test模式
	    defines "PYRO_TEST"
		buildoptions "/MDd"
		symbols "On"

    filter "configurations:Debug" --Debug模式
	    defines "PYRO_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release" --Release模式
	    defines "PYRO_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist" --Dist模式
	    defines "PYRO_DIST"
		buildoptions "/MD"
		optimize "On"

    filter "system:Windows" --当系统是Windows时
		staticruntime "On"
		systemversion "latest"

		defines --预定义宏
		{
			"PYRO_PLATFORM_WINDOWS",
			"PYRO_BUILD_DLL"
		}

		postbuildcommands --编译完成后执行
		{
			-- ("{MKDIR} %[bin/" .. outputDir .. "/TestGame]"),
			-- ("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/TestGame")
		}
