#ifndef TIMELINEDATA_H
#define TIMELINEDATA_H

#include <mutex>
#include <juce>
#include "Interpolator.h"

class TimelineData {
	public:
		TimelineData();


		// misc functions
		static TimelineData& getTimelineData();
		Interpolator& getInterpolator();

		void addListenerToTree(ValueTree::Listener* listener);


		// scene related stuff
		ValueTree getScenesArray();
		int getNumScenes();

		ValueTree getScene(const int nthScene);
		bool isScene(ValueTree scene);
		ValueTree addScene(ValueTree scene, int position = -1);
		ValueTree addScene(var start, var duration, var shaderSource, int position = -1);
		ValueTree addSceneUnchecked(ValueTree scene, int position = -1);

		void setSceneId(ValueTree scene, var id);
		void setSceneStart(ValueTree scene, var start);
		void setSceneDuration(ValueTree scene, var duration);
		void setSceneShaderSource(ValueTree scene, var shaderSource);

		int getLastSceneEndTime();
		ValueTree getSceneForTime(const int time);
		int getNewSceneId();

		ValueTree getUniformsArray();
		bool addUniform(ValueTree uniform, int position = -1);
		bool addUniform(var name, var type, int position = -1);
		ValueTree getUniform(const var& name);

		ValueTree getSequencesArray(ValueTree uniform);
		bool addSequence(ValueTree uniform, ValueTree sequence, int position = -1);
		bool addSequence(ValueTree uniform, var sceneId, var start, var duration, var interpolation, int position = -1);
		bool setSequencePropertiesForAbsoluteStart(ValueTree sequence, int absoluteStart);

		ValueTree getKeyframesArray(ValueTree sequence);
		bool initializeKeyframesArray(ValueTree sequence);
		bool addKeyframe(ValueTree sequence, ValueTree keyframe);
		bool addKeyframe(ValueTree sequence, var keyframePosition);

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
