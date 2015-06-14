solution "RetroCommander"
	configurations {
		"Debug",
		"Release",
	}

	if _ACTION == "xcode4" then
		platforms {
			"Universal",
		}
	else
		platforms {
			"x32",
			"x64",
			"Native", -- for targets where bitness is not specified
		}
	end

	language "C++"
	startproject "RetroCommander"

COMMANDER_DIR = path.getabsolute("..")
local COMMANDER_BUILD_DIR = path.join(COMMANDER_DIR, "build")
local BGFX_THIRD_PARTY_DIR = path.join(COMMANDER_DIR, "3rdparty/bgfx/3rdparty")
BX_DIR = path.getabsolute(path.join(COMMANDER_DIR, "3rdparty/bx"))
BGFX_DIR = path.getabsolute(path.join(COMMANDER_DIR, "3rdparty/bgfx"))

dofile (path.join(BX_DIR, "scripts/toolchain.lua"))
if not toolchain(COMMANDER_BUILD_DIR, BGFX_THIRD_PARTY_DIR) then
	return -- no action specified
end

function copyLib()
end

group "3rdparty"
dofile "../3rdparty/bgfx/scripts/bgfx.lua"
dofile "../3rdparty/bgfx/scripts/example-common.lua"
bgfxProject("", "StaticLib", {})

group "main"

project ("RetroCommander")
uuid (os.uuid("RetroCommander"))
kind "WindowedApp"

configuration {}

includedirs {
	path.join(BX_DIR,   "include"),
	path.join(BGFX_DIR, "include"),
	path.join(BGFX_DIR, "3rdparty"),
	path.join(BGFX_DIR, "examples/common"),
	path.join(BGFX_DIR, "examples/common/entry"),
}

files {
	path.join(COMMANDER_DIR, "src", "**.cpp"),
	path.join(COMMANDER_DIR, "src", "**.h"),
}

links {
	"bgfx",
	"example-common",
}

configuration { "vs*" }
	linkoptions {
		"/ignore:4199", -- LNK4199: /DELAYLOAD:*.dll ignored; no imports found from *.dll
	}
	links { -- this is needed only for testing with GLES2/3 on Windows with VS2008
		"DelayImp",
	}

configuration { "vs201*" }
	linkoptions { -- this is needed only for testing with GLES2/3 on Windows with VS201x
		"/DELAYLOAD:\"libEGL.dll\"",
		"/DELAYLOAD:\"libGLESv2.dll\"",
	}

configuration { "mingw*" }
	targetextension ".exe"

configuration { "vs20* or mingw*" }
	links {
		"gdi32",
		"psapi",
	}

configuration { "linux or freebsd" }
	links {
		"Xrandr",
		"Xinerama",
		"Xi",
		"Xxf86vm",
		"Xcursor",
	}

configuration { "linux-* or freebsd" }
	links {
		"X11",
		"GL",
		"pthread",
	}
	
configuration { "osx" }
	files {
		path.join(COMMANDER_DIR, "examples/common/**.mm"),
	}
	links {
		"Cocoa.framework",
		"OpenGL.framework",
	}

configuration {}
