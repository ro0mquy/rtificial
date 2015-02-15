#include "ValueEditorPropertyComponent.h"
#include <Timeline/TimelineData.h>
#include <Sidebar/ValueSlider.h>
#include <Sidebar/ValueToggleButton.h>
#include <ColorPicker/LabColor.h>
#include <ColorPicker/LabColorPicker.h>
#include <ColorPicker/ColorPickerComponent.h>

class BoolEditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		BoolEditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 1),
			button("Changed " + name + " Value")
		{
			useValueData(valueData);
			addAndMakeVisible(button);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			button.setValueData(data.getValueBoolStateAsValue(valueData));
		}

	private:
		ValueToggleButton button;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolEditorPropertyComponent)
};

class FloatEditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		FloatEditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 1),
			sliderX("Changed " + name + " Value")
		{
			useValueData(valueData);
			addAndMakeVisible(sliderX);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			sliderX.setValueData(data.getValueFloatXAsValue(valueData));
		}

	private:
		ValueSlider sliderX;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatEditorPropertyComponent)
};

class Vec2EditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		Vec2EditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 2),
			sliderX("Changed " + name + " Value"),
			sliderY("Changed " + name + " Value")
		{
			useValueData(valueData);
			addAndMakeVisible(sliderX);
			addAndMakeVisible(sliderY);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			sliderX.setValueData(data.getValueVec2XAsValue(valueData));
			sliderY.setValueData(data.getValueVec2YAsValue(valueData));
		}

	private:
		ValueSlider sliderX;
		ValueSlider sliderY;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vec2EditorPropertyComponent)
};

class Vec3EditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		Vec3EditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 3),
			sliderX("Changed " + name + " Value"),
			sliderY("Changed " + name + " Value"),
			sliderZ("Changed " + name + " Value")
		{
			useValueData(valueData);
			addAndMakeVisible(sliderX);
			addAndMakeVisible(sliderY);
			addAndMakeVisible(sliderZ);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			sliderX.setValueData(data.getValueVec3XAsValue(valueData));
			sliderY.setValueData(data.getValueVec3YAsValue(valueData));
			sliderZ.setValueData(data.getValueVec3ZAsValue(valueData));
		}

	private:
		ValueSlider sliderX;
		ValueSlider sliderY;
		ValueSlider sliderZ;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vec3EditorPropertyComponent)
};

class Vec4EditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		Vec4EditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 4),
			sliderX("Changed " + name + " Value"),
			sliderY("Changed " + name + " Value"),
			sliderZ("Changed " + name + " Value"),
			sliderW("Changed " + name + " Value")
		{
			useValueData(valueData);
			addAndMakeVisible(sliderX);
			addAndMakeVisible(sliderY);
			addAndMakeVisible(sliderZ);
			addAndMakeVisible(sliderW);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			if (data.isValueQuat(valueData)) {
				sliderX.setValueData(data.getValueQuatXAsValue(valueData));
				sliderY.setValueData(data.getValueQuatYAsValue(valueData));
				sliderZ.setValueData(data.getValueQuatZAsValue(valueData));
				sliderW.setValueData(data.getValueQuatWAsValue(valueData));
			} else {
				sliderX.setValueData(data.getValueVec4XAsValue(valueData));
				sliderY.setValueData(data.getValueVec4YAsValue(valueData));
				sliderZ.setValueData(data.getValueVec4ZAsValue(valueData));
				sliderW.setValueData(data.getValueVec4WAsValue(valueData));
			}
		}

	private:
		ValueSlider sliderX;
		ValueSlider sliderY;
		ValueSlider sliderZ;
		ValueSlider sliderW;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vec4EditorPropertyComponent)
};

class ColorEditorPropertyComponent : public ValueEditorPropertyComponent,
	private Value::Listener
{
	public:
		ColorEditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 1)
		{
			useValueData(valueData);
			colorR.addListener(this);
			colorG.addListener(this);
			colorB.addListener(this);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			colorR.referTo(data.getValueColorRAsValue(valueData));
			colorG.referTo(data.getValueColorGAsValue(valueData));
			colorB.referTo(data.getValueColorBAsValue(valueData));
		}

		void paint(Graphics& g) override {
			ValueEditorPropertyComponent::paint(g);

			const uint8 R = (float) colorR.getValue() * 255;
			const uint8 G = (float) colorG.getValue() * 255;
			const uint8 B = (float) colorB.getValue() * 255;

			g.setColour(Colour(R, G, B));
			g.fillRect(getLookAndFeel().getPropertyComponentContentPosition(*this));
		}

		void mouseUp(const MouseEvent &event) override {
			if (!isEnabled()) return;

			const Rectangle<int> contentBounds = getLookAndFeel().getPropertyComponentContentPosition(*this);
			if (!contentBounds.contains(event.getPosition())) return;

			TimelineData& data = TimelineData::getTimelineData();
			data.getUndoManager().beginNewTransaction("Changed " + getName() + " Value");

			auto popupPicker = new ColorPickerComponent(colorR, colorG, colorB);
			popupPicker->setSize(300, 406);

			CallOutBox& callOutBox = CallOutBox::launchAsynchronously(popupPicker, localAreaToGlobal(contentBounds), nullptr);
			callOutBox.setDismissalMouseClicksAreAlwaysConsumed(true);
		}

		void valueChanged(Value& /*value*/) {
			// color values changed
			repaint();
		}

	private:
		Value colorR;
		Value colorG;
		Value colorB;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorEditorPropertyComponent)
};

ValueEditorPropertyComponent::ValueEditorPropertyComponent(const String& propertyName, int numberOfRows) :
	PropertyComponent(propertyName, numberOfRows * 25)
{
}

void ValueEditorPropertyComponent::refresh() {
}

void ValueEditorPropertyComponent::resized() {
	const int numChildren = getNumChildComponents();
	Rectangle<int> contentBounds = getLookAndFeel().getPropertyComponentContentPosition(*this);
	int childHeight = float(contentBounds.getHeight()) / numChildren;
	for (int i = 0; i < numChildren; i++) {
		getChildComponent(i)->setBounds(contentBounds.removeFromTop(childHeight));
	}
}

ValueEditorPropertyComponent* ValueEditorPropertyComponent::newValueEditorPropertyComponent(const String& propertyName, ValueTree valueData) {
	TimelineData& data = TimelineData::getTimelineData();
	if (data.isValueBool(valueData)) {
		return new BoolEditorPropertyComponent(propertyName, valueData);
	} else if (data.isValueFloat(valueData)) {
		return new FloatEditorPropertyComponent(propertyName, valueData);
	} else if (data.isValueVec2(valueData)) {
		return new Vec2EditorPropertyComponent(propertyName, valueData);
	} else if (data.isValueVec3(valueData)) {
		return new Vec3EditorPropertyComponent(propertyName, valueData);
	} else if (data.isValueVec4(valueData)) {
		return new Vec4EditorPropertyComponent(propertyName, valueData);
	} else if (data.isValueColor(valueData)) {
		return new ColorEditorPropertyComponent(propertyName, valueData);
	} else if (data.isValueQuat(valueData)) {
		return new Vec4EditorPropertyComponent(propertyName, valueData);
	}
	return new ValueEditorPropertyComponent(propertyName);
}
