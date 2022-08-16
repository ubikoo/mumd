SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/atto/opencl/_*.cpp), \
                         $(wildcard $(ROOTDIR)/atto/opencl/*.cpp)) \
            $(filter-out $(wildcard $(ROOTDIR)/atto/opencl/_*.c), \
                         $(wildcard $(ROOTDIR)/atto/opencl/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/atto/opencl/*.hpp) \
            $(wildcard $(ROOTDIR)/atto/opencl/*.h)

CFLAGS   += -I$(ROOTDIR)
