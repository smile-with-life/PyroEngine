-- 设置工程名
set_project "PyroEngine"
-- 设置工程版本
set_version "0.1.0"
-- 设置编码
set_encodings "utf-8"
-- 设置最小 xmake 版本为：3.0.0
set_xmakever "3.0.0"
-- 设置允许编译的平台列表
set_allowedplats("windows", "linux", "android", "macosx", "ios")
-- 设置允许编译的平台架构
set_allowedarchs("x64")
-- 设置代码语言标准：最新
set_languages "cxxlatest"

-- 包含引擎工程描述文件
includes "Engine/xmake.lua"
includes "Sandbox/xmake.lua"