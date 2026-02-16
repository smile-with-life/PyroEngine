function config()
    local projectdir = os.projectdir()
    local config = {
        headers = {
            "Source/Module/Window/*.h"
        },
        files ={
            "Source/Module/Window/*.cpp"
        }
    }
    if is_plat("windows") then
        table.insert(config.headers, "Source/Module/Window/Windows/**.h")
        table.insert(config.files, "Source/Module/Window/Windows/**.cpp")
    end
    return config
end



