package "GoogleTest"
    add_deps "cmake"
    set_sourcedir "GoogleTest"

    on_install(function (package)
        local configs = {
            "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"),
            "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"),
            "-Dgtest_force_shared_crt=" .. (package:config("shared") and "ON" or "OFF")  -- 强制共享CRT
        }
        import("package.tools.cmake").install(package, configs)
    end)

package_end()