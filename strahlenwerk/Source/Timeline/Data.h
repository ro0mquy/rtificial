#ifndef DATA_H
#define DATA_H

#include "../../JuceLibraryCode/JuceHeader.h"

class Data {
	public:
		Data();

	private:
		ValueTree valueTree;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Data)
};

#endif // DATA_H
