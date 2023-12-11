workspace "Cosmic Expansion"
    architecture "x64"
    configurations { "Debug", "Release" }

local outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "part_one"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/bin-int/" .. outputdir .. "/%{prj.name}")
    files { "src/cosmic_expansion_part_one.cpp", "one.input.txt", "one.sample.txt"}


project "part_two"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/bin-int/" .. outputdir .. "/%{prj.name}")
    files { "src/cosmic_expansion_part_two.cpp", "one.input.txt", "one.sample.txt" }
