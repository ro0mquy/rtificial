--
-- TODO
-- * resolution, threading commandline flags
-- * use Custom Build Commands for executing kkrunchy/linux pack script/shader minifier
-- * generalize nasm define pass through http://stackoverflow.com/questions/32531784/get-list-of-defines-as-token-or-string-in-premake-5
--

newoption {
	trigger = "project",
	value = "PATH",
	description = "Path to the project directory of the strahlenwerk project to be build.",
	default = "Builds",
}


workspace "Demo"
	local workspace_dir = _OPTIONS["project"].."/export/build/".._ACTION
	location (workspace_dir)
	os.mkdir(workspace_dir)
	os.copyfile(_MAIN_SCRIPT_DIR.."/".._ACTION..".gitignore", workspace_dir.."/.gitignore")

	configurations {
		"Debug",
		"Release",
	}

	filter { "system:linux" }
		defines "__linux"
	filter { "system:windows" }
		defines "_WINDOWS"
		os.copyfile(_MAIN_SCRIPT_DIR.."/Bin/nasm.exe", workspace_dir.."/nasm.exe")

	filter { "configurations:Debug" }
		defines "_DEBUG"
		symbols "On" -- Generate debugging information.
		runtime "Debug"
	filter { "configurations:Release" }
		defines "NDEBUG"


project "vinyl"
	language   "C++"
	kind       "WindowedApp"
	targetname "rt"

	platforms {
		"V2",
		"4klang",
		"dual_V2_4klang",
		"vorbis",
	}
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
	filter { "platforms:dual_V2_4klang" }
		defines "SYNTH_DUAL_V2_4KLANG"
	filter {} -- reset filters

	files {
		"Source/*.cpp",
		"Source/*.h",
		"Source/math/*.cpp",
		"Source/math/*.h",
		_OPTIONS["project"].."/export/strahlenwerk/strahlenwerk_export*.h",
		_OPTIONS["project"].."/export/strahlenwerk/shaders/*.h",
	}
	removefiles {
		"Source/*Frontend.cpp",
		"Source/incbin.asm",
	}

	libdirs {
		"Lib/lib",
	}

	includedirs {
		"Lib/include",
		_OPTIONS["project"].."/export/strahlenwerk/",
	}

	filter { "configurations:Release" }
		optimize "Size"
	filter { "configurations:Debug" }
		optimize "Off" -- "Debug"

	-- nasm custom build commands for including binary data
	filter { "files:Source/incbin.asm", "platforms:vorbis or V2 or dual_V2_4klang" }
		buildcommands 'nasm -f %{cfg.system == "linux" and "elf64" or cfg.system == "windows" and "win32"} -o %{cfg.objdir}/%{file.basename}.%{cfg.system == "linux" and "o" or cfg.system == "windows" and "lib"} %{file.abspath} %{table.implode(cfg.defines, "-D", "", " ")}'
		buildmessage "%{file.name}"
		buildoutputs '%{cfg.objdir}/%{file.basename}.%{cfg.system == "linux" and "o" or cfg.system == "windows" and "lib"}'


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
		toolset "msc-140"
		characterset ("MBCS")

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
		filter { "configurations:Debug", "system:windows" }


	-- V2

	filter { "platforms:V2 or dual_V2_4klang", "system:windows" }
		files {
			_OPTIONS["project"].."/export/music/soundtrack.v2m",
			"Source/incbin.asm",
			"Lib/include/libv2.h",
			"Lib/include/v2mplayer.cpp",
			"Lib/include/v2mplayer.h",
		}
		links {
			"Dsound",
			"winmm",
			"libv2",
		}

		filter { "platforms:V2 or dual_V2_4klang", "system:windows", "configurations:Debug" }
			ignoredefaultlibraries { "libcmt.lib" }

	-- 4klang

	filter { "platforms:4klang or dual_V2_4klang", "system:windows" }
		files {
			_OPTIONS["project"].."/export/music/soundtrack.4klang.h",
			_OPTIONS["project"].."/export/music/soundtrack.4klang.lib",
		}
		libdirs {
			_OPTIONS["project"].."/export/music/",
		}
		links {
			"winmm",
			"soundtrack.4klang",
		}

	-- vorbis

	filter { "platforms:vorbis", "system:windows" }
		files {
			_OPTIONS["project"].."/music.ogg",
			"Source/incbin.asm",
			"Lib/include/stb_vorbis_wrapper.c",
			"Lib/include/stb_vorbis.h",
		}
		links {
			"winmm",
		}

		filter { "configurations:Release", "platforms:vorbis", "system:windows" }
			defines {
				"STB_VORBIS_NO_CRT",
			}
			buildoptions {
				-- trade __dtoui3 missing against __ftol2 missing
				-- this option is undocumented!
				-- http://stackoverflow.com/questions/19556103/how-to-get-vs2013-to-stop-generating-calls-to-dtol3-dtoui3-and-other-funct
				-- "/d2noftol3",
				"/QIfist", -- see rtificial/ps0ke/hg_tipps/a_wild_tee_appears__msvc__flt_foo.txt
			}
			optimize "Off"




	--
	-- Linux
	--

	filter { "system:linux" }
		removeplatforms { "V2", "dual_V2_4klang" }
		files {
			"Source/LinuxFrontend.cpp",
		}
		buildoptions {
			"-std=c++11",
			"-Wall",
			"-Wextra",
		}
		linkoptions {
			"-pthread",
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
	-- TODO migrate to new export file structure

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
		}

	-- vorbis
	-- TODO migrate to new export file structure

	filter { "platforms:vorbis", "system:linux" }
		files {
			"Source/music/bpm.h",
			"Source/music/soundtrack.ogg",
			"Source/incbin.asm",
			"Lib/include/stb_vorbis_wrapper.c",
			"Lib/include/stb_vorbis.h",
			"%{cfg.objdir}/incbin.o",
		}

		-- someday
		--[[
		filter { "configurations:Release", "platforms:vorbis", "system:linux" }
			defines {
				"STB_VORBIS_NO_CRT",
			}
		--]]

