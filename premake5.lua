workspace "PyroEngine" --工作区
    system "Windows"
	architecture "x86_64" --架构
    characterset "Unicode" --字符集
    startproject "TestGame" --[[启动项目]]

	configurations --配置项
    {    
        "Test",                        
        "Debug",
        "Release",
        "Dist"
    }

    flags  --设置编译器选项
	{
		"MultiProcessorCompile" --多处理器并行编译
	}


outputDir = "%{prj.name}/Build/%{cfg.system}/{cfg.buildcfg}" --输出目录

project "Engine" --项目
    location "Engine" --项目文件的目录
    kind "SharedLib" --生成类型
    language "C++" --语言
    cppdialect "C++20" --C++标准
    staticruntime "on" --是否将运行时库静态链接运行时库（dll属性的文件需要关闭）

    targetdir (outputDir .. "/Bin") --输出目录
    objdir (outputDir .. "/Obj}") --中间目录

    --pchheader "pch.h" --预编译头文件
    --pchsource "pch.cpp" --VS下的预编译文件-

    files
    {
        --"%{prj.name}/Source/starpch.h",
        --"%{prj.name}/Source/starpch.cpp",
        --"%{prj.name}/Source/Starengine.h"
    }

    includedirs --包含目录
    {
        --"%{prj.name}/Source",
        --"%{IncludeDir.GLFW}",
        --"%{IncludeDir.Glad}",
        --"%{IncludeDir.ICU4C}",
        --"%{IncludeDir.Stb}",
        --"%{IncludeDir.NlohmannJson}"
    }

    libdirs 
    {
        --"%{prj.name}/Dependencies/ICU4C/lib64"
    }

    links --链接库
    {
        --"GLFW",
        --"Glad",
        --"opengl32.lib",
        --"icuuc",
        --"Stb"
    }

    filter "configurations:Test" --Test模式
	    defines "STAR_TEST"
		buildoptions "/MDd"
		symbols "On"

    filter "configurations:Debug" --Debug模式
	    defines "STAR_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release" --Release模式
	    defines "STAR_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist" --Dist模式
	    defines "STAR_DIST"
		buildoptions "/MD"
		optimize "On"

    filter "system:Windows" --当系统是Windows时
		cppdialect "C++20"
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
    
project "TestGame"
    location "Game/TestGame"
    kind "windowedapp"
    language "C++"
    
    targetdir (outputDir .. "/Bin")
    objdir (outputDir .. "/Obj")

    debugdir "." -- 工作目录
    
    files
    {
        -- "%{prj.name}/Source/**.h",
        -- "%{prj.name}/Source/**.cpp"
    }
    
    includedirs
    {
        -- "StarEngine/Source",
        -- "."
    }

    libdirs 
    {
        -- "%{prj.name}/Dependencies/ICU4C/lib64"
    }
    
    links
    {
        "StarEngine",
        -- "icuuc"
    }

    filter "configurations:Test" --Test模式
	    defines "STAR_Test"
		buildoptions "/MDd"
		symbols "On"

    filter "configurations:Debug" --Debug模式
	    defines "STAR_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release" --Release模式
	    defines "STAR_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist" --Dist模式
	    defines "STAR_DIST"
		buildoptions "/MD"
		optimize "On"
    
    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
            
        defines
        {
            "PYRO_PLATFORM_WINDOWS",
            "PYRO_BUILD_DLL"
        }
    
    