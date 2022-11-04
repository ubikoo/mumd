# -----------------------------------------------------------------------------
# Set the target macro
ifeq ($(origin target), undefined)
TARGET := test
else ifeq ($(target),)
TARGET := test
else
TARGET := $(target)
endif

# Project macros and dag description
BINARY   := $(join $(TARGET),.out)

# Source files, include files and search paths
SOURCES  := $(filter-out $(wildcard _*.cpp), $(wildcard *.cpp)) \
			$(filter-out $(wildcard _*.c), $(wildcard *.c))
INCLUDES := $(wildcard *.hpp) $(wildcard *.h)
CFLAGS   := -I.

# -----------------------------------------------------------------------------
# Template module file makefile.mk
# 	SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/source/_*.c), \
#   	                     $(wildcard $(ROOTDIR)/source/*.c))
# 	INCLUDES += $(wildcard $(ROOTDIR)/source/*.h)
# 	CFLAGS   += -I$(ROOTDIR)/include
#
#	SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/source/_*.cpp), \
#   	                     $(wildcard $(ROOTDIR)/source/*.cpp)) \
#       	    $(filter-out $(wildcard $(ROOTDIR)/source/_*.c), \
#           	             $(wildcard $(ROOTDIR)/source/*.c))
#	INCLUDES += $(wildcard $(ROOTDIR)/include/*.hpp) \
#               $(wildcard $(ROOTDIR)/include/*.h)
#	CFLAGS   += -I$(ROOTDIR)/include
#
# Include modules
ROOTDIR	 := ../../..
include $(ROOTDIR)/atto/core.mk

# Catch2 module
ROOTDIR	 := ../../../3rdparty
CFLAGS   += -I$(ROOTDIR)

# ----------------------------------------------------------------------------
# Objects and dependencies
CXX_SOURCES := $(filter %.cpp,$(SOURCES))
CXX_OBJECTS := $(patsubst %.cpp,%.o,$(CXX_SOURCES))
CXX_DEPENDS := $(patsubst %.cpp,%.d,$(CXX_SOURCES))

C_SOURCES   := $(filter %.c,$(SOURCES))
C_OBJECTS  	:= $(patsubst %.c,%.o,$(C_SOURCES))
C_DEPENDS  	:= $(patsubst %.c,%.d,$(C_SOURCES))

OBJECTS     := $(C_OBJECTS) $(CXX_OBJECTS)
DEPENDS     := $(C_DEPENDS) $(CXX_DEPENDS)

# -----------------------------------------------------------------------------
# Compiler settings
AR      := ar rcs
RM      := rm -vf
CP      := cp -vf
WC      := wc
TAR     := tar
AWK     := gawk
ECHO    := echo
INSTALL := install
SHELL	:= bash
UNAME   := $(shell uname -s)

# Darwin kernel flags
ifeq ($(UNAME), Darwin)
CC      := g++-mp-9

CFLAGS  += -march=native -mavx -Wa,-q
CFLAGS  += -I/opt/local/include -Wall -std=c++14
CFLAGS  += -fPIC -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS  += $(shell pkg-config --cflags glfw3)
CFLAGS  += $(shell pkg-config --cflags assimp)

LDFLAGS += -L/opt/local/lib -Wl,-rpath,/opt/local/lib -lm
LDFLAGS += -Wl,-framework,OpenGL
LDFLAGS += -Wl,-framework,OpenCL
LDFLAGS += $(shell pkg-config --libs glfw3)
LDFLAGS += $(shell pkg-config --libs assimp)
endif

# Linux kernel flags
ifeq ($(UNAME), Linux)
CC      := g++

CFLAGS  += -march=native -mavx
CFLAGS  += -I/usr/include -Wall -std=c++14
CFLAGS  += -fPIC -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS  += $(shell pkg-config --cflags glfw3)
CFLAGS  += $(shell pkg-config --cflags assimp)

LDFLAGS += -L/usr/lib64 -Wl,-rpath,/usr/lib64 -lm
LDFLAGS += -lGL -lGLU -lOpenCL
LDFLAGS += $(shell pkg-config --libs glfw3)
LDFLAGS += $(shell pkg-config --libs assimp)
endif

# Enable/disable debug flags
# CFLAGS  += -g -ggdb -O0 -pedantic -fopt-info-vec-optimized
CFLAGS  += -Ofast

# Enable/disable OpenMP flags
CFLAGS  += -fopenmp
LDFLAGS += -fopenmp

# Enable/disable Pthreads flags
# CFLAGS  += -pthread
# LDFLAGS += -pthread

# -----------------------------------------------------------------------------
# Target rules

## help: Show this message.
#	@sed -n 's/^##//p' $(1)
define makehelp
	@$(AWK) \
		'BEGIN \
		{ \
			printf("\nusage: make [\033[0;36mtarget\033[0m]\n"); \
		} \
		{ \
			if ($$1 == "##") { \
				printf("\033[0;36m %-16s \033[0m", $$2); \
				for (i=3; i<=NF; i++) printf("%s ", $$i);\
				printf "\n"; \
			} \
		} \
		END \
		{ \
			printf "\n"; \
		}' < $(1)
endef

.DEFAULT_GOAL := help
.PHONY: help
help: $(firstword $(MAKEFILE_LIST))
	$(call makehelp,$<)

## count: Count number of lines.
.PHONY: count
count:
	$(WC) $(SOURCES) $(INCLUDES)

## all: Build all targets.
.PHONY: all
all: bin

## clean: Remove auto generated files.
.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(BINARY)

## bin: Build the binary program.
.PHONY: bin
bin: $(BINARY)

# -----------------------------------------------------------------------------
# Binary and static library
$(BINARY): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(BINARY)

# Objects and dependencies
define makedep
	$(eval SRCFILE := $(1))
	$(eval DEPFILE := $(2))
	$(eval DEPDIR  := $(3))
	@if [[ "$(DEPDIR)" == "." ]] || [[ "x$(DEPDIR)" == "x" ]]; \
	then \
	$(CC) -MM -MG $(CFLAGS) $(SRCFILE) | sed -e 's#^\(.*\)\.o:#\1.o:#g' > $(DEPFILE); \
	else \
	$(CC) -MM -MG $(CFLAGS) $(SRCFILE) | sed -e 's#^\(.*\)\.o:#$(DEPDIR)/\1.o:#g' > $(DEPFILE); \
	fi;
endef

$(CXX_OBJECTS): %.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	$(call makedep,$<,$(patsubst %cpp,%d,$<),$(shell dirname $<))

$(C_OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(call makedep,$<,$(patsubst %c,%d,$<),$(shell dirname $<))

-include $(DEPENDS)
