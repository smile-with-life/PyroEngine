function config()
    local config = {
        headers = {
            "Source/Platform/*.h"
        },
        files = {
            "Source/Platform/*.cpp"
        }
    }
    if is_plat("windows") then
        table.insert(config.headers, "Source/Platform/Windows/**.h")
        table.insert(config.files, "Source/Platform/Windows/**.cpp")
    end
    return config
end