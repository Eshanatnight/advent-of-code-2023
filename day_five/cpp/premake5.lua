workspace "day_five"
    architecture "x64"
    configurations { "Debug", "Release" }

local outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "part_one"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files { "src/part_one.cpp"}


project "part_two"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files { "src/part_two.cpp"}