project "TestGame"
    kind "windowedapp"
    language "C++"
    cppdialect "C++23"

    -- 变量定义
    local outputDir = "Build/" .. SystemName[_TARGET_OS] .. "/%{cfg.buildcfg}"
    
    targetdir (outputDir .. "/Bin")
    objdir (outputDir .. "/Obj")
    
    files
    {
        "Source/game.cpp"
    }
    
    includedirs
    {
        "Source",
        EngineSourcePath,
    }

    libdirs 
    {
        EngineLibPath
    }
    
    links
    {
        "Engine",
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
        staticruntime "On"
        systemversion "latest"
            
        defines
        {
            "PLATFORM_WINDOWS"
        }

        postbuildcommands --编译完成后执行
		{
			
		}
        prebuildcommands {
            [[set PATH=%PATH%;]] .. EngineLibPath
            -- 或 PowerShell 命令（更灵活）
            -- [[powershell -ExecutionPolicy Bypass -Command "$env:Path += ']] .. EngineLibPath .. [[/Engine.dll'"]]
        }