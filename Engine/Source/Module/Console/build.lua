function config()
    local projectdir = os.projectdir()
    local config = {
        headers = {
            "Source/Module/Console/*.h"
        },
        files ={
            "Source/Module/Console/*.cpp"
        }
    }
    if is_plat("windows") then
        table.insert(config.headers, "Source/Module/Console/Windows/**.h")
        table.insert(config.files, "Source/Module/Console/Windows/**.cpp")
    end
    return config
end



