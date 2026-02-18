# GNU make (gmake) & nmake shared makefile.
# Conditional logic implemented with differing end-of-line
# comment syntax.
# To enable multithreaded compilation, nmake compiles all
# files together with
# 	/MP, while gmake compiles automatically with -jN.
# Files are pre-compiled with pch.cpp. Reference
# <https://gist.github.com/Prince-Mandor/ee16968da4707fb3705eb39b326bdb02>
# for batch variable wrangling.

# Variables:
# PROJ: a repository can have multiple *project*s, each in
# 	their own directory.
# BIN: name of the project; a project may have multiple.
# SRC: target filenames (can be multiple) to link, starting
# 	from the project directory.
# BUILD: 0 (default), 1, or 2, specifying the type of build
# 	(debug, release, instrument).
# ARCH: 64 (default) or 86, specifying the architecture to
# 	build for. The environment may need to be initialized
# 	accordingly (e.g. for cl.exe).
# PCH: 0 or 1 (default), whether or not to use precompiled
# 	pch.cpp.
# ARGS: command-line arguments to pass to the executable to
# 	be run.
# INCL: set from caller on Windows, enumerates header file
# 	directory wildcards.

# Targets:
# build (default): compile & link.
# run: compile, link, and run.
# clean: remove intermediates directory.
# info: print select makefile variables and exit.

# Default target defined before any "fake" targets.
__default: _default

# ----------------------------------------------------------
ifdef MAKEDIR: # gmake: false; nmake: unused target
!ifdef MAKEDIR # gmake: not seen; nmake: true
# ----------------------------------------------------------
# nmake specific commands.

# It is important we get the absolute path without `..` here
# for standardized path substitution techniques later.
ROOT_DIR=$(abspath $(MAKEDIR)\..)^\
REPO_NAME=fuscate
!IFNDEF PROJ
PROJ=src
!ENDIF
PROJ_NAME=$(PROJ)
!IFNDEF BIN
!IFNDEF SRC
BIN=$(REPO_NAME)
# This will include all *.cpp* directories as well. In this
# case, name directories as *.cpp.assets.
SRC=*.cpp
!ELSE
# patsubsti wildcards need to be escaped iff they are part
# of a path.
BIN=$(patsubsti %.cpp,%,$(SRC))
!ENDIF
!ELSE
!IFNDEF SRC
SRC=$(BIN).cpp
!ENDIF
!ENDIF
BIN_NAME=$(BIN)
SRC_NAME=$(SRC)
!IFNDEF BUILD
BUILD=0
!ENDIF
!IF $(BUILD)==2
BUILD_NAME=instrument
!ELSE IF $(BUILD)==1
BUILD_NAME=release
!ELSE
BUILD_NAME=debug
!ENDIF
!IFNDEF ARCH
ARCH=64
!ENDIF
!IFNDEF PCH
PCH=1
!ENDIF

# The directory structure here must be consistent with
# macro substitution structure utilized later.
BIN_DIR=$(ROOT_DIR).bin^\
PROJ_BIN=$(BIN_DIR)$(PROJ_NAME)\$(BIN_NAME).$(BUILD_NAME).x$(ARCH).exe
PROJ_BIN_DIR=$(BIN_DIR)$(PROJ_NAME)^\
OBJ_DIR=$(ROOT_DIR).obj^\
PROJ_OBJ_DIR=$(OBJ_DIR)$(PROJ_NAME)^\
PROJ_SRC_DIR=$(ROOT_DIR)$(PROJ_NAME)^\
!IF "$(SRC_NAME)"=="*.cpp"
PROJ_SRC=$(patsubsti ..\\%,$(ROOT_DIR)\%,$(PROJ_SRC_WC))
!ELSE
PROJ_SRC=$(patsubsti %,$(PROJ_SRC_DIR)\%,$(SRC_NAME))
!ENDIF

COMPILER=cl /c
COMPILER_FLAGS_INCLUDES=/I$(ROOT_DIR)rain\include\
	/I$(ROOT_DIR)include
COMPILER_FLAGS_COMMON=$(COMPILER_FLAGS_INCLUDES)\
	/std:c++20 /D_CONSOLE\
	/Fp$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\pch.pch\
	/fp:fast /MP /permissive-\
	/Zc:__cplusplus /Zc:wchar_t /Zc:forScope /Zc:inline /Zf\
	/GS /W3 /WX- /wd4250 /wd4068 /sdl\
	/diagnostics:column /EHsc /Gm- /nologo
COMPILER_FLAGS_DEBUG=/D_DEBUG /MDd /Od /RTC1 /JMC /ZI
COMPILER_FLAGS_RELEASE=/DNDEBUG /MT /O2 /Oi /GL /Gy /Zi
COMPILER_FLAGS_INSTRUMENT=$(COMPILER_FLAGS_DEBUG)\
	/Fa$(PROJ_OBJ_DIR)
COMPILER_FLAGS_64=
COMPILER_FLAGS_86=

LINKER=link
LINKER_FLAGS_LIBRARIES=
LINKER_FLAGS_COMMON=$(LINKER_FLAGS_LIBRARIES)\
	/OUT:$(PROJ_BIN)\
	/PDB:$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(BIN_NAME).pdb\
	/ILK:$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(BIN_NAME).ilk\
	/MANIFESTUAC:"level='asInvoker' uiAccess='false'"\
	/MANIFESTFILE:$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(BIN_NAME).exe.intermediate.manifest\
	/LTCGOUT:$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(BIN_NAME).iobj\
	/SUBSYSTEM:CONSOLE /NOLOGO
LINKER_FLAGS_DEBUG=/DEBUG
LINKER_FLAGS_RELEASE=/INCREMENTAL:NO\
	/OPT:ICF /OPT:REF /LTCG:incremental
LINKER_FLAGS_INSTRUMENT=$(LINKER_FLAGS_DEBUG)
LINKER_FLAGS_64=/MACHINE:x64
LINKER_FLAGS_86=/MACHINE:x86

!IF "$(BUILD_NAME)"=="instrument"
COMPILER_FLAGS=$(COMPILER_FLAGS_COMMON)\
	$(COMPILER_FLAGS_INSTRUMENT)
LINKER_FLAGS=$(LINKER_FLAGS_COMMON)\
	$(LINKER_FLAGS_INSTRUMENT)
!ELSE IF "$(BUILD_NAME)"=="release"
COMPILER_FLAGS=$(COMPILER_FLAGS_COMMON)\
	$(COMPILER_FLAGS_RELEASE)
LINKER_FLAGS=$(LINKER_FLAGS_COMMON) $(LINKER_FLAGS_RELEASE)
!ELSE
COMPILER_FLAGS=$(COMPILER_FLAGS_COMMON)\
	$(COMPILER_FLAGS_DEBUG)
LINKER_FLAGS=$(LINKER_FLAGS_COMMON) $(LINKER_FLAGS_DEBUG)
!ENDIF

!IF $(ARCH)==86
COMPILER_FLAGS=$(COMPILER_FLAGS) $(COMPILER_FLAGS_86)
LINKER_FLAGS=$(LINKER_FLAGS) $(LINKER_FLAGS_86)
!ELSE
COMPILER_FLAGS=$(COMPILER_FLAGS) $(COMPILER_FLAGS_64)
LINKER_FLAGS=$(LINKER_FLAGS) $(LINKER_FLAGS_64)
!ENDIF
# Used in the build step to save an if statement.
!IF $(PCH)==0
COMPILER_FLAGS_PCH=
LINKER_FLAGS_PCH=
!ELSE
COMPILER_FLAGS_PCH=/Yu"rain.hpp"
LINKER_FLAGS_PCH=$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\pch.obj
!ENDIF

# Real targets.
# Default/build target only compiles and links, but does
# not run.
_default: build
# Ordering is important so that version is updated first.
build: version.build.txt $(PROJ_BIN)
# Build and run.
run: build
	@ECHO ---------------- [Run] $(BIN_NAME) ----------------
	@CD $(BIN_DIR)$(PROJ_NAME) &&\
		$(BIN_NAME).$(BUILD_NAME).x$(ARCH).exe $(ARGS)
# Clean target removes all intermediates, but remakes the
# in.txt file.
clean:
	@ECHO Cleaning...
	@IF EXIST $(BIN_DIR) RD /Q /S $(BIN_DIR)
	@IF EXIST $(OBJ_DIR) RD /Q /S $(OBJ_DIR)
info:
	@ECHO "INCL_PCH=$(INCL_PCH)"
	@ECHO "INCL=$(INCL)"
	@ECHO "PROJ_SRC_WC=$(PROJ_SRC_WC)"
	@ECHO "MAKEFLAGS=$(MAKEFLAGS)"
	@ECHO ROOT_DIR=$(ROOT_DIR)
	@ECHO "BIN_NAME=$(BIN_NAME)"
	@ECHO "SRC_NAME=$(SRC_NAME)"
	@ECHO "BUILD_NAME=$(BUILD_NAME)"
	@ECHO "ARCH=$(ARCH)"
	@ECHO "PROJ_SRC=$(PROJ_SRC)"
	@ECHO "PROJ_OBJ=$(PROJ_OBJ)"
	@ECHO "COMPILER=$(COMPILER)"
	@ECHO "COMPILER_FLAGS=$(COMPILER_FLAGS)"
	@ECHO "LINKER_FLAGS=$(LINKER_FLAGS)"

# Updates version if any project dependency has changed.
# Mark files as up-to-date after.
# Updated version may not be propagated until clean.
# Order dependencies so that output is ordered; not super
# important.
version.build.txt: $(INCL) $(PROJ_SRC)
# Skip printing outdated files; command line limit is 8192.
# @FOR %%I IN ($?) DO @ECHO Outdated: %%I.
	@version.bat $(uppercase $(REPO_NAME))
	@$(MAKE) /C /T version.build.txt
# Precompiled headers.
$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\pch.pch: pch.cpp
	@ECHO Building $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\pch.pch...
	@IF NOT EXIST $(PROJ_OBJ_DIR) MD $(PROJ_OBJ_DIR)
	@IF NOT EXIST $(PROJ_OBJ_DIR)$(BUILD_NAME)\
		MD $(PROJ_OBJ_DIR)$(BUILD_NAME)
	@IF NOT EXIST $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\
		MD $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)
	@$(COMPILER) $(COMPILER_FLAGS) /Yc\
		/Fd$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\pch.pdb\
		/Fo$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\pch.obj pch.cpp
pch.cpp: $(INCL_PCH)

# Binary build target.
# First compile all outdated files (with /MP), then link
# everything together.
# Need additional variable to parse dependencies correctly.
$(PROJ_BIN): $(PROJ_SRC)
!IF $(PCH)==1
	@$(MAKE) /C\
		$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\pch.pch\
		PROJ=$(PROJ) BIN=$(BIN) SRC="$(SRC)"\
		BUILD=$(BUILD) ARCH=$(ARCH) PCH=$(PCH)
!ENDIF
	@ECHO ---------------- [Build] $(BIN_NAME) ----------------
	@IF NOT EXIST $(PROJ_BIN_DIR) MD $(PROJ_BIN_DIR)
	@IF NOT EXIST $(PROJ_OBJ_DIR) MD $(PROJ_OBJ_DIR)
	@IF NOT EXIST $(PROJ_OBJ_DIR)$(BUILD_NAME)\
		MD $(PROJ_OBJ_DIR)$(BUILD_NAME)
	@IF NOT EXIST $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\
		MD $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)
!IF $(PCH)==1
	@REM PDB and IDB should be derived from the PCH versions.
	@IF NOT EXIST $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(BIN_NAME).pdb\
		CP $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\pch.pdb\
			$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(BIN_NAME).pdb
	@IF NOT EXIST $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(BIN_NAME).idb\
		IF EXIST $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\pch.idb\
			CP $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\pch.idb\
				$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(BIN_NAME).idb
!ENDIF
	@REM Compile all subdirectories separately so as to place
	@REM object files in the correct relative subdirectory.
	@REM We use . as a placeholder for wildcards in the
	@REM project source root.
	@REM The else case exploits behavior that we can use ..\
	@REM to ignore the last part of a path in `MD`.
	@FOR %%I IN ($(patsubsti %*.obj,"%",\
		$(patsubsti %.cpp,%.obj,\
			$(patsubsti $(PROJ_SRC_DIR)\%,%,$(PROJ_SRC))))) DO\
		@IF "%%~xI" NEQ ".obj" (\
			$(MAKE) /C $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\%%~I.sentinel.tmp\
				WINDOWS_INNER_ARG=%%~I\
				PROJ=$(PROJ) BIN=$(BIN) SRC="$(SRC)"\
				BUILD=$(BUILD) ARCH=$(ARCH) PCH=$(PCH)\
		) ELSE (\
			$(MAKE) /C $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\%%~I\
				WINDOWS_INNER_ARG=%%~I\
				PROJ=$(PROJ) BIN=$(BIN) SRC="$(SRC)"\
				BUILD=$(BUILD) ARCH=$(ARCH) PCH=$(PCH)\
		)
	@ECHO Linking $(PROJ_BIN)...
	@$(LINKER) $(LINKER_FLAGS) $(LINKER_FLAGS_PCH)\
		$(patsubsti %.cpp,%.obj,\
		$(patsubsti $(PROJ_SRC_DIR)\%,\
			$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\\%,\
			$**))

# Secondary build target for subdirectories. We are able to use nmake macros again via the recursive call, and also avoid rebuilding unchanged source files.
# Use a sentinel in the .obj directory to mark the directory as up-to-date. Without the sentinel and using a fake target instead, all dependents are considered out of date.
$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(WINDOWS_INNER_ARG).sentinel.tmp: $(PROJ_SRC_DIR)$(WINDOWS_INNER_ARG)*.cpp
	@IF NOT EXIST "$@\..\" MD "$@\..\"
	@ECHO Building directory $(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(WINDOWS_INNER_ARG)*.obj...
	@$(COMPILER) $(COMPILER_FLAGS) $(COMPILER_FLAGS_PCH)\
		/Fd$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(BIN_NAME).pdb\
		/Fo$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(WINDOWS_INNER_ARG) \
		$?
	@TYPE NUL>$@
# Secondary build target for files. Need ths proxy variable or the dependency line results in a syntax error.
WINDOWS_INNER_BUILD_FILE_DEP=$(PROJ_SRC_DIR)$(patsubsti %.obj,%.cpp,$(WINDOWS_INNER_ARG))
$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(WINDOWS_INNER_ARG): $(WINDOWS_INNER_BUILD_FILE_DEP)
	@IF NOT EXIST "$@\..\" MD "$@\..\"
	@ECHO Building file $@...
	@$(COMPILER) $(COMPILER_FLAGS) $(COMPILER_FLAGS_PCH)\
		/Fd$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(BIN_NAME).pdb\
		/Fo$(PROJ_OBJ_DIR)$(BUILD_NAME)\x$(ARCH)\$(WINDOWS_INNER_ARG)\..\ \
		$?

# Mark source files as out of date if any headers have
# changed.
# An empty target here will correctly set the modified date.
$(PROJ_SRC): $(INCL) pch.cpp
	@EXIT

# ----------------------------------------------------------
!else
else
# ----------------------------------------------------------
# GNU make (gmake) specific commands.

# Second expansion for stem matching syntax.
.SECONDEXPANSION:
# For usage of bash commands like `read`.
SHELL:=/bin/bash
# For recursive dependency listing. Directories must end
# with /.
rwildcard=$(foreach d,\
	$(wildcard $(1:%=%*)),\
	$(filter $(subst *,%,$2),$d) $(call rwildcard,$d/,$2))

ROOT_DIR:=../
REPO_NAME:=fuscate
PROJ?=src
PROJ_NAME:=$(PROJ)
ifndef BIN
ifndef SRC
BIN:=$(REPO_NAME)
SRC:=*.cpp
else
BIN:=$(patsubst .cpp,,$(SRC))
endif
else
SRC:=$(BIN).cpp
endif
BIN_NAME:=$(BIN)
SRC_NAME:=$(SRC)
BUILD?=0
ifeq ($(BUILD),2)
BUILD_NAME:=instrument
else ifeq ($(BUILD),1)
BUILD_NAME:=release
else
BUILD_NAME:=debug
endif
ARCH?=64
PCH?=1

INCL_PCH:=$(call rwildcard,$(ROOT_DIR)rain/include/,*.hpp)
INCL:=$(INCL_PCH)\
	$(call rwildcard,$(ROOT_DIR)include/,*.hpp)

BIN_DIR:=$(ROOT_DIR).bin/
PROJ_BIN:=$(BIN_DIR)$(PROJ_NAME)/$(BIN_NAME).$(BUILD_NAME).x$(ARCH)
PROJ_BIN_DIR:=$(BIN_DIR)$(PROJ_NAME)/
OBJ_DIR:=$(ROOT_DIR).obj/
PROJ_OBJ_DIR:=$(OBJ_DIR)$(PROJ_NAME)/
PROJ_SRC_DIR:=$(ROOT_DIR)$(PROJ_NAME)/
ifeq ($(SRC_NAME),*.cpp)
PROJ_SRC:=$(call rwildcard,$(PROJ_SRC_DIR),*.cpp)
else
PROJ_SRC:=$(patsubst %,$(PROJ_SRC_DIR)%,$(SRC_NAME))
endif

COMPILER?=clang++
COMPILER_FLAGS_INCLUDES:=-I$(ROOT_DIR)rain/include\
	-I$(ROOT_DIR)include
COMPILER_FLAGS_COMMON:=$(COMPILER_FLAGS_INCLUDES)\
	-std=c++20 -pthread\
	-Wall -Wextra -D_CONSOLE -masm=intel
COMPILER_FLAGS_DEBUG:=-D_DEBUG -g -O0
COMPILER_FLAGS_RELEASE:=-DNDEBUG -O3 -ffast-math
COMPILER_FLAGS_INSTRUMENT:=$(COMPILER_FLAGS_DEBUG)\
	/Fa$(PROJ_OBJ_DIR)
COMPILER_FLAGS_64:=-m64
COMPILER_FLAGS_86:=-m32

LINKER:=$(COMPILER)
LINKER_FLAGS_LIBRARIES:=-pthread -lresolv
LINKER_FLAGS_COMMON:=$(LINKER_FLAGS_LIBRARIES)
LINKER_FLAGS_DEBUG:=
LINKER_FLAGS_RELEASE:=
LINKER_FLAGS_INSTRUMENT:=$(LINKER_FLAGS_DEBUG)\
	-fsanitize=address,undefined,leak
LINKER_FLAGS_64:=-m64
LINKER_FLAGS_86:=-m32

# -lstdc++fs is neither available nor required on typical
# MacOS C++ compiler implementations.
ifneq ($(OS),Darwin)
	LINKER_FLAGS_LIBRARIES:=$(LINKER_FLAGS_LIBRARIES)\
		-lstdc++fs
endif

ifeq ($(BUILD_NAME),instrument)
	COMPILER_FLAGS:=$(COMPILER_FLAGS_COMMON)\
		$(COMPILER_FLAGS_INSTRUMENT)
	LINKER_FLAGS:=$(LINKER_FLAGS_COMMON)\
		$(LINKER_FLAGS_INSTRUMENT)
else ifeq ($(BUILD_NAME),release)
	COMPILER_FLAGS:=$(COMPILER_FLAGS_COMMON)\
		$(COMPILER_FLAGS_RELEASE)
	LINKER_FLAGS:=$(LINKER_FLAGS_COMMON)\
		$(LINKER_FLAGS_RELEASE)
else
	COMPILER_FLAGS:=$(COMPILER_FLAGS_COMMON)\
		$(COMPILER_FLAGS_DEBUG)
	LINKER_FLAGS:=$(LINKER_FLAGS_COMMON) $(LINKER_FLAGS_DEBUG)
endif

ifeq ($(ARCH),86)
	COMPILER_FLAGS:=$(COMPILER_FLAGS) $(COMPILER_FLAGS_86)
	LINKER_FLAGS:=$(LINKER_FLAGS) $(LINKER_FLAGS_86)
else
	COMPILER_FLAGS:=$(COMPILER_FLAGS) $(COMPILER_FLAGS_64)
	LINKER_FLAGS:=$(LINKER_FLAGS) $(LINKER_FLAGS_64)
endif
ifeq ($(PCH),1)
	PCH_PCH=$(PROJ_OBJ_DIR)$(BUILD_NAME)/x$(ARCH)/pch.pch
	COMPILER_FLAGS_PCH=-include-pch\
		$(PROJ_OBJ_DIR)$(BUILD_NAME)/x$(ARCH)/pch.pch
else
	PCH_PCH=
	COMPILER_FLAGS_PCH=
endif

# Unlike `nmake`, `gmake` first compiles all the relevant
# obj files, then compiles the relevant binaries.
# Keep intermediate and output files.
# Introduce list of project object intermediates parsed
# from the source directory with SRC_NAME.
PROJ_OBJ:=$(foreach s,$(PROJ_SRC),\
	$(addprefix $(PROJ_OBJ_DIR)$(BUILD_NAME)/x$(ARCH)/,\
		$(addsuffix .o,\
			$(s:$(PROJ_SRC_DIR)%.cpp=%))))
.SECONDARY:$(PROJ_BIN) $(PROJ_OBJ)

# Ensure files with the same name as fake targets do not
# trigger.
.PHONY: __default _default build run clean info
_default: build
build: version.build.txt $(PROJ_BIN)
run: build
	@echo ---------------- [Run] $(BIN_NAME) ----------------
	@cd $(PROJ_BIN_DIR) &&\
		./$(BIN_NAME).$(BUILD_NAME).x$(ARCH) $(ARGS)
clean:
	@echo Cleaning...
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
info:
	@echo INCL_PCH=$(INCL_PCH)
	@echo INCL=$(INCL)
	@echo PROJ_SRC_WC=$(PROJ_SRC_WC)
	@echo MAKEFLAGS=$(MAKEFLAGS)
	@echo BIN_NAME=$(BIN_NAME)
	@echo SRC_NAME=$(SRC_NAME)
	@echo BUILD_NAME=$(BUILD_NAME)
	@echo ARCH=$(ARCH)
	@echo PROJ_SRC=$(PROJ_SRC)
	@echo PROJ_OBJ=$(PROJ_OBJ)
	@echo COMPILER=$(COMPILER)
	@echo COMPILER_FLAGS=$(COMPILER_FLAGS)
	@echo LINKER_FLAGS=$(LINKER_FLAGS)

version.build.txt: $(INCL) $(PROJ_SRC)
	@for i in $?; do echo Outdated: $$i.;\
	done
	@./version.sh $(shell \
		echo $(REPO_NAME) | tr '[:lower:]' '[:upper:]')
$(PROJ_OBJ_DIR)$(BUILD_NAME)/x$(ARCH)/pch.pch: pch.cpp
	@echo Building $@...
	@mkdir -p $(PROJ_OBJ_DIR)$(BUILD_NAME)/x$(ARCH)
	@$(COMPILER) $(COMPILER_FLAGS) -xc++-header pch.cpp -o $@
pch.cpp: $(INCL_PCH)

# Compile rule. The first line includes all header
# dependencies.
# Stem % matches source file name.
$(PROJ_OBJ): $(INCL) $(PCH_PCH)
$(PROJ_OBJ): $(PROJ_OBJ_DIR)$(BUILD_NAME)/x$(ARCH)/%.o : $(PROJ_SRC_DIR)$$*.cpp
	@mkdir -p $(@D)
	@echo Building $@...
	@$(COMPILER) $(COMPILER_FLAGS) $(COMPILER_FLAGS_PCH)\
		-c $< -o $@

# Link rule.
$(PROJ_BIN): $(BIN_DIR)% : $(PROJ_OBJ)
	@mkdir -p $(@D)
	@echo Linking $@...
	@$(LINKER) $(PROJ_OBJ) $(LINKER_FLAGS) -o $@

# ----------------------------------------------------------
endif    # gmake: close condition; nmake: not seen
!endif : # gmake: unused target; nmake close conditional
# ----------------------------------------------------------
