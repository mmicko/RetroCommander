UNAME := $(shell uname)
ifeq ($(UNAME),$(filter $(UNAME),Linux Darwin FreeBSD GNU/kFreeBSD))
ifeq ($(UNAME),$(filter $(UNAME),Darwin))
OS=darwin
else
ifeq ($(UNAME),$(filter $(UNAME),FreeBSD GNU/kFreeBSD))
OS=bsd
else
OS=linux
endif
endif
else
OS=windows
endif

# $(info $(OS))

GENIE=3rdparty/bx/tools/bin/$(OS)/genie

all:
	$(GENIE) vs2013
	$(GENIE) vs2015
	$(GENIE) --gcc=mingw-gcc gmake
	$(GENIE) --gcc=linux-gcc gmake
	$(GENIE) --gcc=osx gmake

build/projects/gmake-linux:
	$(GENIE)  --gcc=linux-gcc gmake
linux-debug32: build/projects/gmake-linux
	$(MAKE) -R -C build/projects/gmake-linux config=debug32
linux-release32: build/projects/gmake-linux
	$(MAKE) -R -C build/projects/gmake-linux config=release32
linux-debug64: build/projects/gmake-linux
	$(MAKE) -R -C build/projects/gmake-linux config=debug64
linux-release64: build/projects/gmake-linux
	$(MAKE) -R -C build/projects/gmake-linux config=release64
linux: linux-debug32 linux-release32 linux-debug64 linux-release64
build/projects/gmake-mingw-gcc:
	$(GENIE)  --gcc=mingw-gcc gmake
mingw-gcc-debug32: build/projects/gmake-mingw-gcc
	$(MAKE) -R -C build/projects/gmake-mingw-gcc config=debug32
mingw-gcc-release32: build/projects/gmake-mingw-gcc
	$(MAKE) -R -C build/projects/gmake-mingw-gcc config=release32
mingw-gcc-debug64: build/projects/gmake-mingw-gcc
	$(MAKE) -R -C build/projects/gmake-mingw-gcc config=debug64
mingw-gcc-release64: build/projects/gmake-mingw-gcc
	$(MAKE) -R -C build/projects/gmake-mingw-gcc config=release64
mingw-gcc: mingw-gcc-debug32 mingw-gcc-release32 mingw-gcc-debug64 mingw-gcc-release64

build/projects/vs2012:
	$(GENIE)  vs2012

build/projects/vs2013:
	$(GENIE)  vs2013

build/projects/vs2015:
	$(GENIE)  vs2015

build/projects/gmake-osx:
	$(GENIE)  --gcc=osx gmake
osx-debug32: build/projects/gmake-osx
	$(MAKE) -C build/projects/gmake-osx config=debug32
osx-release32: build/projects/gmake-osx
	$(MAKE) -C build/projects/gmake-osx config=release32
osx-debug64: build/projects/gmake-osx
	$(MAKE) -C build/projects/gmake-osx config=debug64
osx-release64: build/projects/gmake-osx
	$(MAKE) -C build/projects/gmake-osx config=release64
osx: osx-debug32 osx-release32 osx-debug64 osx-release64

clean:
	@echo Cleaning...
	-@rm -rf build

