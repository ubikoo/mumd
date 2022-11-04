SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/atto/math/_*.cpp), \
                         $(wildcard $(ROOTDIR)/atto/math/*.cpp)) \
			$(filter-out $(wildcard $(ROOTDIR)/atto/math/_*.c), \
                         $(wildcard $(ROOTDIR)/atto/math/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/atto/math/*.hpp) \
			$(wildcard $(ROOTDIR)/atto/math/*.h)

CFLAGS   += -I$(ROOTDIR)
