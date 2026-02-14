function config()
    local config = {
        headers = {
            "Source/Launch/*.h"
        },
        files = {
            "Source/Launch/*.cpp"
        }
    }
    if is_plat("windows") then
        table.insert(config.headers, "Source/Launch/Windows/**.h")
        table.insert(config.files, "Source/Launch/Windows/WindowsExport.def")
        table.insert(config.files, "Source/Launch/Windows/**.cpp")
    end
    return config
end