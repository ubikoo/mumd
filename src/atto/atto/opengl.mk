SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/atto/opengl/_*.cpp), \
                         $(wildcard $(ROOTDIR)/atto/opengl/*.cpp)) \
            $(filter-out $(wildcard $(ROOTDIR)/atto/opengl/_*.c), \
                         $(wildcard $(ROOTDIR)/atto/opengl/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/atto/opengl/*.hpp) \
            $(wildcard $(ROOTDIR)/atto/opengl/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/atto/opengl/glsl/_*.cpp), \
                         $(wildcard $(ROOTDIR)/atto/opengl/glsl/*.cpp)) \
            $(filter-out $(wildcard $(ROOTDIR)/atto/opengl/glsl/_*.c), \
                         $(wildcard $(ROOTDIR)/atto/opengl/glsl/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/atto/opengl/glsl/*.hpp) \
            $(wildcard $(ROOTDIR)/atto/opengl/glsl/*.h)

SOURCES  += $(filter-out $(wildcard $(ROOTDIR)/atto/opengl/graphics/_*.cpp), \
                         $(wildcard $(ROOTDIR)/atto/opengl/graphics/*.cpp)) \
            $(filter-out $(wildcard $(ROOTDIR)/atto/opengl/graphics/_*.c), \
                         $(wildcard $(ROOTDIR)/atto/opengl/graphics/*.c))
INCLUDES += $(wildcard $(ROOTDIR)/atto/opengl/graphics/*.hpp) \
            $(wildcard $(ROOTDIR)/atto/opengl/graphics/*.h)

CFLAGS   += -I$(ROOTDIR)