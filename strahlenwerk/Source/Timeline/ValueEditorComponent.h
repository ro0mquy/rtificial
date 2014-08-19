#ifndef VALUEEDITORCOMPONENT_H
#define VALUEEDITORCOMPONENT_H

#include <juce>

class ValueEditorComponent : public Component {
	public:
		ValueEditorComponent(ValueTree valueData_);

		void resized() override;

	private:
		ValueTree valueData;
		ScopedPointer<Component> specificTypeEditor;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueEditorComponent)
};

#endif // VALUEEDITORCOMPONENT_H
