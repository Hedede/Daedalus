#
# Copyright (C) 2015  hedede <haddayn@gmail.com>
#
# License LGPLv3 or later:
# GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
# This is free software: you are free to change and redistribute it.
# There is NO WARRANTY, to the extent permitted by law.

# All project makefiles must define:
# ProjectName
# RootPath
# Objects
# Executable


# Global configuration
Version = 0

EXTRAFLAGS=

# Makefile tricks
comma = ,
space :=
space +=

# Per-project settings
ifeq ($(Executable),true)
	InstallDir = $(RootPath)/bin
	OutputName = $(ProjectName)
else
	InstallDir = $(RootPath)/lib
	OutputShortName = lib$(ProjectName).so
	OutputName = $(OutputShortName).$(Version)
	EXTRAFLAGS += -shared 
endif
BuildDir = $(RootPath)/build/$(ProjectName)
Includes = -I$(RootPath)/include
Objects = $(patsubst %.cpp, $(BuildDir)/%.o, $(Sources))
Depends = $(Objects:.o=.d)
ProjectDefines = $(addprefix -D,$(Defines))
ProjectDependencies = $(addprefix -l,$(Libraries))


# User configuration
include $(RootPath)/Config.mk

ExtraIncludePaths = $(addprefix -I$(RootPath)/,$(CONFIG_INCLUDE_PATHS))
ExtraLibraryPaths = $(addprefix -L$(RootPath)/,$(CONFIG_LIBRARY_PATHS))

# Tool configuration
MKDIR_P = mkdir -p
ECHO = @echo

CXXFLAGS  = -std=c++14
CXXFLAGS += -fPIC
CXXFLAGS += -fno-exceptions
CXXFLAGS += -fvisibility=default
CXXFLAGS += -fdiagnostics-color=auto
CXXFLAGS_DEBUG   = -g -DDEBUG -D_DEBUG
CXXFLAGS_RELEASE = -O3 -DNDEBUG

CCFLAGS  = -std=c11
CPPFLAGS = $(ProjectDefines) $(Includes) $(ExtraIncludePaths)
LDFLAGS  = -Wl,-rpath-link,$(RootPath)/lib,-R,'$$ORIGIN/../lib' -L$(RootPath)/lib
LDFLAGS += $(ExtraLibraryPaths)
LDFLAGS += $(ProjectDependencies)

# Generate dependency files
ifeq ($(CONFIG_MAKE_DEPENDS),true)
CCFLAGS  += -MMD -MP
CXXFLAGS += -MMD -MP
endif

# Colors
PRINTF = @printf
PRINTF_BOLD=$(PRINTF) '\033[1m'
PRINTF_RED=$(PRINTF) '\033[31m'
PRINTF_RESET=$(PRINTF) '\033[0m'

# Build rules
all: debug install

.PHONY: debug
debug: CXXFLAGS+=$(CXXFLAGS_DEBUG)
debug: Build

.PHONY: release  
release: CXXFLAGS+=$(CXXFLAGS_RELEASE)
release: Build

$(BuildDir)/%.o: %.cpp
	$(PRINTF_BOLD)
	$(ECHO) [Build] Compiling $@
	$(PRINTF_RESET)
	@ $(MKDIR_P) $(dir $@)
	@ $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

Build: $(Objects)
	@ $(MKDIR_P) $(BuildDir)
	$(PRINTF_BOLD)
	$(ECHO) [Build] Linking object files.
	$(PRINTF_RESET)
	@ $(CXX) $(EXTRAFLAGS) -o $(BuildDir)/$(OutputName) \
	$(CPPFLAGS) $(CXXFLAGS) $(Objects) $(LDFLAGS)
	$(PRINTF_BOLD)
	$(ECHO) [Build] Done.
	$(PRINTF_RESET)

install: Build
	$(PRINTF_BOLD)
	$(ECHO) [Install] Copying $(OutputName)
	$(PRINTF_RESET)
	@ $(MKDIR_P) $(InstallDir)
	@ cp $(BuildDir)/$(OutputName) $(InstallDir)/$(OutputName)
ifeq ($(Executable),false)
	$(PRINTF_BOLD)
	$(ECHO) [Install] Creating symlink $(OutputShortName) to $(OutputName).
	$(PRINTF_RESET)
	@ ln -sf $(OutputName) $(InstallDir)/$(OutputShortName)
endif
	$(PRINTF_BOLD)
	$(ECHO) [Install] Done.
	$(PRINTF_RESET)


.PHONY : clean
clean: 
	$(PRINTF_BOLD)
	$(ECHO) [Clean] Removing build files
	$(PRINTF_RESET)
	@ rm -f $(Objects) $(BuildDir)/$(OutputName)
	$(PRINTF_BOLD)
	$(ECHO) [Done] Removing build files
	$(PRINTF_RESET)

ifeq ($(CONFIG_MAKE_DEPENDS),true)
-include $(Depends)
endif
