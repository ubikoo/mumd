# -----------------------------------------------------------------------------
# Set the target macro
ifeq ($(origin target), undefined)
TARGET := ent
else ifeq ($(target),)
TARGET := foo
else
TARGET := $(target)
endif

# Project macros and dag description
BINARY   := $(join $(TARGET),.out)

SOURCES  := $(filter-out $(wildcard _*.c), $(wildcard *.c))
INCLUDES := $(wildcard *.h)
CFLAGS   := -I.

# Objects and dependencies
OBJECTS  := $(patsubst %.c,%.o,$(SOURCES))
DEPENDS  := $(patsubst %.c,%.d,$(SOURCES))

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
CC      := gcc-mp-9
SWIG    := /opt/local/bin/swig -python -I/opt/local/include -I/include

CFLAGS  += -march=native -mavx -Wa,-q
CFLAGS  += -I/opt/local/include -Wall -std=c99
CFLAGS  += -fPIC -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

LDFLAGS += -L/opt/local/lib -Wl,-rpath,/opt/local/lib -lm
endif


# Linux kernel flags
ifeq ($(UNAME), Linux)
CC      := gcc
SWIG    := /usr/bin/swig -python -I/usr/include -I/include

CFLAGS  += -march=native -mavx
CFLAGS  += -I/usr/include -Wall -std=c99
CFLAGS  += -fPIC -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

LDFLAGS += -L/usr/lib64 -Wl,-rpath,/usr/lib64 -lm
endif

# Enable Debug flags
ifeq ($(origin debug), undefined)
DEBUG := no
else ifeq ($(debug),)
DEBUG := no
else
DEBUG := $(debug)
endif

ifeq ($(strip $(DEBUG)),yes)
CFLAGS  += -g -ggdb -O1 -pedantic -fopt-info-vec-optimized
else
CFLAGS  += -Ofast
endif

# Enable OpenMP flags
ifeq ($(origin omp), undefined)
OPENMP := yes
else ifeq ($(omp),)
OPENMP := yes
else
OPENMP := $(omp)
endif

ifeq ($(strip $(OPENMP)),yes)
CFLAGS  += -fopenmp
LDFLAGS += -fopenmp
endif

# -----------------------------------------------------------------------------
# Target rules

## help: Show this message.
#	@sed -n 's/^##//p' $(1)
define makehelp
	@$(AWK) \
		'BEGIN \
		{ \
			printf("\nusage: make [\033[0;36mtarget\033[0m]\n\n"); \
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
			printf("\nOptional Features:\n\n"); \
			printf("\033[0;36m %-16s \033[0mSet target name (default=%s).\n", \
					"target=[arg]", "$(TARGET)"); \
			printf("\033[0;36m %-16s \033[0mEnable debug (default=%s).\n", \
					"debug=[yes|no]", "no"); \
			printf("\033[0;36m %-16s \033[0mEnable openmp (default=%s).\n", \
					"omp=[yes|no]", "yes"); \
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

## all: Build all targets, same as 'make bin lib libpy'.
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
	$(eval DEPDIR := $(shell dirname $(1)))
	$(eval DEPNAME := $(patsubst %c,%d,$(1)))
	@if [[ "$(DEPDIR)" == "." ]] || [[ "x$(DEPDIR)" == "x" ]]; \
	then \
	$(CC) -MM -MG $(CFLAGS) $(1) | sed -e 's#^\(.*\)\.o:#\1.o:#g' > $(DEPNAME); \
	else \
	$(CC) -MM -MG $(CFLAGS) $(1) | sed -e 's#^\(.*\)\.o:#$(DEPDIR)/\1.o:#g' > $(DEPNAME); \
	fi;
endef

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(call makedep,$<)

-include $(DEPENDS)
