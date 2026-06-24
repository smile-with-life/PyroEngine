function config()
    local config = {
        headers = {
            "Source/Core/Version/**.h"
        },
        files = {
            "Source/Core/Version/**.cpp"
        }
    }
    return config
end