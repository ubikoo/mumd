SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/atto/core/_*.cpp), \
                         $(wildcard $(ROOTDIR)/atto/core/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/atto/core/_*.c), \
                         $(wildcard $(ROOTDIR)/atto/core/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/atto/core/*.hpp) \
			$(wildcard $(ROOTDIR)/atto/core/*.h)

CFLAGS   += -I$(ROOTDIR)
