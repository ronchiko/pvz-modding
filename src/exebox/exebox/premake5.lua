project "ExeBox"
    kind "ConsoleApp"
    files {
        "**.cpp",
        "**.hpp",
        "**.h",
    }

    links {
        "framework"
    }

    filter {"action:vs*"}
        linkoptions "/BASE:0xF000000"
    filter{}