
language "C++"
cppdialect "C++20"
cdialect "C17"

workspace "PvZ Modding"
    kind "StaticLib"
    staticruntime "on"

    configurations { 
        "Release", 
        "Debug"
    }
    platforms { 
        "Windows-x86"
    }

    startproject "ExeBox"

    filter {"platforms:Windows"}
        defines { "_WINDOWS" }
        system "Windows"
        architecture "x86"

    filter {}

targetdir ("%{wks.location}/.build/bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}")
objdir  ("%{wks.location}/.build/obj/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}")


include "src/exebox"