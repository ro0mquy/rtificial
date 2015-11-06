#ifndef QUATERNIONEDITOR_H
#define QUATERNIONEDITOR_H

#include <juce>
#include <Sidebar/DraggableQuaternion.h>

class QuaternionEditor :
	public Component,
	private Value::Listener
{
	public:
		QuaternionEditor(const String& transactionName_);
		~QuaternionEditor();

		void setValueData(const Value& quatX_, const Value& quatY_, const Value& quatZ_, const Value& quatW_);

		void paint(Graphics& g) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;

		void valueChanged(Value& value) override;

	private:
		Value quatX;
		Value quatY;
		Value quatZ;
		Value quatW;

		const String transactionName;
		DraggableQuaternion orientation;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(QuaternionEditor)
};

#endif // QUATERNIONEDITOR_H
