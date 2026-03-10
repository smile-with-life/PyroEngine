function config()
    local projectdir = os.projectdir()
    local config = {
        headers = {
            "Source/Runtime/Console/*.h"
        },
        files ={
            "Source/Runtime/Console/*.cpp"
        }
    }
    if is_plat("windows") then
        table.insert(config.headers, "Source/Runtime/Console/Windows/**.h")
        table.insert(config.files, "Source/Runtime/Console/Windows/**.cpp")
    end
    return config
end



