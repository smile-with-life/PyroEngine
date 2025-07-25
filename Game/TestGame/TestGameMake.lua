project "TestGame"
    kind "windowedapp"
    language "C++"
    cppdialect "C++23"

    -- 变量定义
    local outputDir = "Build/" .. SystemName[_TARGET_OS] .. "/%{cfg.buildcfg}"
    local EngineSourcePath = WorkPath .. EnginePath .. "Source"
    
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
        
    }
    
    links
    {
        "PyroEngine",
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
    
    filter "system:Windows"
        staticruntime "On"
        systemversion "latest"
            
        defines
        {
            "PLATFORM_WINDOWS"
        }

        postbuildcommands --编译完成后执行
		{
			'{MKDIR} "%{targetdir}"', -- 确保目标目录存在
            '{COPY} "' .. EnginePath .. "Build/Windwos/Debug/Bin" .. '/PyroEngine.dll" "%{targetdir}/PyroEngine.dll"'
		}