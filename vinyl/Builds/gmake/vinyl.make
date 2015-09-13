# GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_4klang
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild prelink

ifeq ($(config),debug_4klang)
  RESCOMP = windres
  TARGETDIR = bin/4klang/Debug
  TARGET = $(TARGETDIR)/rt
  OBJDIR = obj/4klang/Debug
  DEFINES += -D__linux -D_DEBUG -DSYNTH_4KLANG
  INCLUDES += -I../../Lib/include
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -g -O0 -std=c++11 -Wall -Wextra -m32
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CFLAGS)
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -lGL -lX11 -lasound ../../Source/music/4klang.linux.o
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L../../Lib/lib -L../../Source/music -pthread -m32
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),debug_vorbis)
  RESCOMP = windres
  TARGETDIR = bin/vorbis/Debug
  TARGET = $(TARGETDIR)/rt
  OBJDIR = obj/vorbis/Debug
  DEFINES += -D__linux -D_DEBUG -DSYNTH_VORBIS -DSTB_VORBIS_NOPUSHDATA_API -DSTB_VORBIS_NO_STDIO
  INCLUDES += -I../../Lib/include
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -g -O0 -std=c++11 -Wall -Wextra
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CFLAGS)
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -lGL -lX11 -lasound
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L../../Lib/lib -L../../Source/music -pthread
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),release_4klang)
  RESCOMP = windres
  TARGETDIR = bin/4klang/Release
  TARGET = $(TARGETDIR)/rt
  OBJDIR = obj/4klang/Release
  DEFINES += -D__linux -DNDEBUG -DSYNTH_4KLANG
  INCLUDES += -I../../Lib/include
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Os -std=c++11 -Wall -Wextra -m32
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CFLAGS)
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -lGL -lX11 -lasound ../../Source/music/4klang.linux.o
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L../../Lib/lib -L../../Source/music -s -pthread -m32
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),release_vorbis)
  RESCOMP = windres
  TARGETDIR = bin/vorbis/Release
  TARGET = $(TARGETDIR)/rt
  OBJDIR = obj/vorbis/Release
  DEFINES += -D__linux -DNDEBUG -DSYNTH_VORBIS -DSTB_VORBIS_NOPUSHDATA_API -DSTB_VORBIS_NO_STDIO
  INCLUDES += -I../../Lib/include
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Os -std=c++11 -Wall -Wextra
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CFLAGS)
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += -lGL -lX11 -lasound
  LDDEPS +=
  ALL_LDFLAGS += $(LDFLAGS) -L../../Lib/lib -L../../Source/music -s -pthread
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

endif

OBJECTS := \
	$(OBJDIR)/AmbientLight.o \
	$(OBJDIR)/BRDFLut.o \
	$(OBJDIR)/DataInterpolator.o \
	$(OBJDIR)/EnvironmentMap.o \
	$(OBJDIR)/Framebuffer.o \
	$(OBJDIR)/LinuxFrontend.o \
	$(OBJDIR)/Shader.o \
	$(OBJDIR)/main.o \
	$(OBJDIR)/quat.o \
	$(OBJDIR)/scalar.o \
	$(OBJDIR)/stdmath.o \
	$(OBJDIR)/vec2.o \
	$(OBJDIR)/vec3.o \
	$(OBJDIR)/vec4.o \
	$(OBJDIR)/stdlib.o \

RESOURCES := \

CUSTOMFILES := \

ifeq ($(config),debug_vorbis)
  OBJECTS += \
	$(OBJDIR)/stb_vorbis_wrapper.o \
	obj/vorbis/Debug/incbin.o \

endif

ifeq ($(config),release_vorbis)
  OBJECTS += \
	$(OBJDIR)/stb_vorbis_wrapper.o \
	obj/vorbis/Release/incbin.o \

endif

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif

$(TARGET): $(GCH) $(OBJECTS) $(LDDEPS) $(RESOURCES) ${CUSTOMFILES}
	@echo Linking vinyl
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning vinyl
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) $(PCH)
$(GCH): $(PCH)
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
endif

$(OBJDIR)/stb_vorbis_wrapper.o: ../../Lib/include/stb_vorbis_wrapper.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AmbientLight.o: ../../Source/AmbientLight.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/BRDFLut.o: ../../Source/BRDFLut.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/DataInterpolator.o: ../../Source/DataInterpolator.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/EnvironmentMap.o: ../../Source/EnvironmentMap.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Framebuffer.o: ../../Source/Framebuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/LinuxFrontend.o: ../../Source/LinuxFrontend.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Shader.o: ../../Source/Shader.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
ifeq ($(config),debug_vorbis)
obj/vorbis/Debug/incbin.o: ../../Source/incbin.asm
	@echo "incbin.asm"
	$(SILENT) nasm -f elf64 -o obj/vorbis/Debug/incbin.o ../../Source/incbin.asm -D__linux -DSYNTH_VORBIS
endif
ifeq ($(config),release_vorbis)
obj/vorbis/Release/incbin.o: ../../Source/incbin.asm
	@echo "incbin.asm"
	$(SILENT) nasm -f elf64 -o obj/vorbis/Release/incbin.o ../../Source/incbin.asm -D__linux -DNDEBUG -DSYNTH_VORBIS
endif
$(OBJDIR)/main.o: ../../Source/main.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/quat.o: ../../Source/math/quat.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/scalar.o: ../../Source/math/scalar.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/stdmath.o: ../../Source/math/stdmath.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/vec2.o: ../../Source/math/vec2.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/vec3.o: ../../Source/math/vec3.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/vec4.o: ../../Source/math/vec4.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/stdlib.o: ../../Source/stdlib.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
endif