--
-- TODO
-- * resolution, threading commandline flags
-- * use Custom Build Commands for executing kkrunchy/linux pack script
--

solution "Demo"
	location ("PremakeBuilds/".._ACTION)

	configurations {
		"Debug",
		"Release",
	}

	filter "system:linux"
		defines { "__linux" }
	filter "system:windows"
		defines { "_WINDOWS" }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		flags {
			"Symbols" -- Generate debugging information.
		}
	filter "configurations:Release"
		defines { "NDEBUG" }


project "vinyl"
	language   "C++"
	kind       "WindowedApp"
	targetname "demo"

	platforms {
		"V2",
		"4klang",
	}
	filter { "platforms:V2", "system:windows" }
		defines { "SYNTH_V2" }
	filter "platforms:4klang"
		defines { "SYNTH_4KLANG" }

	filter {}
	files {
		"Source/**.cpp",
		"Source/**.h",
	}
	removefiles {
		"Source/Music/*",
	}

	libdirs {
		"Lib/lib",
		"Source/music",
	}
	-- links { }

	includedirs {
		"Lib/include",
	}

	filter { "configurations:Release" }
		optimize "Size"
	filter { "configurations:Debug" }
		optimize "Off" -- "Debug"


	--
	-- Windows
	--

	filter { "system:windows" }
		flags {
			"MultiProcessorCompile",
			"OmitDefaultLibrary", -- Omit the specification of a runtime library in object files.
			"WinMain", -- Use `WinMain()` as entry point for Windows applications, rather than the default `main()`.
		}
		buildoptions {
			"/GS-", -- Disable Security checks
			"/MT", -- Runtime Library Multi-threaded
		}
		linkoptions {
			"/NODEFAULTLIB", -- Ignore All Default Libraries
			"/SAFESEH:NO", -- Image Has Safe Exception Handers: No
		}
		links {
			"opengl32",
		}
		exceptionhandling "Off"

	-- V2

	filter { "platforms:V2", "system:windows" }
		files {
			"Source/music/soundtrack.v2m",
			"Source/music/v2inc.asm",
			"Lib/include/libv2.h",
			"Lib/include/v2mplayer.cpp",
			"Lib/include/v2mplayer.h",
		}
		links {
			"Dsound",
			"winmm",
			"libv2",
			"%{cfg.objdir}/v2inc",
		}

	filter { "configurations:Debug", "platforms:V2", "system:windows", "files:Source/music/v2inc.asm" }
		buildcommands "nasm.exe -f win32 -o %{cfg.objdir}%{file.basename}.lib %{file.abspath}"
		buildmessage "%{file.name}"
		buildoutputs "%{cfg.objdir}%{file.basename}.lib"
	filter { "configurations:Release", "platforms:V2", "system:windows", "files:Source/music/v2inc.asm" }
		buildcommands "nasm.exe -f win32 -o %{cfg.objdir}%{file.basename}.lib %{file.abspath} -DNDEBUG%3b"
		buildmessage "%{file.name}"
		buildoutputs "%{cfg.objdir}%{file.basename}.lib"

	-- 4klang

	filter { "platforms:4klang", "system:windows" }
		files {
			"Source/music/4klang.windows.h",
			"Source/music/4klang.windows.lib",
		}
		links {
			"winmm",
			"4klang.windows",
		}


	--
	-- Linux
	--

	filter { "system:linux" }
		removeplatforms { "V2" }
		buildoptions {
			"-std=c++11",
			"-Wall",
			"-Wextra",
		}
		links {
			"GL",
			"X11",
			"asound",
		}
		filter { "configurations:release" }
			linkoptions {
				-- usleep is a problem
				-- "-nodefaultlibs",
			}

	-- 4klang

	filter { "platforms:4klang", "system:linux" }
		files {
			"Source/music/4klang.linux.h",
			"Source/music/4klang.linux.o",
		}
		links {
			"Source/music/4klang.linux.o"
		}
		buildoptions {
			-- for use with -nodefaultlibs
			-- "-fno-use-cxa-atexit",
			"-m32",
		}
		linkoptions {
			"-m32",
			"-pthread",
		}
