#ifndef TIMELINEDATA_H
#define TIMELINEDATA_H

#include <mutex>
#include <juce>
#include <glm/fwd.hpp>
#include "Interpolator.h"

class TimelineData {
	public:
		TimelineData(const File& dataFile);
		TimelineData();


		// misc functions
		static TimelineData& getTimelineData();
		Interpolator& getInterpolator();

		void readTimelineDataFromFile(const File& dataFile);
		void writeTimelineDataToFile(const File& dataFile);
		void addListenerToTree(ValueTree::Listener* listener);
		void removeListenerFromTree(ValueTree::Listener* listener);

		std::recursive_mutex& getMutex();


		// scene related stuff
		ValueTree getScenesArray();
		int getNumScenes();

		ValueTree getScene(const int nthScene);
		ValueTree getScene(const var& sceneId);
		bool isScene(ValueTree scene);
		ValueTree addScene(ValueTree scene, int position = -1);
		ValueTree addScene(var start, var duration, var shaderSource, int position = -1);
		ValueTree addSceneUnchecked(ValueTree scene, int position = -1);
		void removeScene(ValueTree scene);

		var getSceneId(ValueTree scene);
		var getSceneStart(ValueTree scene);
		var getSceneDuration(ValueTree scene);
		var getSceneShaderSource(ValueTree scene);

		Value getSceneShaderSourceAsValue(ValueTree scene);

		void setSceneId(ValueTree scene, var id);
		void setSceneStart(ValueTree scene, var start);
		void setSceneDuration(ValueTree scene, var duration);
		void setSceneShaderSource(ValueTree scene, var shaderSource);

		float getLastSceneEndTime();
		ValueTree getSceneForTime(const float time);
		ValueTree getCurrentScene();
		int getNewSceneId();


		// uniform stuff
		ValueTree getUniformsArray();
		int getNumUniforms();

		ValueTree getUniform(const int nthUniform);
		ValueTree getUniform(const var& name);
		bool isUniform(ValueTree uniform);
		ValueTree addUniform(ValueTree uniform, int position = -1);
		ValueTree addUniform(var name, var type, int position = -1);
		ValueTree addUniformUnchecked(ValueTree uniform, int position);

		var getUniformName(ValueTree uniform);
		var getUniformType(ValueTree uniform);
		ValueTree getUniformStandardValue(ValueTree uniform);
		ValueTree getOrCreateUniformStandardValue(ValueTree uniform);

		void setUniformName(ValueTree uniform, var name);
		void setUniformType(ValueTree uniform, var type);
		void setUniformStandardValue(ValueTree uniform, ValueTree standardValue);

		int getUniformIndex(ValueTree uniform);


		// sequence stuff
		ValueTree getSequencesArray(ValueTree uniform);
		int getNumSequences(ValueTree uniform);

		ValueTree getSequence(ValueTree uniform, const int nthSequence);
		bool isSequence(ValueTree sequence);
		ValueTree addSequence(ValueTree uniform, ValueTree sequence, int position = -1);
		ValueTree addSequence(ValueTree uniform, float absoluteStart, var duration, var interpolation, int position = -1);
		ValueTree addSequenceUnchecked(ValueTree uniform, ValueTree sequence, int position);
		void removeSequence(ValueTree sequence);

		var getSequenceSceneId(ValueTree sequence);
		var getSequenceStart(ValueTree sequence);
		var getSequenceDuration(ValueTree sequence);
		var getSequenceInterpolation(ValueTree sequence);

		void setSequenceSceneId(ValueTree sequence, var sceneId);
		void setSequenceStart(ValueTree sequence, var start);
		void setSequenceDuration(ValueTree sequence, var duration);
		void setSequenceInterpolation(ValueTree sequence, var interpolation);

		void setSequencePropertiesForAbsoluteStart(ValueTree sequence, float absoluteStart);
		float getAbsoluteStartForSequence(ValueTree sequence);
		ValueTree getSequenceParentUniform(ValueTree sequence);


		// keyframe stuff
		ValueTree getKeyframesArray(ValueTree sequence);
		int getNumKeyframes(ValueTree sequence);

		ValueTree getKeyframe(ValueTree sequence, const int nthKeyframe);
		bool isKeyframe(ValueTree keyframe);
		ValueTree addKeyframe(ValueTree sequence, ValueTree keyframe);
		ValueTree addKeyframe(ValueTree sequence, var keyframePosition);
		ValueTree addKeyframeUnchecked(ValueTree sequence, ValueTree keyframe);
		void removeKeyframe(ValueTree keyframe);

		var getKeyframePosition(ValueTree keyframe);
		ValueTree getKeyframeValue(ValueTree keyframe);

		void setKeyframePosition(ValueTree keyframe, var position);
		void setKeyframeValue(ValueTree keyframe, ValueTree value);

		void initializeKeyframesArray(ValueTree sequence);
		int getKeyframeIndex(ValueTree keyframe);
		ValueTree getKeyframeParentSequence(ValueTree keyframe);
		int compareKeyframes(const ValueTree& first, const ValueTree& second);


		// value stuff
		int getNumValueProperties(ValueTree value);
		bool initializeValue(ValueTree valueData, String valueType);

		bool isValueBool(ValueTree value);
		bool isValueFloat(ValueTree value);
		bool isValueVec2(ValueTree value);
		bool isValueVec3(ValueTree value);
		bool isValueVec4(ValueTree value);
		bool isValueColor(ValueTree value);

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

		void setValueBoolState(ValueTree value, var boolState);
		void setValueFloatX(ValueTree value, var floatX);
		void setValueVec2X(ValueTree value, var vec2X);
		void setValueVec2Y(ValueTree value, var vec2Y);
		void setValueVec3X(ValueTree value, var vec3X);
		void setValueVec3Y(ValueTree value, var vec3Y);
		void setValueVec3Z(ValueTree value, var vec3Z);
		void setValueVec4X(ValueTree value, var vec4X);
		void setValueVec4Y(ValueTree value, var vec4Y);
		void setValueVec4Z(ValueTree value, var vec4Z);
		void setValueVec4W(ValueTree value, var vec4W);
		void setValueColorR(ValueTree value, var colorR);
		void setValueColorG(ValueTree value, var colorG);
		void setValueColorB(ValueTree value, var colorB);


		// math stuff (glm, ...)
		float getFloatFromValue(ValueTree value);
		glm::vec2 getVec2FromValue(ValueTree value);
		glm::vec3 getVec3FromValue(ValueTree value);
		glm::vec4 getVec4FromValue(ValueTree value);
		glm::vec3 getColorFromValue(ValueTree value);

		void setFloatToValue(ValueTree value, float scalar);
		void setVec2ToValue(ValueTree value, glm::vec2 vector);
		void setVec3ToValue(ValueTree value, glm::vec3 vector);
		void setVec4ToValue(ValueTree value, glm::vec4 vector);
		void setQuatToValue(ValueTree value, glm::quat vector);
		void setColorToValue(ValueTree value, glm::vec3 vector);

		ValueTree mixValues(ValueTree value1, ValueTree value2, const float t);
		ValueTree calculateCcrSplineForValues(ValueTree valueP0, ValueTree P1, ValueTree P2, ValueTree P3, const float t);

	private:
		ValueTree valueTree;
		UndoManager undoManager;
		Interpolator interpolator;

		std::recursive_mutex treeMutex;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineData)
};

#endif // TIMELINEDATA_H
