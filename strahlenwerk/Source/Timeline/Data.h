#ifndef DATA_H
#define DATA_H

#include <juce>
#include "Interpolator.h"

class Data {
	public:
		Data();

		Interpolator& getInterpolator();

		void addListenerToTree(ValueTree::Listener* listener);

		ValueTree getScenesArray();
		bool addScene(ValueTree scene, int position = -1);
		bool addScene(var id, var start, var duration, var shaderSource, int position = -1);
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

		bool initializeValue(ValueTree valueData, var type);

		Value currentTime;

	private:
		ValueTree valueTree;
		UndoManager undoManager;
		Interpolator interpolator;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Data)
};

#endif // DATA_H
