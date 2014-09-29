#include "ValueEditorPropertyComponent.h"
#include "TimelineData.h"
#include <ColorPicker/LabColor.h>
#include <ColorPicker/LabColorPicker.h>

static const double sliderRange = 1000.;
static const double sliderStepSize = .001;

class BoolEditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		BoolEditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 1)
		{
			useValueData(valueData);
			addAndMakeVisible(button);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			boolState.referTo(data.getValueBoolStateAsValue(valueData));
			button.getToggleStateValue().referTo(boolState);
		}

	private:
		Value boolState;
		ToggleButton button;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolEditorPropertyComponent)
};

class FloatEditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		FloatEditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 1),
			sliderX(Slider::IncDecButtons, Slider::TextBoxLeft)
		{
			sliderX.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderX.setRange(-sliderRange, sliderRange, sliderStepSize);
			sliderX.setMouseDragSensitivity(500 * sliderRange);
			useValueData(valueData);
			addAndMakeVisible(sliderX);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			floatX.referTo(data.getValueFloatXAsValue(valueData));
			sliderX.getValueObject().referTo(floatX);
		}

	private:
		Value floatX;
		Slider sliderX;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatEditorPropertyComponent)
};

class Vec2EditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		Vec2EditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 2),
			sliderX(Slider::IncDecButtons, Slider::TextBoxLeft),
			sliderY(Slider::IncDecButtons, Slider::TextBoxLeft)
		{
			sliderX.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderY.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderX.setRange(-sliderRange, sliderRange, sliderStepSize);
			sliderY.setRange(-sliderRange, sliderRange, sliderStepSize);
			sliderX.setMouseDragSensitivity(500 * sliderRange);
			sliderY.setMouseDragSensitivity(500 * sliderRange);
			useValueData(valueData);
			addAndMakeVisible(sliderX);
			addAndMakeVisible(sliderY);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			floatX.referTo(data.getValueVec2XAsValue(valueData));
			floatY.referTo(data.getValueVec2YAsValue(valueData));
			sliderX.getValueObject().referTo(floatX);
			sliderY.getValueObject().referTo(floatY);
		}

	private:
		Value floatX;
		Value floatY;
		Slider sliderX;
		Slider sliderY;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vec2EditorPropertyComponent)
};

class Vec3EditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		Vec3EditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 3),
			sliderX(Slider::IncDecButtons, Slider::TextBoxLeft),
			sliderY(Slider::IncDecButtons, Slider::TextBoxLeft),
			sliderZ(Slider::IncDecButtons, Slider::TextBoxLeft)
		{
			sliderX.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderY.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderZ.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderX.setRange(-sliderRange, sliderRange, sliderStepSize);
			sliderY.setRange(-sliderRange, sliderRange, sliderStepSize);
			sliderZ.setRange(-sliderRange, sliderRange, sliderStepSize);
			sliderX.setMouseDragSensitivity(500 * sliderRange);
			sliderY.setMouseDragSensitivity(500 * sliderRange);
			sliderZ.setMouseDragSensitivity(500 * sliderRange);
			useValueData(valueData);
			addAndMakeVisible(sliderX);
			addAndMakeVisible(sliderY);
			addAndMakeVisible(sliderZ);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			floatX.referTo(data.getValueVec3XAsValue(valueData));
			floatY.referTo(data.getValueVec3YAsValue(valueData));
			floatZ.referTo(data.getValueVec3ZAsValue(valueData));
			sliderX.getValueObject().referTo(floatX);
			sliderY.getValueObject().referTo(floatY);
			sliderZ.getValueObject().referTo(floatZ);
		}

	private:
		Value floatX;
		Value floatY;
		Value floatZ;
		Slider sliderX;
		Slider sliderY;
		Slider sliderZ;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vec3EditorPropertyComponent)
};

class Vec4EditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		Vec4EditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 4),
			sliderX(Slider::IncDecButtons, Slider::TextBoxLeft),
			sliderY(Slider::IncDecButtons, Slider::TextBoxLeft),
			sliderZ(Slider::IncDecButtons, Slider::TextBoxLeft),
			sliderW(Slider::IncDecButtons, Slider::TextBoxLeft)
		{
			sliderX.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderY.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderZ.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderW.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderX.setRange(-sliderRange, sliderRange, sliderStepSize);
			sliderY.setRange(-sliderRange, sliderRange, sliderStepSize);
			sliderZ.setRange(-sliderRange, sliderRange, sliderStepSize);
			sliderW.setRange(-sliderRange, sliderRange, sliderStepSize);
			sliderX.setMouseDragSensitivity(500 * sliderRange);
			sliderY.setMouseDragSensitivity(500 * sliderRange);
			sliderZ.setMouseDragSensitivity(500 * sliderRange);
			sliderW.setMouseDragSensitivity(500 * sliderRange);
			useValueData(valueData);
			addAndMakeVisible(sliderX);
			addAndMakeVisible(sliderY);
			addAndMakeVisible(sliderZ);
			addAndMakeVisible(sliderW);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			floatX.referTo(data.getValueVec4XAsValue(valueData));
			floatY.referTo(data.getValueVec4YAsValue(valueData));
			floatZ.referTo(data.getValueVec4ZAsValue(valueData));
			floatW.referTo(data.getValueVec4WAsValue(valueData));
			sliderX.getValueObject().referTo(floatX);
			sliderY.getValueObject().referTo(floatY);
			sliderZ.getValueObject().referTo(floatZ);
			sliderW.getValueObject().referTo(floatW);
		}

	private:
		Value floatX;
		Value floatY;
		Value floatZ;
		Value floatW;
		Slider sliderX;
		Slider sliderY;
		Slider sliderZ;
		Slider sliderW;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vec4EditorPropertyComponent)
};

class ColorEditorPropertyComponent : public ValueEditorPropertyComponent,
	private Value::Listener
{
	public:
		ColorEditorPropertyComponent(const String& name, ValueTree valueData) :
			ValueEditorPropertyComponent(name, 1),
			colorLab(LabColor::getLabColorFromFloatRGB(colorR.getValue(), colorG.getValue(), colorB.getValue()))
		{
			useValueData(valueData);
			colorR.addListener(this);
			colorG.addListener(this);
			colorB.addListener(this);
			colorLab.addListenerForLab(this);
		}

		void useValueData(ValueTree valueData) override {
			TimelineData& data = TimelineData::getTimelineData();
			colorR.referTo(data.getValueColorRAsValue(valueData));
			colorG.referTo(data.getValueColorGAsValue(valueData));
			colorB.referTo(data.getValueColorBAsValue(valueData));
		}

		void paint(Graphics& g) override {
			ValueEditorPropertyComponent::paint(g);
			g.setColour(colorLab.getSRGBColor());
			g.fillRect(getLookAndFeel().getPropertyComponentContentPosition(*this));
		}

		void mouseUp(const MouseEvent &event) override {
			if (!isEnabled()) return;

			const Rectangle<int> contentBounds = getLookAndFeel().getPropertyComponentContentPosition(*this);
			if (!contentBounds.contains(event.getPosition())) return;

			LabColorPicker* popupPicker = new LabColorPicker(colorLab);
			popupPicker->setSize(300, 426);

			CallOutBox::launchAsynchronously(popupPicker, localAreaToGlobal(contentBounds), nullptr);
		}

		void valueChanged(Value& value) {
			if (value.refersToSameSourceAs(colorR) ||
					value.refersToSameSourceAs(colorG) ||
					value.refersToSameSourceAs(colorB)) {
				//colorLab = LabColor::getLabColorFromFloatRGB(colorR.getValue(), colorG.getValue(), colorB.getValue());
			} else {
				Vector3D<float> vectorLab = colorLab.getLinearRGBVector3D();
				colorR = vectorLab.x;
				colorG = vectorLab.y;
				colorB = vectorLab.z;
			}
			repaint();
		}

	private:
		Value colorR;
		Value colorG;
		Value colorB;
		LabColor colorLab;

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
	}
	return new ValueEditorPropertyComponent(propertyName);
}
