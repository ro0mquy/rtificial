#ifndef COLORSPACEMARKER_H
#define COLORSPACEMARKER_H

#include <juce>

class ColorSpaceMarker  : public Component {
	public:
		ColorSpaceMarker();
		void paint (Graphics& g) override;

	private:
		JUCE_DECLARE_NON_COPYABLE (ColorSpaceMarker)
};

#endif
