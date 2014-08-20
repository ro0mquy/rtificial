# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCE_DONT_ASSERT_ON_GLSL_COMPILE_ERROR" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION" -D "JUCE_APP_VERSION_HEX=0x0" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../juce_modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 -std=c++11 -Wall -I../../Source
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCE_DONT_ASSERT_ON_GLSL_COMPILE_ERROR" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION" -D "JUCE_APP_VERSION_HEX=0x0" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../juce_modules
  TARGET := strahlenwerk
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCE_DONT_ASSERT_ON_GLSL_COMPILE_ERROR" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION" -D "JUCE_APP_VERSION_HEX=0x0" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../juce_modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O3 -std=c++11 -Wall -I../../Source
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCE_DONT_ASSERT_ON_GLSL_COMPILE_ERROR" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION" -D "JUCE_APP_VERSION_HEX=0x0" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../juce_modules
  TARGET := strahlenwerk
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/Project_cc5166fa.o \
  $(OBJDIR)/LabColorPropertyComponent_eaa105f5.o \
  $(OBJDIR)/ColorSpaceMarker_454a1924.o \
  $(OBJDIR)/LabColor_145742fd.o \
  $(OBJDIR)/LabColorPicker_2909daab.o \
  $(OBJDIR)/LabColorSpaceView_bf2be298.o \
  $(OBJDIR)/ProjectFileLoader_a9547609.o \
  $(OBJDIR)/PostprocPipelineLoader_51edf977.o \
  $(OBJDIR)/PostprocPipeline_473f2604.o \
  $(OBJDIR)/PostprocShader_df5d2107.o \
  $(OBJDIR)/Shader_cfe69b71.o \
  $(OBJDIR)/SceneShader_b92468cf.o \
  $(OBJDIR)/Uniform_96b281f2.o \
  $(OBJDIR)/UniformManager_63433de5.o \
  $(OBJDIR)/ValueEditorComponent_1390aa6e.o \
  $(OBJDIR)/SequenceComponent_10a22e57.o \
  $(OBJDIR)/UniformsBarComponent_d6185a18.o \
  $(OBJDIR)/SceneComponent_2e62f260.o \
  $(OBJDIR)/SnapToGridConstrainer_30f5e134.o \
  $(OBJDIR)/ScenesBarComponent_9e1694c0.o \
  $(OBJDIR)/Data_92ef9439.o \
  $(OBJDIR)/SequenceViewComponent_a0963d32.o \
  $(OBJDIR)/Timeline_d3785850.o \
  $(OBJDIR)/RtificialLookAndFeel_95d2050c.o \
  $(OBJDIR)/MainWindow_499ac812.o \
  $(OBJDIR)/MainComponent_a6ffb4a5.o \
  $(OBJDIR)/Renderer_d34861cc.o \
  $(OBJDIR)/StrahlenwerkApplication_a618e10d.o \
  $(OBJDIR)/OpenGLComponent_fe08fa6f.o \
  $(OBJDIR)/juce_audio_basics_832d1cec.o \
  $(OBJDIR)/juce_audio_devices_f204e6aa.o \
  $(OBJDIR)/juce_audio_formats_2086304a.o \
  $(OBJDIR)/juce_core_204fdce.o \
  $(OBJDIR)/juce_data_structures_8c03830a.o \
  $(OBJDIR)/juce_events_7627d342.o \
  $(OBJDIR)/juce_graphics_7b4d41a6.o \
  $(OBJDIR)/juce_gui_basics_a76b6622.o \
  $(OBJDIR)/juce_gui_extra_54b82a.o \
  $(OBJDIR)/juce_opengl_366ffb6e.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking strahlenwerk
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning strahlenwerk
	@$(CLEANCMD)

strip:
	@echo Stripping strahlenwerk
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/Project_cc5166fa.o: ../../Source/Project.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Project.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LabColorPropertyComponent_eaa105f5.o: ../../Source/ColorPicker/LabColorPropertyComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LabColorPropertyComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ColorSpaceMarker_454a1924.o: ../../Source/ColorPicker/ColorSpaceMarker.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ColorSpaceMarker.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LabColor_145742fd.o: ../../Source/ColorPicker/LabColor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LabColor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LabColorPicker_2909daab.o: ../../Source/ColorPicker/LabColorPicker.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LabColorPicker.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LabColorSpaceView_bf2be298.o: ../../Source/ColorPicker/LabColorSpaceView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LabColorSpaceView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ProjectFileLoader_a9547609.o: ../../Source/ProjectFileLoader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ProjectFileLoader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PostprocPipelineLoader_51edf977.o: ../../Source/Rendering/PostprocPipelineLoader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PostprocPipelineLoader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PostprocPipeline_473f2604.o: ../../Source/Rendering/PostprocPipeline.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PostprocPipeline.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PostprocShader_df5d2107.o: ../../Source/Rendering/PostprocShader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PostprocShader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Shader_cfe69b71.o: ../../Source/Rendering/Shader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Shader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SceneShader_b92468cf.o: ../../Source/Rendering/SceneShader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SceneShader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Uniform_96b281f2.o: ../../Source/Rendering/Uniform.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Uniform.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UniformManager_63433de5.o: ../../Source/Rendering/UniformManager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UniformManager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ValueEditorComponent_1390aa6e.o: ../../Source/Timeline/ValueEditorComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ValueEditorComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SequenceComponent_10a22e57.o: ../../Source/Timeline/SequenceComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SequenceComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UniformsBarComponent_d6185a18.o: ../../Source/Timeline/UniformsBarComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UniformsBarComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SceneComponent_2e62f260.o: ../../Source/Timeline/SceneComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SceneComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SnapToGridConstrainer_30f5e134.o: ../../Source/Timeline/SnapToGridConstrainer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SnapToGridConstrainer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ScenesBarComponent_9e1694c0.o: ../../Source/Timeline/ScenesBarComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ScenesBarComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Data_92ef9439.o: ../../Source/Timeline/Data.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Data.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SequenceViewComponent_a0963d32.o: ../../Source/Timeline/SequenceViewComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SequenceViewComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Timeline_d3785850.o: ../../Source/Timeline/Timeline.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Timeline.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RtificialLookAndFeel_95d2050c.o: ../../Source/RtificialLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RtificialLookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainWindow_499ac812.o: ../../Source/MainWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Renderer_d34861cc.o: ../../Source/Renderer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Renderer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/StrahlenwerkApplication_a618e10d.o: ../../Source/StrahlenwerkApplication.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling StrahlenwerkApplication.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OpenGLComponent_fe08fa6f.o: ../../Source/OpenGLComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling OpenGLComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_832d1cec.o: ../../juce_modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_f204e6aa.o: ../../juce_modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_2086304a.o: ../../juce_modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_204fdce.o: ../../juce_modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_8c03830a.o: ../../juce_modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_7627d342.o: ../../juce_modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_7b4d41a6.o: ../../juce_modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_a76b6622.o: ../../juce_modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_54b82a.o: ../../juce_modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_366ffb6e.o: ../../juce_modules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
