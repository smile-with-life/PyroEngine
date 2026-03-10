function config()
    local projectdir = os.projectdir()
    local config = {
        headers = {
            "Source/Runtime/Window/*.h"
        },
        files ={
            "Source/Runtime/Window/*.cpp"
        }
    }
    if is_plat("windows") then
        table.insert(config.headers, "Source/Runtime/Window/Windows/**.h")
        table.insert(config.files, "Source/Runtime/Window/Windows/**.cpp")
    end
    return config
end



