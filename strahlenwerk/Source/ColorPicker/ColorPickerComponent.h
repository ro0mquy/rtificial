#ifndef COLORPICKERCOMPONENT_H
#define COLORPICKERCOMPONENT_H

#include <juce>

class ColorPickerComponent :
	public TabbedComponent,
	private Value::Listener,
	private ChangeListener
{
	public:
		ColorPickerComponent(Value& colorR_, Value& colorG_, Value& colorB_);
		~ColorPickerComponent();

		void setValueData(Value& colorR_, Value& colorG_, Value& colorB_);

		void valueChanged(Value& value) override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

		enum ColourIds {
			tabBackgroundColourId = 0xe6bc0301,
		};

	private:
		Value colorR;
		Value colorG;
		Value colorB;

		ColourSelector hsvSelector;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorPickerComponent)
};

#endif // COLORPICKERCOMPONENT_H
