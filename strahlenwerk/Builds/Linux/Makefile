# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCE_DONT_ASSERT_ON_GLSL_COMPILE_ERROR -DGLM_FORCE_RADIANS -DJUCER_LINUX_MAKE_7346DA2A=1 -DJUCE_APP_VERSION -DJUCE_APP_VERSION_HEX=0x0 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../juce_modules
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O2 -Wall -Wextra -I../../Source -I../../Lib/include
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt -lefsw -L../../Lib/lib

  TARGET := strahlenwerk
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCE_DONT_ASSERT_ON_GLSL_COMPILE_ERROR -DGLM_FORCE_RADIANS -DJUCER_LINUX_MAKE_7346DA2A=1 -DJUCE_APP_VERSION -DJUCE_APP_VERSION_HEX=0x0 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../juce_modules
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O2 -Wall -Wextra -I../../Source -I../../Lib/include
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -fvisibility=hidden $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt -lefsw -L../../Lib/lib

  TARGET := strahlenwerk
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS := \
  $(JUCE_OBJDIR)/ColorPickerComponent_bd366953.o \
  $(JUCE_OBJDIR)/ColorTextInputComponent_c0aee926.o \
  $(JUCE_OBJDIR)/RtColorSelector_ebb45243.o \
  $(JUCE_OBJDIR)/FileWatchMessageThreadListener_df4b3f31.o \
  $(JUCE_OBJDIR)/Project_dae16ce4.o \
  $(JUCE_OBJDIR)/ProjectFileLoader_546d9f73.o \
  $(JUCE_OBJDIR)/AmbientLight_2567d32a.o \
  $(JUCE_OBJDIR)/EnvironmentMap_3b0268d5.o \
  $(JUCE_OBJDIR)/glcorert_b13e1c72.o \
  $(JUCE_OBJDIR)/PostprocPipeline_473f2604.o \
  $(JUCE_OBJDIR)/PostprocPipelineLoader_51edf977.o \
  $(JUCE_OBJDIR)/PostprocShader_df5d2107.o \
  $(JUCE_OBJDIR)/SceneShader_b92468cf.o \
  $(JUCE_OBJDIR)/Shader_cfe69b71.o \
  $(JUCE_OBJDIR)/Texture_68071d9.o \
  $(JUCE_OBJDIR)/Uniform_96b281f2.o \
  $(JUCE_OBJDIR)/UniformManager_63433de5.o \
  $(JUCE_OBJDIR)/DraggableQuaternion_55786799.o \
  $(JUCE_OBJDIR)/InfoLogComponent_7ff61de3.o \
  $(JUCE_OBJDIR)/InspectorComponent_96983e82.o \
  $(JUCE_OBJDIR)/InspectorSequenceComponent_294b0841.o \
  $(JUCE_OBJDIR)/InterpolationPlotComponent_cc16494.o \
  $(JUCE_OBJDIR)/PerformanceComponent_7ca1c829.o \
  $(JUCE_OBJDIR)/QuaternionEditor_3c7fc27.o \
  $(JUCE_OBJDIR)/SceneBackgroundComponent_3365f63f.o \
  $(JUCE_OBJDIR)/ScopesComponent_6b3b152c.o \
  $(JUCE_OBJDIR)/SequenceBackgroundComponent_5b0f667c.o \
  $(JUCE_OBJDIR)/Sidebar_eccfc76a.o \
  $(JUCE_OBJDIR)/ValueEditorPropertyComponent_ae881406.o \
  $(JUCE_OBJDIR)/ValueSlider_d188aa40.o \
  $(JUCE_OBJDIR)/ValueToggleButton_fed8bf05.o \
  $(JUCE_OBJDIR)/CameraController_be1bf2b0.o \
  $(JUCE_OBJDIR)/CameraMath_ba81f35c.o \
  $(JUCE_OBJDIR)/Interpolator_e73f9cd6.o \
  $(JUCE_OBJDIR)/JsonExporter_56469f78.o \
  $(JUCE_OBJDIR)/KeyframeComponent_bb6b78ca.o \
  $(JUCE_OBJDIR)/MouseCallbackClasses_88049c4b.o \
  $(JUCE_OBJDIR)/SceneComponent_2e62f260.o \
  $(JUCE_OBJDIR)/ScenesBarComponent_9e1694c0.o \
  $(JUCE_OBJDIR)/SectionManager_38bf0a17.o \
  $(JUCE_OBJDIR)/Selection_45a84fa7.o \
  $(JUCE_OBJDIR)/SequenceComponent_10a22e57.o \
  $(JUCE_OBJDIR)/SequenceViewComponent_a0963d32.o \
  $(JUCE_OBJDIR)/SpecialUniformController_926d55c6.o \
  $(JUCE_OBJDIR)/Timeline_d3785850.o \
  $(JUCE_OBJDIR)/TimelineData_bd97819a.o \
  $(JUCE_OBJDIR)/TimeMarkerComponent_32217cf1.o \
  $(JUCE_OBJDIR)/UniformsBarComponent_d6185a18.o \
  $(JUCE_OBJDIR)/ZoomFactor_9cf7cc11.o \
  $(JUCE_OBJDIR)/AudioManager_e2e40c00.o \
  $(JUCE_OBJDIR)/BlenderLookAndFeel_a1c31a69.o \
  $(JUCE_OBJDIR)/MainContentComponent_19da5e66.o \
  $(JUCE_OBJDIR)/MainWindow_499ac812.o \
  $(JUCE_OBJDIR)/OpenGLComponent_fe08fa6f.o \
  $(JUCE_OBJDIR)/OpenGLTargetComponent_5695ea9e.o \
  $(JUCE_OBJDIR)/Renderer_d34861cc.o \
  $(JUCE_OBJDIR)/RtColor_f0fa1222.o \
  $(JUCE_OBJDIR)/RtificialLookAndFeel_95d2050c.o \
  $(JUCE_OBJDIR)/RtImage_468fd01a.o \
  $(JUCE_OBJDIR)/ScreenRecorder_56d2f7d3.o \
  $(JUCE_OBJDIR)/StrahlenwerkApplication_a618e10d.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o \
  $(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o \
  $(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o \
  $(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o \
  $(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o \
  $(JUCE_OBJDIR)/juce_core_75b14332.o \
  $(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o \
  $(JUCE_OBJDIR)/juce_events_d2be882c.o \
  $(JUCE_OBJDIR)/juce_graphics_9c18891e.o \
  $(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o \
  $(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o \

.PHONY: clean

$(JUCE_OUTDIR)/$(TARGET): check-pkg-config $(OBJECTS) $(RESOURCES)
	@echo Linking strahlenwerk
	-@mkdir -p $(JUCE_BINDIR)
	-@mkdir -p $(JUCE_LIBDIR)
	-@mkdir -p $(JUCE_OUTDIR)
	@$(BLDCMD)

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl x11 xext xinerama

clean:
	@echo Cleaning strahlenwerk
	@$(CLEANCMD)

strip:
	@echo Stripping strahlenwerk
	-@$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

$(JUCE_OBJDIR)/ColorPickerComponent_bd366953.o: ../../Source/ColorPicker/ColorPickerComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColorPickerComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColorTextInputComponent_c0aee926.o: ../../Source/ColorPicker/ColorTextInputComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColorTextInputComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RtColorSelector_ebb45243.o: ../../Source/ColorPicker/RtColorSelector.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RtColorSelector.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FileWatchMessageThreadListener_df4b3f31.o: ../../Source/Project/FileWatchMessageThreadListener.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FileWatchMessageThreadListener.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Project_dae16ce4.o: ../../Source/Project/Project.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Project.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProjectFileLoader_546d9f73.o: ../../Source/Project/ProjectFileLoader.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProjectFileLoader.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AmbientLight_2567d32a.o: ../../Source/Rendering/AmbientLight.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AmbientLight.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EnvironmentMap_3b0268d5.o: ../../Source/Rendering/EnvironmentMap.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EnvironmentMap.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/glcorert_b13e1c72.o: ../../Source/Rendering/glcorert.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling glcorert.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PostprocPipeline_473f2604.o: ../../Source/Rendering/PostprocPipeline.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PostprocPipeline.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PostprocPipelineLoader_51edf977.o: ../../Source/Rendering/PostprocPipelineLoader.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PostprocPipelineLoader.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PostprocShader_df5d2107.o: ../../Source/Rendering/PostprocShader.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PostprocShader.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SceneShader_b92468cf.o: ../../Source/Rendering/SceneShader.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SceneShader.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Shader_cfe69b71.o: ../../Source/Rendering/Shader.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Shader.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Texture_68071d9.o: ../../Source/Rendering/Texture.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Texture.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Uniform_96b281f2.o: ../../Source/Rendering/Uniform.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Uniform.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UniformManager_63433de5.o: ../../Source/Rendering/UniformManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UniformManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DraggableQuaternion_55786799.o: ../../Source/Sidebar/DraggableQuaternion.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DraggableQuaternion.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InfoLogComponent_7ff61de3.o: ../../Source/Sidebar/InfoLogComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InfoLogComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InspectorComponent_96983e82.o: ../../Source/Sidebar/InspectorComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InspectorComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InspectorSequenceComponent_294b0841.o: ../../Source/Sidebar/InspectorSequenceComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InspectorSequenceComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InterpolationPlotComponent_cc16494.o: ../../Source/Sidebar/InterpolationPlotComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InterpolationPlotComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PerformanceComponent_7ca1c829.o: ../../Source/Sidebar/PerformanceComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PerformanceComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/QuaternionEditor_3c7fc27.o: ../../Source/Sidebar/QuaternionEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling QuaternionEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SceneBackgroundComponent_3365f63f.o: ../../Source/Sidebar/SceneBackgroundComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SceneBackgroundComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScopesComponent_6b3b152c.o: ../../Source/Sidebar/ScopesComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScopesComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceBackgroundComponent_5b0f667c.o: ../../Source/Sidebar/SequenceBackgroundComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceBackgroundComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Sidebar_eccfc76a.o: ../../Source/Sidebar/Sidebar.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Sidebar.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ValueEditorPropertyComponent_ae881406.o: ../../Source/Sidebar/ValueEditorPropertyComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ValueEditorPropertyComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ValueSlider_d188aa40.o: ../../Source/Sidebar/ValueSlider.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ValueSlider.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ValueToggleButton_fed8bf05.o: ../../Source/Sidebar/ValueToggleButton.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ValueToggleButton.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CameraController_be1bf2b0.o: ../../Source/Timeline/CameraController.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CameraController.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CameraMath_ba81f35c.o: ../../Source/Timeline/CameraMath.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CameraMath.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Interpolator_e73f9cd6.o: ../../Source/Timeline/Interpolator.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Interpolator.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JsonExporter_56469f78.o: ../../Source/Timeline/JsonExporter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JsonExporter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/KeyframeComponent_bb6b78ca.o: ../../Source/Timeline/KeyframeComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling KeyframeComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MouseCallbackClasses_88049c4b.o: ../../Source/Timeline/MouseCallbackClasses.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MouseCallbackClasses.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SceneComponent_2e62f260.o: ../../Source/Timeline/SceneComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SceneComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScenesBarComponent_9e1694c0.o: ../../Source/Timeline/ScenesBarComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScenesBarComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SectionManager_38bf0a17.o: ../../Source/Timeline/SectionManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SectionManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Selection_45a84fa7.o: ../../Source/Timeline/Selection.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Selection.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceComponent_10a22e57.o: ../../Source/Timeline/SequenceComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceViewComponent_a0963d32.o: ../../Source/Timeline/SequenceViewComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceViewComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SpecialUniformController_926d55c6.o: ../../Source/Timeline/SpecialUniformController.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SpecialUniformController.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Timeline_d3785850.o: ../../Source/Timeline/Timeline.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Timeline.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimelineData_bd97819a.o: ../../Source/Timeline/TimelineData.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimelineData.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeMarkerComponent_32217cf1.o: ../../Source/Timeline/TimeMarkerComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeMarkerComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UniformsBarComponent_d6185a18.o: ../../Source/Timeline/UniformsBarComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UniformsBarComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ZoomFactor_9cf7cc11.o: ../../Source/Timeline/ZoomFactor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ZoomFactor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioManager_e2e40c00.o: ../../Source/AudioManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BlenderLookAndFeel_a1c31a69.o: ../../Source/BlenderLookAndFeel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BlenderLookAndFeel.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainContentComponent_19da5e66.o: ../../Source/MainContentComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainContentComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainWindow_499ac812.o: ../../Source/MainWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainWindow.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OpenGLComponent_fe08fa6f.o: ../../Source/OpenGLComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OpenGLComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OpenGLTargetComponent_5695ea9e.o: ../../Source/OpenGLTargetComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OpenGLTargetComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Renderer_d34861cc.o: ../../Source/Renderer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Renderer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RtColor_f0fa1222.o: ../../Source/RtColor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RtColor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RtificialLookAndFeel_95d2050c.o: ../../Source/RtificialLookAndFeel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RtificialLookAndFeel.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RtImage_468fd01a.o: ../../Source/RtImage.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RtImage.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScreenRecorder_56d2f7d3.o: ../../Source/ScreenRecorder.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScreenRecorder.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StrahlenwerkApplication_a618e10d.o: ../../Source/StrahlenwerkApplication.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StrahlenwerkApplication.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o: ../../JuceLibraryCode/juce_audio_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o: ../../JuceLibraryCode/juce_audio_devices.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o: ../../JuceLibraryCode/juce_audio_formats.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o: ../../JuceLibraryCode/juce_audio_processors.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o: ../../JuceLibraryCode/juce_audio_utils.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o: ../../JuceLibraryCode/juce_opengl.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
