#ifndef VALUESLIDER_H
#define VALUESLIDER_H

#include <juce>

class ValueSlider :
	public Slider,
	private Value::Listener
{
	public:
		ValueSlider(Value valueData_, const String transactionName_);
		~ValueSlider();

		void setValueData(const Value& valueData_);

		void startedDragging() override;
		void valueChanged() override;
		void valueChanged(Value& value) override;

	private:
		Value valueData;
		const String transactionName;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueSlider)
};

#endif // VALUESLIDER_H
