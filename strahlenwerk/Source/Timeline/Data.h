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

	private:
		ValueTree valueTree;
		UndoManager undoManager;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Data)
};

#endif // DATA_H
