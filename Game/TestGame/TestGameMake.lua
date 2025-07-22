project "TestGame"
    kind "windowedapp"
    language "C++"
    cppdialect "C++23"

    outputDir = "Build/" .. SystemName[_TARGET_OS] .. "/%{cfg.buildcfg}"
    
    targetdir (outputDir .. "/Bin")
    objdir (outputDir .. "/Obj")
    
    files
    {
        "game.cpp"
    }
    
    includedirs
    {
        "Source",
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
            "PYRO_PLATFORM_WINDOWS",
            "PYRO_BUILD_DLL"
        }

        postbuildcommands --编译完成后执行
		{
			
		}