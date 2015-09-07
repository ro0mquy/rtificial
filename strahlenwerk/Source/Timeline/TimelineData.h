#ifndef TIMELINEDATA_H
#define TIMELINEDATA_H

#include <mutex>
#include <juce>
#include <glm/fwd.hpp>
#include "Interpolator.h"
#include "Selection.h"

class TimelineData :
	private ApplicationCommandManagerListener
{
	public:
		TimelineData(const File& dataFile);
		TimelineData();


		// misc functions
		static TimelineData& getTimelineData();
		Interpolator& getInterpolator();
		std::recursive_mutex& getMutex();
		UndoManager& getUndoManager();
		Selection& getSelection();

		void readTimelineDataFromFile(const File& dataFile);
		void writeTimelineDataToFile(const File& dataFile);
		void addListenerToTree(ValueTree::Listener* listener);
		void removeListenerFromTree(ValueTree::Listener* listener);

		void applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) override;
		void applicationCommandListChanged() override;


		// scene related stuff
		ValueTree getScenesArray();
		int getNumScenes();

		ValueTree getScene(const int nthScene);
		ValueTree getScene(const var& sceneId);
		bool isScene(ValueTree scene);
		ValueTree addScene(ValueTree scene, int position = -1);
		ValueTree addScene(var start, var duration, var shaderSource, var environmentSource, int position = -1);
		ValueTree addSceneUnchecked(ValueTree scene, int position = -1);
		void removeScene(ValueTree scene);

		var getSceneId(ValueTree scene);
		var getSceneStart(ValueTree scene);
		var getSceneDuration(ValueTree scene);
		var getSceneShaderSource(ValueTree scene);
		var getSceneEnvironmentSource(ValueTree scene);

		Value getSceneShaderSourceAsValue(ValueTree scene);
		Value getSceneEnvironmentSourceAsValue(ValueTree scene);

		void setSceneId(ValueTree scene, var id);
		void setSceneStart(ValueTree scene, var start);
		void setSceneDuration(ValueTree scene, var duration);
		void setSceneShaderSource(ValueTree scene, var shaderSource);
		void setSceneEnvironmentSource(ValueTree scene, var environmentSource);

		int getLastSceneEndTime();
		ValueTree getSceneForTime(const int time);
		ValueTree getCurrentScene();
		int getNewSceneId();
		int compareScenes(const ValueTree& first, const ValueTree& second);


		// uniform stuff
		ValueTree getUniformsArray();
		int getNumUniforms();

		ValueTree getUniform(const int nthUniform);
		ValueTree getUniform(const var& name);
		bool isUniform(ValueTree uniform);
		ValueTree addUniform(ValueTree uniform);
		ValueTree addUniform(var name, var type);
		ValueTree addUniformUnchecked(ValueTree uniform);
		void removeUniform(ValueTree uniform);

		var getUniformName(ValueTree uniform);
		var getUniformType(ValueTree uniform);
		ValueTree getUniformStandardValue(ValueTree uniform);
		ValueTree getOrCreateUniformStandardValue(ValueTree uniform);

		void setUniformName(ValueTree uniform, var name);
		void setUniformType(ValueTree uniform, var type);
		void setUniformStandardValue(ValueTree uniform, ValueTree standardValue);

		int getUniformIndex(ValueTree uniform);
		int compareUniforms(const ValueTree& first, const ValueTree& second);
		void bakeUniform(ValueTree uniform);


		// sequence stuff
		ValueTree getSequencesArray(ValueTree uniform);
		int getNumSequences(ValueTree uniform);

		ValueTree getSequence(ValueTree uniform, const int nthSequence);
		bool isSequence(ValueTree sequence);
		ValueTree addSequence(ValueTree uniform, ValueTree sequence, int position = -1);
		ValueTree addSequence(ValueTree uniform, int absoluteStart, var duration, var interpolation, int position = -1);
		ValueTree addSequenceUnchecked(ValueTree uniform, ValueTree sequence, int position);
		void removeSequence(ValueTree sequence);

		var getSequenceSceneId(ValueTree sequence);
		var getSequenceStart(ValueTree sequence);
		var getSequenceDuration(ValueTree sequence);
		var getSequenceInterpolation(ValueTree sequence);

		Value getSequenceInterpolationAsValue(ValueTree sequence);

		void setSequenceSceneId(ValueTree sequence, var sceneId);
		void setSequenceStart(ValueTree sequence, var start);
		void setSequenceDuration(ValueTree sequence, var duration);
		void setSequenceInterpolation(ValueTree sequence, var interpolation);

		void setSequencePropertiesForAbsoluteStart(ValueTree sequence, int absoluteStart);
		int getAbsoluteStartForSequence(ValueTree sequence);
		ValueTree getSequenceForTime(ValueTree uniform, const int absoluteTime);
		ValueTree getCurrentSequence(ValueTree uniform);
		ValueTree getSequenceParentUniform(ValueTree sequence);
		ValueTree getSequenceUniformStandardValue(ValueTree sequence);
		int compareSequences(const ValueTree& first, const ValueTree& second);


		// keyframe stuff
		ValueTree getKeyframesArray(ValueTree sequence);
		int getNumKeyframes(ValueTree sequence);

		ValueTree getKeyframe(ValueTree sequence, const int nthKeyframe);
		ValueTree getKeyframe(ValueTree sequence, const var& position);
		bool isKeyframe(ValueTree keyframe);
		ValueTree addKeyframe(ValueTree sequence, ValueTree keyframe);
		ValueTree addKeyframe(ValueTree sequence, var keyframePosition, var keyframeEaseToward = false, var keyframeEaseAway = false);
		ValueTree addKeyframeUnchecked(ValueTree sequence, ValueTree keyframe);
		void removeKeyframe(ValueTree keyframe);

		var getKeyframePosition(ValueTree keyframe);
		var getKeyframeEaseToward(ValueTree keyframe);
		var getKeyframeEaseAway(ValueTree keyframe);
		ValueTree getKeyframeValue(ValueTree keyframe);

		Value getKeyframePositionAsValue(ValueTree keyframe);
		Value getKeyframeEaseTowardAsValue(ValueTree keyframe);
		Value getKeyframeEaseAwayAsValue(ValueTree keyframe);

		void setKeyframePosition(ValueTree keyframe, var position);
		void setKeyframeEaseToward(ValueTree keyframe, var easeToward);
		void setKeyframeEaseAway(ValueTree keyframe, var easeAway);
		void setKeyframeValue(ValueTree keyframe, ValueTree value);

		int getKeyframeIndex(ValueTree keyframe);
		ValueTree getKeyframeParentSequence(ValueTree keyframe);
		ValueTree getKeyframeParentUniform(ValueTree keyframe);
		int compareKeyframes(const ValueTree& first, const ValueTree& second);


		// value stuff
		int getNumValueProperties(ValueTree value);
		bool initializeValue(ValueTree valueData, String valueType);
		String getValueAsString(ValueTree value);

		bool isValueBool(ValueTree value);
		bool isValueFloat(ValueTree value);
		bool isValueVec2(ValueTree value);
		bool isValueVec3(ValueTree value);
		bool isValueVec4(ValueTree value);
		bool isValueColor(ValueTree value);
		bool isValueQuat(ValueTree value);

		var getValueBoolState(ValueTree value);
		var getValueFloatX(ValueTree value);
		var getValueVec2X(ValueTree value);
		var getValueVec2Y(ValueTree value);
		var getValueVec3X(ValueTree value);
		var getValueVec3Y(ValueTree value);
		var getValueVec3Z(ValueTree value);
		var getValueVec4X(ValueTree value);
		var getValueVec4Y(ValueTree value);
		var getValueVec4Z(ValueTree value);
		var getValueVec4W(ValueTree value);
		var getValueColorR(ValueTree value);
		var getValueColorG(ValueTree value);
		var getValueColorB(ValueTree value);
		var getValueQuatX(ValueTree value);
		var getValueQuatY(ValueTree value);
		var getValueQuatZ(ValueTree value);
		var getValueQuatW(ValueTree value);

		Value getValueBoolStateAsValue(ValueTree value);
		Value getValueFloatXAsValue(ValueTree value);
		Value getValueVec2XAsValue(ValueTree value);
		Value getValueVec2YAsValue(ValueTree value);
		Value getValueVec3XAsValue(ValueTree value);
		Value getValueVec3YAsValue(ValueTree value);
		Value getValueVec3ZAsValue(ValueTree value);
		Value getValueVec4XAsValue(ValueTree value);
		Value getValueVec4YAsValue(ValueTree value);
		Value getValueVec4ZAsValue(ValueTree value);
		Value getValueVec4WAsValue(ValueTree value);
		Value getValueColorRAsValue(ValueTree value);
		Value getValueColorGAsValue(ValueTree value);
		Value getValueColorBAsValue(ValueTree value);
		Value getValueQuatXAsValue(ValueTree value);
		Value getValueQuatYAsValue(ValueTree value);
		Value getValueQuatZAsValue(ValueTree value);
		Value getValueQuatWAsValue(ValueTree value);

		void setValueBoolState(ValueTree value, var boolState, bool useUndoManager = true);
		void setValueFloatX(ValueTree value, var floatX, bool useUndoManager = true);
		void setValueVec2X(ValueTree value, var vec2X, bool useUndoManager = true);
		void setValueVec2Y(ValueTree value, var vec2Y, bool useUndoManager = true);
		void setValueVec3X(ValueTree value, var vec3X, bool useUndoManager = true);
		void setValueVec3Y(ValueTree value, var vec3Y, bool useUndoManager = true);
		void setValueVec3Z(ValueTree value, var vec3Z, bool useUndoManager = true);
		void setValueVec4X(ValueTree value, var vec4X, bool useUndoManager = true);
		void setValueVec4Y(ValueTree value, var vec4Y, bool useUndoManager = true);
		void setValueVec4Z(ValueTree value, var vec4Z, bool useUndoManager = true);
		void setValueVec4W(ValueTree value, var vec4W, bool useUndoManager = true);
		void setValueColorR(ValueTree value, var colorR, bool useUndoManager = true);
		void setValueColorG(ValueTree value, var colorG, bool useUndoManager = true);
		void setValueColorB(ValueTree value, var colorB, bool useUndoManager = true);
		void setValueQuatX(ValueTree value, var quatX, bool useUndoManager = true);
		void setValueQuatY(ValueTree value, var quatY, bool useUndoManager = true);
		void setValueQuatZ(ValueTree value, var quatZ, bool useUndoManager = true);
		void setValueQuatW(ValueTree value, var quatW, bool useUndoManager = true);


		// math stuff (glm, ...)
		bool getBoolFromValue(ValueTree value);
		float getFloatFromValue(ValueTree value);
		glm::vec2 getVec2FromValue(ValueTree value);
		glm::vec3 getVec3FromValue(ValueTree value);
		glm::vec4 getVec4FromValue(ValueTree value);
		glm::vec3 getColorFromValue(ValueTree value);
		glm::quat getQuatFromValue(ValueTree value);

		void setFloatToValue(ValueTree value, float scalar, bool useUndoManager = true);
		void setVec2ToValue(ValueTree value, glm::vec2 vector, bool useUndoManager = true);
		void setVec3ToValue(ValueTree value, glm::vec3 vector, bool useUndoManager = true);
		void setVec4ToValue(ValueTree value, glm::vec4 vector, bool useUndoManager = true);
		void setColorToValue(ValueTree value, glm::vec3 vector, bool useUndoManager = true);
		void setQuatToValue(ValueTree value, glm::quat vector, bool useUndoManager = true);

		bool areValuesEqual(ValueTree v1, ValueTree v2);
		ValueTree mixValues(ValueTree value1, ValueTree value2, const float t);
		ValueTree calculateCcrSplineForValues(ValueTree valueP0, ValueTree P1, ValueTree P2, ValueTree P3, const float t);

		enum CommandIDs {
			undoAction = 0x7f41fa00,
			redoAction,
		};

	private:
		ValueTree valueTree;
		UndoManager undoManager;
		Interpolator interpolator;
		Selection selection;

		std::recursive_mutex treeMutex;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineData)
};

#endif // TIMELINEDATA_H
