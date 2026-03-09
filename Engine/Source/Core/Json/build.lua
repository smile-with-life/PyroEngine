function config()
    local config = {
        headers = {
            "Source/Core/Json/**.h"
        },
        files = {
            "Source/Core/Json/**.cpp"
        }
    }
    return config
end