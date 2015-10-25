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
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O3 -Wall -Wextra -I../../Source -I../../Lib/include
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -lefsw -L../../Lib/lib

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
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O3 -Wall -Wextra -I../../Source -I../../Lib/include
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -lefsw -L../../Lib/lib

  TARGET := strahlenwerk
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/ColorPickerComponent_bd366953.o \
  $(OBJDIR)/ColorSpaceMarker_454a1924.o \
  $(OBJDIR)/ColorTextInputComponent_c0aee926.o \
  $(OBJDIR)/LabColor_145742fd.o \
  $(OBJDIR)/LabColorPicker_2909daab.o \
  $(OBJDIR)/LabColorPropertyComponent_eaa105f5.o \
  $(OBJDIR)/LabColorSpaceView_bf2be298.o \
  $(OBJDIR)/FileWatchMessageThreadListener_df4b3f31.o \
  $(OBJDIR)/Project_dae16ce4.o \
  $(OBJDIR)/ProjectFileLoader_546d9f73.o \
  $(OBJDIR)/AmbientLight_2567d32a.o \
  $(OBJDIR)/EnvironmentMap_3b0268d5.o \
  $(OBJDIR)/glcorert_b13e1c72.o \
  $(OBJDIR)/PostprocPipeline_473f2604.o \
  $(OBJDIR)/PostprocPipelineLoader_51edf977.o \
  $(OBJDIR)/PostprocShader_df5d2107.o \
  $(OBJDIR)/SceneShader_b92468cf.o \
  $(OBJDIR)/Shader_cfe69b71.o \
  $(OBJDIR)/Texture_68071d9.o \
  $(OBJDIR)/Uniform_96b281f2.o \
  $(OBJDIR)/UniformManager_63433de5.o \
  $(OBJDIR)/DraggableQuaternion_55786799.o \
  $(OBJDIR)/InfoLogComponent_7ff61de3.o \
  $(OBJDIR)/InspectorComponent_96983e82.o \
  $(OBJDIR)/InspectorSequenceComponent_294b0841.o \
  $(OBJDIR)/InterpolationPlotComponent_cc16494.o \
  $(OBJDIR)/QuaternionEditor_3c7fc27.o \
  $(OBJDIR)/SceneBackgroundComponent_3365f63f.o \
  $(OBJDIR)/SequenceBackgroundComponent_5b0f667c.o \
  $(OBJDIR)/Sidebar_eccfc76a.o \
  $(OBJDIR)/ValueEditorPropertyComponent_ae881406.o \
  $(OBJDIR)/ValueSlider_d188aa40.o \
  $(OBJDIR)/ValueToggleButton_fed8bf05.o \
  $(OBJDIR)/CameraController_be1bf2b0.o \
  $(OBJDIR)/CameraMath_ba81f35c.o \
  $(OBJDIR)/Interpolator_e73f9cd6.o \
  $(OBJDIR)/JsonExporter_56469f78.o \
  $(OBJDIR)/KeyframeComponent_bb6b78ca.o \
  $(OBJDIR)/MouseCallbackClasses_88049c4b.o \
  $(OBJDIR)/SceneComponent_2e62f260.o \
  $(OBJDIR)/ScenesBarComponent_9e1694c0.o \
  $(OBJDIR)/Selection_45a84fa7.o \
  $(OBJDIR)/SequenceComponent_10a22e57.o \
  $(OBJDIR)/SequenceViewComponent_a0963d32.o \
  $(OBJDIR)/SpecialUniformController_926d55c6.o \
  $(OBJDIR)/Timeline_d3785850.o \
  $(OBJDIR)/TimelineData_bd97819a.o \
  $(OBJDIR)/TimeMarkerComponent_32217cf1.o \
  $(OBJDIR)/UniformsBarComponent_d6185a18.o \
  $(OBJDIR)/ZoomFactor_9cf7cc11.o \
  $(OBJDIR)/AudioManager_e2e40c00.o \
  $(OBJDIR)/BlenderLookAndFeel_a1c31a69.o \
  $(OBJDIR)/MainContentComponent_19da5e66.o \
  $(OBJDIR)/MainWindow_499ac812.o \
  $(OBJDIR)/OpenGLComponent_fe08fa6f.o \
  $(OBJDIR)/OpenGLTargetComponent_5695ea9e.o \
  $(OBJDIR)/Renderer_d34861cc.o \
  $(OBJDIR)/RtificialLookAndFeel_95d2050c.o \
  $(OBJDIR)/StrahlenwerkApplication_a618e10d.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/juce_audio_basics_832d1cec.o \
  $(OBJDIR)/juce_audio_devices_f204e6aa.o \
  $(OBJDIR)/juce_audio_formats_2086304a.o \
  $(OBJDIR)/juce_audio_processors_3a5c4aa4.o \
  $(OBJDIR)/juce_audio_utils_92f7636a.o \
  $(OBJDIR)/juce_core_204fdce.o \
  $(OBJDIR)/juce_data_structures_8c03830a.o \
  $(OBJDIR)/juce_events_7627d342.o \
  $(OBJDIR)/juce_graphics_7b4d41a6.o \
  $(OBJDIR)/juce_gui_basics_a76b6622.o \
  $(OBJDIR)/juce_gui_extra_54b82a.o \
  $(OBJDIR)/juce_opengl_366ffb6e.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
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

$(OBJDIR)/ColorPickerComponent_bd366953.o: ../../Source/ColorPicker/ColorPickerComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ColorPickerComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ColorSpaceMarker_454a1924.o: ../../Source/ColorPicker/ColorSpaceMarker.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ColorSpaceMarker.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ColorTextInputComponent_c0aee926.o: ../../Source/ColorPicker/ColorTextInputComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ColorTextInputComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LabColor_145742fd.o: ../../Source/ColorPicker/LabColor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LabColor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LabColorPicker_2909daab.o: ../../Source/ColorPicker/LabColorPicker.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LabColorPicker.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LabColorPropertyComponent_eaa105f5.o: ../../Source/ColorPicker/LabColorPropertyComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LabColorPropertyComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LabColorSpaceView_bf2be298.o: ../../Source/ColorPicker/LabColorSpaceView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LabColorSpaceView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FileWatchMessageThreadListener_df4b3f31.o: ../../Source/Project/FileWatchMessageThreadListener.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FileWatchMessageThreadListener.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Project_dae16ce4.o: ../../Source/Project/Project.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Project.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ProjectFileLoader_546d9f73.o: ../../Source/Project/ProjectFileLoader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ProjectFileLoader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AmbientLight_2567d32a.o: ../../Source/Rendering/AmbientLight.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AmbientLight.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnvironmentMap_3b0268d5.o: ../../Source/Rendering/EnvironmentMap.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnvironmentMap.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/glcorert_b13e1c72.o: ../../Source/Rendering/glcorert.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling glcorert.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PostprocPipeline_473f2604.o: ../../Source/Rendering/PostprocPipeline.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PostprocPipeline.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PostprocPipelineLoader_51edf977.o: ../../Source/Rendering/PostprocPipelineLoader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PostprocPipelineLoader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PostprocShader_df5d2107.o: ../../Source/Rendering/PostprocShader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PostprocShader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SceneShader_b92468cf.o: ../../Source/Rendering/SceneShader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SceneShader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Shader_cfe69b71.o: ../../Source/Rendering/Shader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Shader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Texture_68071d9.o: ../../Source/Rendering/Texture.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Texture.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Uniform_96b281f2.o: ../../Source/Rendering/Uniform.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Uniform.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UniformManager_63433de5.o: ../../Source/Rendering/UniformManager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UniformManager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DraggableQuaternion_55786799.o: ../../Source/Sidebar/DraggableQuaternion.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DraggableQuaternion.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/InfoLogComponent_7ff61de3.o: ../../Source/Sidebar/InfoLogComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling InfoLogComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/InspectorComponent_96983e82.o: ../../Source/Sidebar/InspectorComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling InspectorComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/InspectorSequenceComponent_294b0841.o: ../../Source/Sidebar/InspectorSequenceComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling InspectorSequenceComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/InterpolationPlotComponent_cc16494.o: ../../Source/Sidebar/InterpolationPlotComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling InterpolationPlotComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/QuaternionEditor_3c7fc27.o: ../../Source/Sidebar/QuaternionEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling QuaternionEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SceneBackgroundComponent_3365f63f.o: ../../Source/Sidebar/SceneBackgroundComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SceneBackgroundComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SequenceBackgroundComponent_5b0f667c.o: ../../Source/Sidebar/SequenceBackgroundComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SequenceBackgroundComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Sidebar_eccfc76a.o: ../../Source/Sidebar/Sidebar.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Sidebar.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ValueEditorPropertyComponent_ae881406.o: ../../Source/Sidebar/ValueEditorPropertyComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ValueEditorPropertyComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ValueSlider_d188aa40.o: ../../Source/Sidebar/ValueSlider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ValueSlider.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ValueToggleButton_fed8bf05.o: ../../Source/Sidebar/ValueToggleButton.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ValueToggleButton.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CameraController_be1bf2b0.o: ../../Source/Timeline/CameraController.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CameraController.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CameraMath_ba81f35c.o: ../../Source/Timeline/CameraMath.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CameraMath.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Interpolator_e73f9cd6.o: ../../Source/Timeline/Interpolator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Interpolator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/JsonExporter_56469f78.o: ../../Source/Timeline/JsonExporter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling JsonExporter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/KeyframeComponent_bb6b78ca.o: ../../Source/Timeline/KeyframeComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling KeyframeComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MouseCallbackClasses_88049c4b.o: ../../Source/Timeline/MouseCallbackClasses.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MouseCallbackClasses.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SceneComponent_2e62f260.o: ../../Source/Timeline/SceneComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SceneComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ScenesBarComponent_9e1694c0.o: ../../Source/Timeline/ScenesBarComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ScenesBarComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Selection_45a84fa7.o: ../../Source/Timeline/Selection.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Selection.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SequenceComponent_10a22e57.o: ../../Source/Timeline/SequenceComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SequenceComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SequenceViewComponent_a0963d32.o: ../../Source/Timeline/SequenceViewComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SequenceViewComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SpecialUniformController_926d55c6.o: ../../Source/Timeline/SpecialUniformController.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SpecialUniformController.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Timeline_d3785850.o: ../../Source/Timeline/Timeline.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Timeline.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TimelineData_bd97819a.o: ../../Source/Timeline/TimelineData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TimelineData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TimeMarkerComponent_32217cf1.o: ../../Source/Timeline/TimeMarkerComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TimeMarkerComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UniformsBarComponent_d6185a18.o: ../../Source/Timeline/UniformsBarComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UniformsBarComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ZoomFactor_9cf7cc11.o: ../../Source/Timeline/ZoomFactor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ZoomFactor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioManager_e2e40c00.o: ../../Source/AudioManager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioManager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BlenderLookAndFeel_a1c31a69.o: ../../Source/BlenderLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BlenderLookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainContentComponent_19da5e66.o: ../../Source/MainContentComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainContentComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainWindow_499ac812.o: ../../Source/MainWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OpenGLComponent_fe08fa6f.o: ../../Source/OpenGLComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling OpenGLComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OpenGLTargetComponent_5695ea9e.o: ../../Source/OpenGLTargetComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling OpenGLTargetComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Renderer_d34861cc.o: ../../Source/Renderer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Renderer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RtificialLookAndFeel_95d2050c.o: ../../Source/RtificialLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RtificialLookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/StrahlenwerkApplication_a618e10d.o: ../../Source/StrahlenwerkApplication.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling StrahlenwerkApplication.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
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

$(OBJDIR)/juce_audio_processors_3a5c4aa4.o: ../../juce_modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_92f7636a.o: ../../juce_modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
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
