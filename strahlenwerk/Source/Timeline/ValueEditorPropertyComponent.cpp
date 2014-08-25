#include "ValueEditorPropertyComponent.h"
#include "TreeIdentifiers.h"
#include "../ColorPicker/LabColor.h"
#include "../ColorPicker/LabColorPicker.h"

class BoolEditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		BoolEditorPropertyComponent(const String& name, Value boolState_) :
			ValueEditorPropertyComponent(name, 1),
			boolState(boolState_)
		{
			button.getToggleStateValue().referTo(boolState);
			addAndMakeVisible(button);
		}

	private:
		Value boolState;
		ToggleButton button;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolEditorPropertyComponent)
};

class FloatEditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		FloatEditorPropertyComponent(const String& name, Value floatX_) :
			ValueEditorPropertyComponent(name, 1),
			floatX(floatX_),
			sliderX(Slider::IncDecButtons, Slider::TextBoxLeft)
		{
			sliderX.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderX.setRange(-1., 1., .001);
			sliderX.getValueObject().referTo(floatX);
			addAndMakeVisible(sliderX);
		}

	private:
		Value floatX;
		Slider sliderX;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatEditorPropertyComponent)
};

class Vec2EditorPropertyComponent : public ValueEditorPropertyComponent {
	public:
		Vec2EditorPropertyComponent(const String& name, Value floatX_, Value floatY_) :
			ValueEditorPropertyComponent(name, 2),
			floatX(floatX_),
			floatY(floatY_),
			sliderX(Slider::IncDecButtons, Slider::TextBoxLeft),
			sliderY(Slider::IncDecButtons, Slider::TextBoxLeft)
		{
			sliderX.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderY.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderX.setRange(-1., 1., .001);
			sliderY.setRange(-1., 1., .001);
			sliderX.getValueObject().referTo(floatX);
			sliderY.getValueObject().referTo(floatY);
			addAndMakeVisible(sliderX);
			addAndMakeVisible(sliderY);
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
		Vec3EditorPropertyComponent(const String& name, Value floatX_, Value floatY_, Value floatZ_) :
			ValueEditorPropertyComponent(name, 3),
			floatX(floatX_),
			floatY(floatY_),
			floatZ(floatZ_),
			sliderX(Slider::IncDecButtons, Slider::TextBoxLeft),
			sliderY(Slider::IncDecButtons, Slider::TextBoxLeft),
			sliderZ(Slider::IncDecButtons, Slider::TextBoxLeft)
		{
			sliderX.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderY.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderZ.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderX.setRange(-1., 1., .001);
			sliderY.setRange(-1., 1., .001);
			sliderZ.setRange(-1., 1., .001);
			sliderX.getValueObject().referTo(floatX);
			sliderY.getValueObject().referTo(floatY);
			sliderZ.getValueObject().referTo(floatZ);
			addAndMakeVisible(sliderX);
			addAndMakeVisible(sliderY);
			addAndMakeVisible(sliderZ);
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

class ColorEditorPropertyComponent : public ValueEditorPropertyComponent,
	private Value::Listener
{
	public:
		ColorEditorPropertyComponent(const String& name, Value colorR_, Value colorG_, Value colorB_) :
			ValueEditorPropertyComponent(name, 1),
			colorR(colorR_),
			colorG(colorG_),
			colorB(colorB_),
			colorLab(LabColor::getLabColorFromFloatRGB(colorR.getValue(), colorG.getValue(), colorB.getValue()))
		{
			colorLab.addListenerForLab(this);
		}

		void paint(Graphics& g) override {
			ValueEditorPropertyComponent::paint(g);
			g.setColour(colorLab.getSRGBColor());
			g.fillRect(getLookAndFeel().getPropertyComponentContentPosition(*this));
		}

		void mouseUp(const MouseEvent &event) override {
			const Rectangle<int> contentBounds = getLookAndFeel().getPropertyComponentContentPosition(*this);
			if (!contentBounds.contains(event.getPosition())) return;

			LabColorPicker* popupPicker = new LabColorPicker(colorLab);
			popupPicker->setSize(300, 426);

			CallOutBox::launchAsynchronously(popupPicker, localAreaToGlobal(contentBounds), nullptr);
		}

		void valueChanged(Value& value) {
			Vector3D<float> vectorLab = colorLab.getLinearRGBVector3D();
			colorR = vectorLab.x;
			colorG = vectorLab.y;
			colorB = vectorLab.z;
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

PropertyComponent* ValueEditorPropertyComponent::newValueEditorPropertyComponent(const String& propertyName, ValueTree valueData) {
	String valueType = valueData.getProperty(treeId::valueType);
	if (valueType.equalsIgnoreCase("bool")) {
		Value boolState = valueData.getChildWithName(treeId::valueBool).getPropertyAsValue(treeId::valueBoolState, nullptr);
		return new BoolEditorPropertyComponent(propertyName, boolState);
	} else if (valueType.equalsIgnoreCase("float")) {
		Value floatX = valueData.getChildWithName(treeId::valueFloat).getPropertyAsValue(treeId::valueFloatX, nullptr);
		return new FloatEditorPropertyComponent(propertyName, floatX);
	} else if (valueType.equalsIgnoreCase("vec2")) {
		Value floatX = valueData.getChildWithName(treeId::valueVec2).getPropertyAsValue(treeId::valueVec2X, nullptr);
		Value floatY = valueData.getChildWithName(treeId::valueVec2).getPropertyAsValue(treeId::valueVec2Y, nullptr);
		return new Vec2EditorPropertyComponent(propertyName, floatX, floatY);
	} else if (valueType.equalsIgnoreCase("vec3")) {
		Value floatX = valueData.getChildWithName(treeId::valueVec3).getPropertyAsValue(treeId::valueVec3X, nullptr);
		Value floatY = valueData.getChildWithName(treeId::valueVec3).getPropertyAsValue(treeId::valueVec3Y, nullptr);
		Value floatZ = valueData.getChildWithName(treeId::valueVec3).getPropertyAsValue(treeId::valueVec3Z, nullptr);
		return new Vec3EditorPropertyComponent(propertyName, floatX, floatY, floatZ);
	} else if (valueType.equalsIgnoreCase("color")) {
		Value colorR = valueData.getChildWithName(treeId::valueColor).getPropertyAsValue(treeId::valueColorR, nullptr);
		Value colorG = valueData.getChildWithName(treeId::valueColor).getPropertyAsValue(treeId::valueColorG, nullptr);
		Value colorB = valueData.getChildWithName(treeId::valueColor).getPropertyAsValue(treeId::valueColorB, nullptr);
		return new ColorEditorPropertyComponent(propertyName, colorR, colorG, colorB);
	}
	return new TextPropertyComponent(Value(), propertyName, 30, false);
}
