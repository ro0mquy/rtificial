#ifndef VALUEEDITORPROPERTYCOMPONENT_H
#define VALUEEDITORPROPERTYCOMPONENT_H

#include <juce>

class ValueEditorPropertyComponent : public PropertyComponent {
	public:
		ValueEditorPropertyComponent(const String& propertyName, int numberOfRows = 1);

		void refresh() override;
		void resized() override;

		virtual void useValueData(ValueTree /*valueData*/) {};

		static PropertyComponent* newValueEditorPropertyComponent(const String& propertyName, ValueTree valueData);

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueEditorPropertyComponent)
};

#endif // VALUEEDITORPROPERTYCOMPONENT_H
