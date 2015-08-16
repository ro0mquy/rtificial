--
-- TODO
-- * resolution, threading commandline flags
-- * tokens
--
workspace "Demo"
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
		flags {
--			"OmitDefaultLibrary", -- Omit the specification of a runtime library in object files.
--			"WinMain", --Use `WinMain()` as entry point for Windows applications, rather than the default `main()`.
		}
		-- 


project "vinyl"
	language   "C++"
	kind       "WindowedApp"
	location   "PremakeBuilds"
	targetname "demo"

	platforms {
		"V2",
		"4klang",
	}
	filter { "platforms:V2", "system:windows" }
		defines { "SYNTH_V2" }
	filter "platforms:4klang"
		defines { "SYNTH_4KLANG"}

	foo = files {
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
		"Source",
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
			"MultiProcessorCompile"
		}
		links {
			"opengl32",
		}

	-- V2

	filter { "platforms:V2*", "system:windows" }
		files {
			"Source/music/soundtrack.v2m",
			"Source/music/v2inc.asm",
		}
		links {
			"Dsound",
			"winmm",
			"libv2";
		}

	filter { "configurations:Debug", "platforms:V2", "system:windows", "files:Source/music/v2inc.asm" }
		buildcommands "%{wks.location}nasm.exe -f win32 -o %{OutDir}%{Filename}.obj %{FullPath}"
		buildmessage "%{Filename}%{Extension}"
		buildoutputs "${OutDir}%{Filename}.obj;%(Outputs)"
	filter { "configurations:Release", "platforms:V2", "system:windows", "files:Source/music/v2inc.asm" }
		buildcommands "nasm.exe -f win32 -o ${OutDir}%{Filename}.obj %{FullPath} -DNDEBUG%3b"
		buildmessage "%{Filename}%{Extension}"
		buildoutputs "${OutDir}%{Filename}.obj;%{Outputs}"

	-- 4klang

	filter { "platforms:4klang", "system:windows" }
		files {
			"Source/music/4klang.windows.h",
			"Source/music/4klang.windows",
		}
		links {
			"4klang.windows",
		}


	--
	-- Linux
	--

	filter { "system:linux" }
		buildoptions {
			"-std=c++11",
			"-Wall",
			"-Wextra",
		}
		links {
			"libGL",
			"libX11",
			"libasound",
		}

	-- 4klang

	filter { "platforms:4klang", "system:linux" }
		files {
			"Source/music/4klang.linux.h",
			"Source/music/4klang.linux",
		}
		links {
			"4klang.linux"
		}
		buildoptions {
			"-m32",
		}
		linkoptions {
			"-m32",
			"-pthread"
		}
