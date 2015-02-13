#ifndef VALUETOGGLEBUTTON_H
#define VALUETOGGLEBUTTON_H

#include <juce>

class ValueToggleButton :
	public ToggleButton,
	private Value::Listener
{
	public:
		ValueToggleButton(const String transactionName_);
		~ValueToggleButton();

		void setValueData(const Value& valueData_);

		void clicked() override;
		void valueChanged(Value& value) override;

	private:
		Value valueData;
		const String transactionName;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueToggleButton)
};

#endif // VALUETOGGLEBUTTON_H
