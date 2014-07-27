#ifndef DATA_H
#define DATA_H

#include "../../JuceLibraryCode/JuceHeader.h"

class Data {
	public:
		Data();

		ValueTree getScenesArray();
		bool addScene(ValueTree scene, int position = -1);
		bool addScene(var id, var start, var duration, var shaderSource, int position = -1);
		int getLastSceneEndTime();

		ValueTree getUniformsArray();
		bool addUniform(ValueTree uniform, int position = -1);
		bool addUniform(var name, var type, int position = -1);

		ValueTree getSequencesArray(ValueTree uniform);
		bool addSequence(ValueTree uniform, ValueTree sequence, int position = -1);
		bool addSequence(ValueTree uniform, var sceneId, var start, var duration, var interpolation, int position = -1);

	private:
		ValueTree valueTree;
		UndoManager undoManager;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Data)
};

#endif // DATA_H
