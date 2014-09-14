#ifndef TIMELINEDATA_H
#define TIMELINEDATA_H

#include <mutex>
#include <juce>
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
		int compareElements(const ValueTree& first, const ValueTree& second);


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

		void setSceneId(ValueTree scene, var id);
		void setSceneStart(ValueTree scene, var start);
		void setSceneDuration(ValueTree scene, var duration);
		void setSceneShaderSource(ValueTree scene, var shaderSource);

		int getLastSceneEndTime();
		ValueTree getSceneForTime(const int time);
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

		void setSequenceSceneId(ValueTree sequence, var sceneId);
		void setSequenceStart(ValueTree sequence, var start);
		void setSequenceDuration(ValueTree sequence, var duration);
		void setSequenceInterpolation(ValueTree sequence, var interpolation);

		void setSequencePropertiesForAbsoluteStart(ValueTree sequence, int absoluteStart);
		int getAbsoluteStartForSequence(ValueTree sequence);
		ValueTree getSequenceParentUniform(ValueTree sequence);


		// keyframe stuff
		ValueTree getKeyframesArray(ValueTree sequence);
		int getNumKeyframes(ValueTree sequence);

		ValueTree getKeyframe(ValueTree sequence, const int nthKeyframe);
		bool isKeyframe(ValueTree keyframe);
		ValueTree addKeyframe(ValueTree sequence, ValueTree keyframe);
		ValueTree addKeyframe(ValueTree sequence, var keyframePosition);
		ValueTree addKeyframeUnchecked(ValueTree sequence, ValueTree keyframe);

		var getKeyframePosition(ValueTree keyframe);
		ValueTree getKeyframeValue(ValueTree keyframe);

		void setKeyframePosition(ValueTree keyframe, var position);
		void setKeyframeValue(ValueTree keyframe, ValueTree value);

		void initializeKeyframesArray(ValueTree sequence);


		// value stuff
		bool initializeValue(ValueTree valueData, String valueType);

		Value currentTime;

	private:
		ValueTree valueTree;
		UndoManager undoManager;
		Interpolator interpolator;

		std::recursive_mutex treeMutex;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineData)
};

#endif // TIMELINEDATA_H
