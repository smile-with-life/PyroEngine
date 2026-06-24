function config()
    local config = {
        headers = {
            "Source/Runtime/RHI/*.h",
            "Source/Runtime/RHI/SpirvReflect/**.h",
            "Source/Runtime/RHI/VulkanRHI/**.h"
            
        },
        files = {
            "Source/Runtime/RHI/*.cpp",
            "Source/Runtime/RHI/SpirvReflect/**.cpp",
            "Source/Runtime/RHI/VulkanRHI/**.cpp"
        }
    }
    if is_plat("windows") then
        table.insert(config.headers, "Source/Runtime/RHI/Platform/WindowsRHI.h")
        table.insert(config.files, "Source/Runtime/RHI/Platform/WindowsRHI.cpp")
    end
    return config
end