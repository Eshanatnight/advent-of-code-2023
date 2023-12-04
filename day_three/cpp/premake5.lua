workspace "day_three"
    architecture "x64"
    configurations { "Debug", "Release" }

local outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "part_one"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files { "src/part_one.cpp", "part_one.input.txt", "part_one.sample.txt"}


project "part_two"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files { "src/part_two.cpp", "part_two.input.txt", "part_two.sample.txt" }
