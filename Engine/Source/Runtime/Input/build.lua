function config()
    local projectdir = os.projectdir()
    local config = {
        headers = {
            "Source/Runtime/Input/*.h"
        },
        files ={
            "Source/Runtime/Input/*.cpp"
        }
    }
    if is_plat("windows") then
        table.insert(config.headers, "Source/Runtime/Input/Windows/**.h")
        table.insert(config.files, "Source/Runtime/Input/Windows/**.cpp")
    end
    return config
end



