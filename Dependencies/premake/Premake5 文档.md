# Premake5 文档

## **1.workspace 工作区**

创建新工作区。

```lua
workspace ("name")
```

工作区是 Premake 构建脚本中的顶级对象，是 Visual Studio 解决方案的同义词。每个工作区都包含一个或多个项目，这些项目又包含用于生成单个二进制目标的设置。

#### 参数

`name`是工作区的唯一名称。如果已存在具有给定名称的工作区，则会将其变为活动状态并返回。

如果未指定 name，则返回当前工作区范围，并将其设为活动状态。

如果使用 “*”，则选择适用于所有工作区的 “root” 配置范围，并返回 nil。

默认情况下，工程名称将用作生成的工程文件的文件名;小心使用空格和特殊字符。您可以使用 [filename](https://premake.github.io/docs/filename) 调用覆盖此默认值。

#### 例子

创建名为 “MyWorkspace” 的新工作区，其中包含调试和发布生成配置。

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }
```

## 2.system 操作系统

指定目标作系统。

```lua
system ("value")
```

如果未指定系统，Premake 将识别并定位当前作系统。这可以用命令行参数覆盖，提供下面的系统标识符之一。`--os`

#### 参数

`value`是以下之一：

- aix
- android
- bsd
- [haiku](http://www.haiku-os.org/)
- ios
- linux
- macosx
- solaris
- tvos
- uwp
- wii
- windows
- xbox360

#### 适用于

项目配置。

#### 例子

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }
   system { "Windows", "Unix", "Mac" }

   filter "system:Windows"
      system "windows"

   filter "system:Unix"
      system "linux"

   filter "system:Mac"
      system "macosx"
```

## 3.architecture 架构

指定配置要针对的系统体系结构。

```lua
architecture ("value")
```

#### 参数

`value`是以下之一：

- `universal`：iOS 和 macOS 支持的通用二进制文件
- `x86`
- `x86_64`
- `ARM`
- `ARM64`
- `RISCV64`
- `loongarch64`
- `ppc`
- `ppc64`
- `wasm32`,
- `wasm64`,
- `e2k`,
- `armv5`：仅在 VSAndroid 项目中受支持
- `armv7`：仅在 VSAndroid 项目中受支持
- `aarch64`：仅在 VSAndroid 项目中受支持
- `mips`：仅在 VSAndroid 项目中受支持
- `mips64`：仅在 VSAndroid 项目中受支持

作为上述别名的其他值：

- `i386`：的别名`x86`
- `amd64`：的别名`x86_64`
- `x32`：的别名;有意弃用此`x86`
- `x64`：的别名;有意弃用此`x86_64`

#### 适用于

项目配置。

#### 例子

设置 32 位和 64 位 Windows 版本。

```lua
workspace "MyWorkspace"
   configurations { "Debug32", "Release32", "Debug64", "Release64" }

   filter "configurations:*32"
      architecture "x86"

   filter "configurations:*64"
      architecture "x86_64"
```

## 4.characterset 字符集

设置字符编码。

```lua
characterset ("value")
```

#### 参数

`value`是以下之一：

- `Default`：工具集的默认编码;通常`Unicode`
- `MBCS`：多字节字符集;目前仅限 Visual Studio
- `Unicode`：Unicode 字符编码
- `ASCII`：没有实际字符集

#### 适用于

项目配置。

## 5.startproject 启动项目

指定工作区的启动项目。

```lua
startproject "name"
```

启动项目目前仅受 Visual Studio 支持。

#### 参数

`name`是启动项目的名称。这应该与定义项目的 project（） 调用中提供的名称匹配。

#### 适用于

工作区。

#### 例子

```lua
workspace "MyWorkspace"
    configurations { "Debug", "Release" }
    startproject "MyProject2"

project "MyProject1"
    -- define project 1 here

project "MyProject2"
    -- define project 2 here
```

## 6.configurations 配置项

指定工作区或项目的构建配置集，例如 “Debug” 和 “Release”。

```lua
configurations { "names" }
```



配置封装了一组构建设置，使开发人员能够在它们之间轻松切换。“Debug” 和 “Release” 是最常见的配置名称。

有关更多信息，请参阅[配置和平台](https://premake.github.io/docs/Configurations-and-Platforms)。

#### 参数

`names`是配置名称的列表。允许使用空格，但可能会使使用某些 Premake 功能（例如命令行配置选择）更加困难。

#### 适用于

工作区和项目。

#### 例子

指定工作区的调试和发布配置。

```lua
workspace "MyWorkspace"
  configurations { "Debug", "Release" }
```

为 Dynamic Link Library 版本添加其他配置。

```lua
configurations { "Debug", "Release", "DebugDLL", "ReleaseDLL" }
```

## 7.project 项目

在工作区范围内创建新项目。调用项目后，将清除所有以前的筛选器设置（即重置）。

```lua
project ("name")
```

项目包含生成单个二进制目标所需的所有设置，并且与 Visual Studio 项目同义。这些设置包括源代码文件列表、这些文件使用的编程语言、编译器标志、包含目录以及要链接到的库。

每个项目都属于一个工作区。

#### 参数

`name`是项目的名称，在包含项目的工作区中必须是唯一的。如果具有给定名称的项目已存在，则将其激活并返回。

如果未指定名称，则返回当前项目范围，并使其处于活动状态。

如果使用“*”，则适用于所有工作区的包含工作区将变为活动状态，并返回 nil。

默认情况下，工程名称将用作生成的工程文件的文件名;小心使用空格和特殊字符。您可以使用 [filename](https://premake.github.io/docs/filename) 调用覆盖此默认值。

#### 可用性

Premake 4.0 或更高版本。

#### 例子

创建一个名为“MyProject”的新项目。请注意，必须存在工作空间才能包含项目。缩进是为了可读性，是可选的。

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }

project "MyProject"
   kind "ConsoleApp"
   language "C++"
```

## 8.location 项目文件的目录

设置生成的工作区或项目文件的目标目录。

```lua
location ("path")
```

默认情况下，工作区和项目文件将生成到与定义它们的脚本相同的目录中。该功能允许您更改此位置。`location`

请注意，与其他值不同，它不会自动传播到包含的项目。除非明确覆盖，否则项目将使用其默认位置。`location`

#### 参数

`path`是应存储生成文件的目录，相对于当前正在执行的脚本文件指定。

#### 适用于

工作区和项目。

#### 例子

设置工作区的目标目录。设置项目的位置的工作方式相同。

```lua
workspace "MyWorkspace"
  location "../build"
```



如果您计划使用来自同一源树的多个工具进行构建，则可能需要按工具集拆分工程文件。[_ACTION](https://premake.github.io/docs/globals/_ACTION) 全局变量包含在命令行中指定的当前工具集标识符。请注意，在这种情况下，Lua 语法需要在函数参数周围加上括号。

```lua
location ("../build/" .. _ACTION)
```

## 9.kind 生成类型

设置由项目或配置（如控制台或窗口化应用程序，或者共享或静态库）创建的二进制对象的类型。

```lua
kind ("kind")
```

#### 参数

`kind`是以下字符串标识符之一：

| 值            | 描述                                                         |
| ------------- | ------------------------------------------------------------ |
| ConsoleApp    | 控制台或命令行应用程序。                                     |
| WindowedApp   | 在桌面窗口中运行的应用程序。这种区别不适用于 Linux，但在 Windows 和 Mac OS X 上很重要。 |
| SharedLib     | 共享库或 DLL。                                               |
| StaticLib     | 静态库。                                                     |
| Makefile 文件 | 一种特殊的配置类型，用于调用一个或多个外部命令。创建的实际二进制文件类型未指定。有关更多信息，请参阅 [Makefile 项目](https://premake.github.io/docs/Makefile-Projects)。 |
| Utility       | 仅包含自定义构建规则的配置。                                 |
| None          | 内部版本中未包含的配置。对于仅包含网页、头文件或支持文档的项目非常有用。 |
| Packaging     | 用于创建 .androidproj 文件的配置类型，这些文件在 Visual Studio 下的 Android 应用程序中构建 apk。*请注意，这以前是 `AndroidProj`。* |
| SharedItems   | 一种特殊的配置类型，它不包含自己的任何构建设置，而是使用链接它的任何项目的构建设置。 |

#### 适用于

项目配置。

#### 可用性

**Makefile** 类型在 Premake 5.0 及更高版本中可用，并且受 Visual Studio 和 Codelite 支持。 **None** 类型在 Premake 5.0 及更高版本中可用，并且支持 gmakelegacy、gmake、Codelite 和 Visual Studio。 **Utility** 类型仅适用于 Visual Studio、Codelite 和 gmake，并且在 gmakelegacy 中的支持非常有限。 **SharedItems** 类型仅适用于 Visual Studio 2013 及更高版本。

#### 例子

将项目设置为构建命令行可执行文件。

```lua
kind "ConsoleApp"
```

将项目设置为构建共享库 （DLL）。

```lua
kind "SharedLib"
```

构建静态库或共享库，具体取决于所选的构建配置。

```lua
workspace "MyWorkspace"
   configurations { "DebugLib", "DebugDLL", "ReleaseLib", "ReleaseDLL" }

project "MyProject"

   filter "*Lib"
      kind "StaticLib"

   filter "*DLL"
      kind "SharedLib"
```

## 10.language 语言

设置项目使用的编程语言。

```lua
language ("lang")
```

#### 参数

`lang`是语言标识符。某些语言需要一个模块才能获得完全支持。

| 语言  | 模块          |
| ----- | ------------- |
| `C`   | 内置;始终可用 |
| `C++` | 内置;始终可用 |
| `C#`  | 内置;始终可用 |
| `F#`  | 内置;始终可用 |

#### 适用于

项目配置。

#### 例子

将项目语言设置为 C++。

```lua
language "C++"
```

将项目语言设置为 C#

```lua
language "C#"
```

## 11.cppdialect C++标准

cpp标准

```lua
cppdialect "value"
```

#### 参数

`value`以下之一：

- `Default`：工具集的默认 C++ 方言
- `C++latest`：工具集或作的最新 C++ 方言（如果可用），否则为 Premake 支持的最新 C++ 方言
- `C++98`：ISO C++98
- `C++0x`：ISO C++11 草案
- `C++11`：ISO C++11
- `C++1y`：ISO C++14 草案
- `C++14`：ISO C++14
- `C++1z`：ISO C++17 草案
- `C++17`：ISO C++17
- `C++2a`：ISO C++20 草案
- `C++20`：ISO C++20
- `C++2b`：ISO C++23 草案
- `C++23`：ISO C++23
- `gnu++98`：ISO C++98 的 GNU 方言
- `gnu++0x`：ISO C++11 草案的 GNU 方言
- `gnu++11`：ISO C++11 的 GNU 方言
- `gnu++1y`：ISO C++14 草案的 GNU 方言
- `gnu++14`：ISO C++14 的 GNU 方言
- `gnu++1z`：ISO C++17 草案的 GNU 方言
- `gnu++17`：ISO C++17 的 GNU 方言
- `gnu++2a`：ISO C++20 草案的 GNU 方言
- `gnu++20`：ISO C++20 的 GNU 方言
- `gnu++2b`：ISO C++23 草案的 GNU 方言
- `gnu++23`：ISO C++23 的 GNU 方言

#### 适用于

范围。`config`

#### 例子

```lua
cppdialect "value"
```

## 12.targetdir 输出目录

设置已编译的二进制目标的目标目录。

```lua
targetdir ("path")
```

默认情况下，生成的项目文件会将其编译后的输出与脚本放在同一目录中。该功能允许您更改此位置。`targetdir`

#### 参数

`path`是应存储编译目标文件的目录的文件系统路径。它是相对于当前正在执行的脚本文件指定的。

#### 适用于

项目配置。

#### 例子

该项目按配置类型分离其编译输出。

```lua
project "MyProject"

  filter { "configurations:Debug" }
    targetdir "bin/debug"

  filter { "configurations:Release" }
    targetdir "bin/release"
```

## 13.objdir中间目录

设置构建项目时应放置对象和其他中间文件的目录。

```lua
objdir ("path")
```

默认情况下，中间文件将存储在与项目同一目录中名为“obj”的目录中。该功能允许您更改此位置。`objdir`

为避免构建配置之间的冲突，Premake 将通过附加一个或多个构建配置名称、平台名称或项目名称来确保每个中间目录都是唯一的。可以使用“！” 前缀来防止此行为，并允许重叠的中间目录。有关详细信息，请参阅以下示例。

#### 参数

`path`是应存储对象和中间文件的目录，相对于当前执行的脚本文件指定。可以使用[代币](https://premake.github.io/docs/Tokens)。

#### 适用于

项目配置。

#### 例子

对中间文件使用名为“obj”（默认值）的目录。实际目录将是 和 。`obj/Debug``obj/Release`

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }

project "MyProject"
   objdir "obj"
```

对中间文件使用名为“obj”（默认值）的目录。实际目录将是 、 、 和 。`obj/Debug/x32``obj/Debug/x64``obj/Release/x32``obj/Release/x64`

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }
   platforms { "x32", "x64" }

project "MyProject"
   objdir "obj"
```

使用令牌重新格式化路径。由于最终结果是唯一的，因此 Premake 不会追加任何额外的目录。实际目录将是 、 、 和 。`obj/x32_Debug``obj/x64_Debug``obj/x32_Release``obj/x64_Release`

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }
   platforms { "x32", "x64" }

project "MyProject"
   objdir "obj/%{cfg.platform}_%{cfg.buildcfg}"
```

使用“！” 前缀强制使用 Visual Studio 提供的环境变量而不是 Premake 令牌来执行特定目录。

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }
   platforms { "x32", "x64" }

project "MyProject"
   objdir "!obj/$(Platform)_$(Configuration)"
```

## 14.pchheader 预编译头文件

指定预编译头文件名的 #include 形式。

```lua
pchheader ("name.h")
```

有关更多信息，请参阅[预编译标头。](https://premake.github.io/docs/Precompiled-Headers)

#### 参数

`name.h`是预编译标头的名称，如在项目源代码的 #include 语句中指定的那样。如果您的源代码包含如下所示的标头：

```c
#include "myproject.h"
```

...在脚本中指定标头，如下所示，即使文件本身位于相对于项目的不同路径上（并且可能在编译时通过包含文件搜索路径找到）：

```lua
pchheader "myproject.h"
```

#### 适用于

项目配置。

## 15.pchsource VS下的预编译文件

指定控制标头编译的 C/C++ 源代码文件。

```lua
pchsource ("sourcefile.cpp")
```

有关更多信息，请参阅[预编译标头。](https://premake.github.io/docs/Precompiled-Headers)

#### 参数

sourcefile.cpp是触发标头编译的源代码的名称。此文件必须包含头文件的语句作为代码的第一行;这通常是文件中唯一的语句。

#### 适用于

项目配置。

## 16.files 文件

将文件添加到项目中。

```lua
files { "file_list" }
```

#### 参数

```
file_list`指定一个或多个文件模式。应相对于当前正在执行的脚本文件指定文件路径。文件模式可以包含通配符以匹配当前目录中的文件，或通配符以执行递归匹配。`*``**
```

如果通配符匹配的文件数超过您的预期，您可以使用 [removefiles（）](https://premake.github.io/docs/Removing-Values) 函数过滤结果。

#### 适用于

项目配置。但是[，并非所有导出器当前都支持](https://premake.github.io/docs/Feature-Matrix)每个配置文件列表。

#### 例子

从包含脚本的同一目录向当前项目添加两个文件。

```lua
files { "hello.cpp", "goodbye.cpp" }
```

将 **src/** 目录中的所有 C++ 文件添加到项目中。

```lua
files { "src/*.cpp" }
```

添加 **src/** 目录和任何子目录中的所有 C++ 文件。

```lua
files { "src/**.cpp" }
```

为特定系统添加文件;可能不适用于所有导出器。

```lua
filter "system:Windows"
  files { "src/windows/*.h", "src/windows/*.cpp" }

filter "system:MacOSX"
  files { "src/mac/*.h", "src/mac/*.cpp" }
```

17.includedirs 文件搜索路径

18.libdirs

## 19.links 链接

指定要链接的库和项目的列表。

```lua
links { "references" }
```

#### 参数

`references`是库和项目名称的列表。

与同一工作区中的其他项目链接时，请在此处指定项目名称，而不是库名称。Premake 将找出要针对当前配置链接的正确库，并且还将在项目之间创建依赖关系以确保正确的构建顺序。

链接到系统库时，请勿包含任何前缀或文件扩展名。Premake 将使用适用于当前平台的命名约定。有两个例外：

- 托管C++项目可以通过显式指定“.dll”文件扩展名来链接到托管程序集。应继续指定未管理的库，而不进行任何修饰。
- 可以通过显式包含“.framework”文件扩展名来链接 Objective C 框架。
- 对于 Visual Studio，这会将指定的项目添加到 References 中。相反，'dependson' 在解决方案中两个项目之间生成构建顺序依赖关系。

#### 适用于

项目配置。

#### 可用性

Premake 4.0 或更高版本。

#### 例子

链接到某些系统库。

```lua
filter { "system:windows" }
   links { "user32", "gdi32" }

filter { "system:linux" }
   links { "m", "png" }

filter { "system:macosx" }
   -- OS X frameworks need the extension to be handled properly
   links { "Cocoa.framework", "png" }
```

在包含两个项目的工作区中，将库链接到可执行文件。请注意，项目名称用于指定链接;Premake 将自动找出正确的库文件名和目录，并创建项目依赖项。

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }
   language "C++"

   project "MyExecutable"
      kind "ConsoleApp"
      files "**.cpp"
      links { "MyLibrary" }

   project "MyLibrary"
      kind "SharedLib"
      files "**.cpp"
```

您可以为每个库显式指定链接机制。要显式设置库的链接类型，请向库添加 或 后缀。请注意，此功能仅适用于 和 工具集。`:static``:shared``gcc``clang`

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }
   language "C++"

   project "MyExecutable"
      kind "ConsoleApp"
      files "**.cpp"
      links { "LibraryA:static", "LibraryB:shared" }
```

您还可以在非库项目之间创建链接。在这种情况下，Premake 将生成构建依赖项（链接的项目将首先构建），但不会生成实际链接。在此示例中，MyProject 使用构建依赖项来确保首先构建 MyTool。然后，它使用 MyTool 作为其构建过程的一部分。

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }
   language "C++"

   project "MyProject"
      kind "ConsoleApp"
      files "**.cpp"
      links { "MyTool" }
      prebuildcommands { "MyTool --dosomething" }

   project "MyTool"
      kind "ConsoleApp"
      files "**.cpp"
```

## 

## 20.filter 过滤器

将后续生成设置限制为特定环境。

```lua
filter { "prefix:keywords" }
```

脚本中此函数之后出现的任何设置将仅应用于与所有列出的关键字匹配的上下文。有关一些使用示例，请参阅下文。

#### 参数

```
keywords`是标识符列表，以应测试它们的字段为前缀。当此列表中的所有关键字都与当前上下文匹配时，将应用语句后面的设置。如果任何关键字未通过此测试，则忽略这些设置。关键字可能包含通配符，并且不区分大小写。有关示例，请参阅下文。`filter
```

每个关键字必须包含一个前缀，以指定应测试哪个字段。当前支持以下字段前缀：

- [action](https://premake.github.io/docs/globals/_ACTION)
- [architecture](https://premake.github.io/docs/architecture)
- [configurations](https://premake.github.io/docs/configurations)
- [files](https://premake.github.io/docs/files)
- [kind](https://premake.github.io/docs/kind)
- [language](https://premake.github.io/docs/language)
- [options](https://premake.github.io/docs/globals/_OPTIONS)
- [platforms](https://premake.github.io/docs/platforms)
- [system](https://premake.github.io/docs/system)
- [toolset](https://premake.github.io/docs/toolset)

关键字可以使用 和 通配符来匹配多个术语或文件。您还可以使用修饰符 和 来构建更复杂的条件。同样，请参阅下面的示例以获取更多信息。`\*``\*\*``not``or`

#### 可用性

Premake 5.0 或更高版本。

#### 例子

定义一个仅适用于调试生成的新符号。

```lua
workspace "MyWorkspace"
  configurations { "Debug", "Release" }

filter "configurations:Debug"
  defines { "_DEBUG" }
```

如果关键字中未指定字段前缀，则使用“configurations”作为默认值。

```lua
filter "Debug"
  defines { "_DEBUG" }
```

仅在面向 Visual Studio 2010 时定义符号。

```lua
filter "action:vs2010"
  defines { "VISUAL_STUDIO_2005" }
```

通配符可用于匹配多个术语。为所有版本的 Visual Studio 定义符号。

```lua
filter "action:vs*"
  defines { "VISUAL_STUDIO" }
```

当可能有多个值时，可以使用 **or** 修饰符。仅为库目标定义一个值。

```lua
filter "kind:SharedLib or StaticLib"
  defines { "LIBRARY_TARGET" }
```

当列出多个关键字时，它们之间会假定隐式 **and**。仅在使用 GNU Make 和 GCC 时定义编译器选项。

```lua
filter { "action:gmake*", "toolset:gcc" }
  buildoptions {
    "-Wall", "-Wextra", "-Werror"
  }
```

如果任何关键字模式与当前上下文不匹配，则会跳过整个过滤器。当存在多个关键字时，必须使用 Lua 的大括号列表语法。

向库的调试版本添加后缀。

```lua
-- (configurations == "Debug") and (kind == SharedLib or kind == "StaticLib")
filter { "Debug", "kind:SharedLib or StaticLib" }
  targetsuffix "_d"

-- Could also be written as
filter { "Debug", "kind:*Lib" }
  targetsuffix "_d"
```

根据自定义[命令行选项](https://premake.github.io/docs/Command-Line-Arguments)的存在应用设置。

```lua
-- Using an option like --localized
filter "options:localized"
  files { "src/localizations/**" }

-- Using an option like --renderer=opengl
filter "options:renderer=opengl"
  files { "src/opengl/**.cpp" }
```

尽管目前支持有限，但您也可以将设置应用于特定文件或文件集。此示例为所有 PNG 图像文件设置生成作。

```lua
filter "files:*.png"
  buildaction "Embed"
```

对于文件，您还可以使用 ***\*** 通配符，它将递归到子目录中。

```lua
filter "files:**.png"
  buildaction "Embed"
```

您还可以使用 **不**将设置应用于未设置标识符的所有环境。

```lua
filter "system:not windows"
  defines { "NOT_WINDOWS" }
```

您可以在单个关键字中组合不同的前缀。

```lua
filter "system:windows or language:C#"
```

最后，您可以通过向函数传递一个空表来重置过滤器并删除所有活动关键字。

```lua
filter {}
```

#### 澄清笔记

- 设置筛选器后，任何以前的筛选器作都将变为非活动状态。换句话说，启动过滤器的行为就像先发生重置，然后设置新的过滤器条件一样。
- 筛选器可以被视为一个范围概念。当调用过滤器重置作或启动项目定义时，当前设置的过滤器将“超出范围”。
- 过滤器对空格敏感。例如，过滤器 的 与根本不同。`system:not windows``system: not windows`

## 21.defines 定义

将预处理器或编译器符号添加到项目中。

```lua
defines { "symbols" }
```

#### 参数

`symbols`指定要定义的符号列表。

#### 适用于

项目配置。

#### 例子

在当前项目中定义两个新宏。

```lua
defines { "DEBUG", "TRACE" }
```

宏也可以赋值。

```lua
defines { "CALLSPEC=__dllexport" }
```

## 22.buildoptions  构建选项

将参数直接传递给编译器命令行，无需转换。

```lua
buildoptions { "options" }
```

如果项目包含多个，则对列表的调用将按照它们在脚本中的显示顺序进行连接。`buildoptions`

#### 参数

`options`是特定于特定编译器的编译器标志和选项的列表。

#### 适用于

项目配置。

#### 例子

使用 GCC 在 Linux 上构建时使用样式配置。构建选项始终特定于编译器，并且应针对特定工具集。`pkg-config`

```lua
filter { "system:linux", "action:gmake" }
  buildoptions { "`wx-config --cxxflags`", "-ansi", "-pedantic" }
```

## 23.buildlog 构建日志位置

指定工具集的生成日志的输出位置。

```lua
buildlog ("path")
```

如果未指定生成日志路径，则将使用工具集的默认路径。

#### 参数

`path`是构建日志文件的输出文件系统位置。

#### 适用于

项目配置。

## 24.buildcommands 构建时执行命令

指定生成项目或文件时要执行的一个或多个 shell 命令。

```lua
buildcommands { "commands" }
```

#### 参数

`commands`指定要执行的一个或多个 shell 命令的列表。这些命令可以使用[令牌](https://premake.github.io/docs/Tokens)。

#### 适用于

Makefile 项目和每个文件的自定义构建命令。

#### 例子

使用[每个文件的自定义构建命令](https://premake.github.io/docs/Custom-Build-Commands)将项目中的所有 Lua 文件编译为 C：

```lua
filter 'files:**.lua'
   -- A message to display while this build step is running (optional)
   buildmessage 'Compiling %[%{file.relpath}]'

   -- One or more commands to run (required)
   buildcommands {
      'luac -o "%[%{!cfg.objdir}/%{file.basename}.out]" "%[%{file.relpath}]"'
   }

   -- One or more outputs resulting from the build (required)
   buildoutputs { '%{cfg.objdir}/%{file.basename}.c' }
```

使用 [Makefile 项目](https://premake.github.io/docs/Makefile-Projects)执行外部 makefile。

```lua
workspace "Workspace"
   configurations { "Debug", "Release" }

project "MyProject"
   kind "Makefile"

   buildcommands {
      "make %{cfg.buildcfg}"
   }

   cleancommands {
      "make clean %{cfg.buildcfg}"
   }
```

## 25.postbuildcommands 构建后执行命令

指定在构建完成后要运行的 shell 命令。

```lua
postbuildcommands { "commands" }
```

#### 参数

`commands`是一个或多个 shell 命令。

#### 适用于

项目配置。

#### 例子

```lua
postbuildcommands { "{COPYFILE} %[default.config] %[bin/project.config]" }
```

## 26.buildinputs 构建输入

指定自定义生成命令或规则的源文件文件输入。

```lua
buildinputs { "inputs" }
```

#### 参数

`inputs`是输入源文件的列表。

#### 适用于

项目配置。

## 27.buildmessage 构建消息

指定执行自定义生成命令或规则时要输出到的文本。

```lua
buildmessage ("message")
```

#### 参数

`message`是要写入标准输出的文本。

#### 适用于

项目配置和规则。

## 28.buildoutputs 构建输出

指定自定义生成命令或规则的文件输出。

```lua
buildoutputs { "output" }
```

#### 参数

`output`是由自定义生成命令或规则创建或更新的文件。

#### 适用于

项目配置和规则。

## 29.platforms 构建平台

指定一组构建平台，在构建时充当另一个配置轴。

```lua
platforms { "names" }
```

此处列出的平台只是要在 IDE 中显示的名称，没有内在含义。名为“x86_64”的平台不会创建 64 位构建;仍必须指定适当的体系结构。有关详细信息，请参阅[配置和平台](https://premake.github.io/docs/Configurations-and-Platforms)。

#### 参数

`names`是平台名称的列表。允许使用空格，但可能会使使用某些 Premake 功能（例如命令行配置选择）变得更加困难。

#### 适用于

工作区和项目。

#### 例子

指定工作区的调试和发布配置，具有 32 位和 64 位变体的静态和共享库“平台”。

```lua
workspace "MyWorkspace"
  configurations { "Debug", "Release" }
  platforms { "Static32", "Shared32", "Static64", "Shared64" }

  filter "platforms:Static32"
    kind "StaticLib"
    architecture "x32"

  filter "platforms:Static64"
    kind "StaticLib"
    architecture "x64"

  filter "platforms:Shared32"
    kind "SharedLib"
    architecture "x32"

  filter "platforms:Shared64"
    kind "SharedLib"
    architecture "x64"
```

## 30.libdirs 库搜索路径

指定链接器的库搜索路径。

```lua
libdirs { "paths" }
```

.NET 工具对库搜索目录没有很好的支持。Visual Studio 会将 relative paths 更改为 absolute，从而难以共享生成的项目。MonoDevelop 根本不支持搜索目录，只使用 GAC。通常，最好在[链接](https://premake.github.io/docs/links)中包含到程序集的完整 （相对） 路径。C/C++ 项目没有此限制。

#### 参数

`paths`指定库搜索目录的列表。应相对于当前运行的脚本文件指定路径。

#### 适用于

项目配置。

#### 可用性

Premake 4.0 或更高版本。

#### 例子

定义两个库文件搜索路径。

```lua
libdirs { "../lua/libs", "../zlib" }
```

您还可以使用通配符来匹配多个目录。* 将与单个目录匹配，** 也将递归到子目录中。

```lua
libdirs { "../libs/**" }
```

## 31.floatingpoint浮点数

指定应使用的浮点数学的样式。

```lua
floatingpoint "value"
```

如果未为配置设置值，则将使用工具集的默认浮点设置。

#### 参数

`value`指定所需的浮点数学样式：

| 价值    | 描述                             |
| ------- | -------------------------------- |
| Default | 使用工具集的浮点设置。           |
| Fast    | 以牺牲准确性为代价实现浮点优化。 |
| Strict  | 以牺牲性能为代价提高浮点一致性。 |

#### 适用于

项目配置。

#### 例子

```lua
floatingpoint "Fast"
```

## 32.systemversion 操作系统版本

指定目标作系统的最小和最大版本。

```lua
systemversion ("value")
```

#### 参数

```
value`是一个以冒号分隔的字符串，指定最小值和最大值版本。`min:max
```

范围目前仅受具有 Visual Studio作的 Windows 目标支持。

否则，只能为具有 和 的作的 macOS/iOS/tvOS 目标设置最低版本。`xcode``gmake`

#### 适用于

项目。

#### 例子

```lua
filter "system:windows"
   systemversion "10.0.10240.0" -- To specify the version of the SDK you want
```

```lua
filter "system:windows"
   systemversion "latest" -- To use the latest version of the SDK available
```

```lua
filter "system:windows"
   systemversion "10.0.10240.0:latest" -- To specify a range of minumum and maximum versions
```

```lua
filter "system:macosx"
   systemversion "13.0" -- To target a minimum macOS deployment version of 13.0
```

#### 苹果目标

在 macOS 下，这设置了应用程序运行所需的作系统的最低版本，相当于设置（或更新）编译器标志。`-mmacosx-version-min``-mmacos-version-min`

iOS、iPadOS 和 watchOS 系统目标也是如此，只是它等效于设置（或更新的）编译器标志。`-miphoneos-version-min``-mios-version-min`

警告:还有一个 or 编译器标志，但 Premake 尚不支持 iOS 模拟器目标。`-miphonesimulator-version-min``-mios-simulator-version-min`

对于 tvOS 系统目标也是如此，只是它等效于设置（或更新的）编译器标志。`-mappletvos-version-min``-mtvos-version-min`

警告:还有一个 or 编译器标志，但 Premake 尚不支持 tvOS 模拟器目标。`-mappletvsimulator-version-min``-mtvos-simulator-version-min`

对于作，这相当于 、 、 或 Xcode 设置（取决于目标作系统）。`xcode``MACOSX_DEPLOYMENT_TARGET``IPHONEOS_DEPLOYMENT_TARGET``TVOS_DEPLOYMENT_TARGET`

#### Windows 目标

在 Windows 和 Visual Studio作下，这等效于设置 （如果面向） MSBuild 属性。`WindowsTargetPlatformVersion``WindowsTargetPlatformMinVersion``UWP`

## 33.optimize 优化

函数指定在构建目标配置时使用的优化级别和类型。

```lua
optimize "value"
```

如果未为配置设置任何值，则将执行工具集的默认优化（通常无）。

#### 参数

*value* 指定所需的优化级别：

| 价值  | 描述                             |
| ----- | -------------------------------- |
| Off   | 不会执行任何优化。               |
| On    | 执行一组平衡的优化。             |
| Debug | 通过一些调试器逐步支持进行优化。 |
| Size  | 针对最小文件大小进行优化。       |
| Speed | 优化以获得最佳性能。             |
| Full  | 全面优化。                       |

#### 适用于

项目配置。

#### 例子

```lua
optimize "Speed"
```

## 34.symbols 符号

打开/关闭调试符号表生成。

```lua
symbols "switch"
```



默认情况下，生成的项目文件将使用编译器默认设置来生成调试符号。这可能是打开或关闭，或者完全取决于配置。

#### 参数

`switch`是符号信息的标识符。

| 选择       | 可用性                        |
| ---------- | ----------------------------- |
| `Default`  | 始终可用                      |
| `Off`      | 始终可用                      |
| `On`       | 始终可用                      |
| `FastLink` | Visual Studio 2015 或更高版本 |
| `Full`     | Visual Studio 2017 或更高版本 |

#### 适用于

项目配置。

#### 例子

此项目生成调试符号信息，以便更好地调试。

```lua
project "MyProject"
    symbols "On"
```