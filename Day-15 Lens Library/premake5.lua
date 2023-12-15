workspace "Lens Library"
    architecture "x64"
    configurations { "Debug", "Release" }

local outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "part_one"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "../cpputils/" }
    files { "src/lens_library_part_one.cpp", "input.txt", "sample.txt"}


project "part_two"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "../cpputils/" }
    files { "src/lens_library_part_two.cpp", "input.txt", "sample.txt" }
