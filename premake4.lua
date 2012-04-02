solution "A*"
    configurations { "Debug", "Release" }
    platforms { "native", "x32", "x64" }

    project "astar_test"
        language "C"
        buildoptions { "-std=c99" }

        files { "src/**" }

        targetdir "bin"
        targetname "Test"

        --------------------------------------- 
        -- Link static libraries and config
        libdirs ("lib")
        links { "GLEW" }

        configuration "linux"
            targetprefix "linux_"
            links { "GL", "X11", "Xrandr", "pthread", "m", "glfw" }

        configuration "windows"
            targetprefix "windows_"
            links { "glfw", "opengl32" }

        configuration { "native or x64" }
            targetsuffix "64"

        configuration "x32"
            targetsuffix "32"

        --------------------------------------- 
        -- Build rules
        configuration "Debug"
            kind "ConsoleApp" 
            defines "DEBUG"
            flags { "Symbols", "ExtraWarnings" }

        configuration "Release"
            kind "ConsoleApp"
            flags { "Optimize", "ExtraWarnings" }

