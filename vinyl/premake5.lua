--
-- TODO
-- * resolution, threading commandline flags
-- * use Custom Build Commands for executing kkrunchy/linux pack script/shader minifier
-- * generalize nasm define pass through http://stackoverflow.com/questions/32531784/get-list-of-defines-as-token-or-string-in-premake-5
--

workspace "Demo"
	location ("Builds/".._ACTION)

	configurations {
		"Debug",
		"Release",
	}

	filter { "system:linux" }
		defines "__linux"
	filter { "system:windows" }
		defines "_WINDOWS"

	filter { "configurations:Debug" }
		defines "_DEBUG"
		flags {
			"Symbols" -- Generate debugging information.
		}
	filter { "configurations:Release" }
		defines "NDEBUG"


project "vinyl"
	language   "C++"
	kind       "WindowedApp"
	targetname "rt"

	platforms {
		"V2",
		"4klang",
		"vorbis",
	}
	SYNTH_DEFINE = ""
	filter { "platforms:V2", "system:windows" }
		defines "SYNTH_V2"
	filter { "platforms:4klang" }
		defines "SYNTH_4KLANG"
	filter { "platforms:vorbis" }
		defines {
			"SYNTH_VORBIS",
			"STB_VORBIS_NOPUSHDATA_API",
			"STB_VORBIS_NO_STDIO",
		}
	filter {} -- reset filters

	files {
		"Source/**.cpp",
		"Source/**.h",
	}
	removefiles {
		"Source/music/*",
		"Source/*Frontend.cpp",
		"Source/incbin.asm",
	}

	libdirs {
		"Lib/lib",
		"Source/music",
	}

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
		files {
			"Source/WindowsFrontend.cpp",
		}
		flags {
			"MultiProcessorCompile",
			"WinMain", -- Use `WinMain()` as entry point for Windows applications, rather than the default `main()`.
		}
		buildoptions {
			"/GS-", -- Disable Security checks
		}
		linkoptions {
			"/SAFESEH:NO", -- Image Has Safe Exception Handers: No
		}
		links {
			"opengl32",
		}
		exceptionhandling "Off"

		filter { "configurations:Release", "system:windows" }
			flags {
				"OmitDefaultLibrary", -- Omit the specification of a runtime library in object files.
			}
			buildoptions {
				"/MT", -- Runtime Library Multi-threaded
			}
			linkoptions {
				"/NODEFAULTLIB", -- Ignore All Default Libraries
			}

	-- V2

	filter { "platforms:V2", "system:windows" }
		files {
			"Source/music/bpm.h",
			"Source/music/soundtrack.v2m",
			"Source/incbin.asm",
			"Lib/include/libv2.h",
			"Lib/include/v2mplayer.cpp",
			"Lib/include/v2mplayer.h",
		}
		links {
			"Dsound",
			"winmm",
			"libv2",
			"%{cfg.objdir}/incbin",
		}

		-- nasm custom build commands for including binary data
		filter { "files:Source/incbin.asm", "configurations:Release", "system:windows", "platforms:V2" }
			buildcommands "nasm.exe -f win32 -o %{cfg.objdir}%{file.basename}.lib %{file.abspath} -DNDEBUG%3b -DSYNTH_V2"
			buildmessage "%{file.name}"
			buildoutputs "%{cfg.objdir}%{file.basename}.lib"
		filter { "files:Source/incbin.asm", "configurations:Debug", "system:windows", "platforms:V2" }
			buildcommands "nasm.exe -f win32 -o %{cfg.objdir}%{file.basename}.lib %{file.abspath} -DSYNTH_V2"
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

	-- vorbis

	filter { "platforms:vorbis", "system:windows" }
		files {
			"Source/music/bpm.h",
			"Source/music/vorbis_info.h",
			"Source/music/soundtrack.ogg",
			"Source/incbin.asm",
			"Lib/include/stb_vorbis.c",
			"Lib/include/stb_vorbis.h",
		}
		links {
			"winmm",
			"%{cfg.objdir}/incbin",
		}

		filter { "configurations:Release", "platforms:vorbis", "system:windows" }
			defines {
				"STB_VORBIS_NO_CRT",
			}
			buildoptions {
				-- trade __dtoui3 missing against __ftol2 missing
				-- this option is undocumented!
				-- http://stackoverflow.com/questions/19556103/how-to-get-vs2013-to-stop-generating-calls-to-dtol3-dtoui3-and-other-funct
				"/d2noftol3",
			}

		-- nasm custom build commands for including binary data
		filter { "files:Source/incbin.asm", "configurations:Release", "system:windows", "platforms:vorbis" }
			buildcommands "nasm.exe -f win32 -o %{cfg.objdir}%{file.basename}.lib %{file.abspath} -DNDEBUG%3b -DSYNTH_VORBIS"
			buildmessage "%{file.name}"
			buildoutputs "%{cfg.objdir}%{file.basename}.lib"
		filter { "files:Source/incbin.asm", "configurations:Debug", "system:windows", "platforms:vorbis" }
			buildcommands "nasm.exe -f win32 -o %{cfg.objdir}%{file.basename}.lib %{file.abspath} -DSYNTH_VORBIS"
			buildmessage "%{file.name}"
			buildoutputs "%{cfg.objdir}%{file.basename}.lib"


	--
	-- Linux
	--

	filter { "system:linux" }
		removeplatforms { "V2", "vorbis", }
		files {
			"Source/LinuxFrontend.cpp",
		}
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
		filter { "configurations:release", "system:linux" }
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
