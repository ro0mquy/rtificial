#include "ValueEditorComponent.h"
#include "TreeIdentifiers.h"
#include "../ColorPicker/LabColor.h"
#include "../ColorPicker/LabColorPicker.h"

class BoolEditorComponent : public Component {
	public:
		BoolEditorComponent(Value boolState_) :
			boolState(boolState_)
		{
			button.getToggleStateValue().referTo(boolState);
			addAndMakeVisible(button);
		}

		void resized() override {
			button.setBounds(getLocalBounds());
		}

	private:
		Value boolState;
		ToggleButton button;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolEditorComponent)
};

class FloatEditorComponent : public Component {
	public:
		FloatEditorComponent(Value floatX_) :
			floatX(floatX_),
			sliderX(Slider::IncDecButtons, Slider::TextBoxLeft)
		{
			sliderX.setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
			sliderX.setRange(-1., 1., .001);
			sliderX.getValueObject().referTo(floatX);
			addAndMakeVisible(sliderX);
		}

		void resized() override {
			sliderX.setBounds(getLocalBounds());
		}

	private:
		Value floatX;
		Slider sliderX;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatEditorComponent)
};

class Vec2EditorComponent : public Component {
	public:
		Vec2EditorComponent(Value floatX_, Value floatY_) :
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

		void resized() override {
			sliderX.setBoundsRelative(0., 0., 1., .5);
			sliderY.setBoundsRelative(0., .5, 1., .5);
		}

	private:
		Value floatX;
		Value floatY;
		Slider sliderX;
		Slider sliderY;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vec2EditorComponent)
};

class Vec3EditorComponent : public Component {
	public:
		Vec3EditorComponent(Value floatX_, Value floatY_, Value floatZ_) :
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

		void resized() override {
			sliderX.setBoundsRelative(0., 0./3., 1., 1./3.);
			sliderY.setBoundsRelative(0., 1./3., 1., 1./3.);
			sliderZ.setBoundsRelative(0., 2./3., 1., 1./3.);
		}

	private:
		Value floatX;
		Value floatY;
		Value floatZ;
		Slider sliderX;
		Slider sliderY;
		Slider sliderZ;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vec3EditorComponent)
};

class ColorEditorComponent : public Component,
							 private Value::Listener
{
	public:
		ColorEditorComponent(Value colorR_, Value colorG_, Value colorB_) :
			colorR(colorR_),
			colorG(colorG_),
			colorB(colorB_),
			colorLab(LabColor::getLabColorFromFloatRGB(colorR.getValue(), colorG.getValue(), colorB.getValue()))
		{
			colorLab.addListenerForLab(this);
		}

		void paint(Graphics& g) override {
			g.fillAll(colorLab.getSRGBColor());
		}

		void mouseUp(const MouseEvent &event) override {
			LabColorPicker* popupPicker = new LabColorPicker(colorLab);
			popupPicker->setSize(300, 426);

			CallOutBox::launchAsynchronously(popupPicker, getScreenBounds(), nullptr);
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

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorEditorComponent)
};

ValueEditorComponent::ValueEditorComponent(ValueTree valueData_) :
	valueData(valueData_)
{
	String valueType = valueData.getProperty(treeId::valueType);
	if (valueType.equalsIgnoreCase("bool")) {
		Value boolState = valueData.getChildWithName(treeId::valueBool).getPropertyAsValue(treeId::valueBoolState, nullptr);
		specificTypeEditor = new BoolEditorComponent(boolState);
	} else if (valueType.equalsIgnoreCase("float")) {
		Value floatX = valueData.getChildWithName(treeId::valueFloat).getPropertyAsValue(treeId::valueFloatX, nullptr);
		specificTypeEditor = new FloatEditorComponent(floatX);
	} else if (valueType.equalsIgnoreCase("vec2")) {
		Value floatX = valueData.getChildWithName(treeId::valueVec2).getPropertyAsValue(treeId::valueVec2X, nullptr);
		Value floatY = valueData.getChildWithName(treeId::valueVec2).getPropertyAsValue(treeId::valueVec2Y, nullptr);
		specificTypeEditor = new Vec2EditorComponent(floatX, floatY);
	} else if (valueType.equalsIgnoreCase("vec3")) {
		Value floatX = valueData.getChildWithName(treeId::valueVec3).getPropertyAsValue(treeId::valueVec3X, nullptr);
		Value floatY = valueData.getChildWithName(treeId::valueVec3).getPropertyAsValue(treeId::valueVec3Y, nullptr);
		Value floatZ = valueData.getChildWithName(treeId::valueVec3).getPropertyAsValue(treeId::valueVec3Z, nullptr);
		specificTypeEditor = new Vec3EditorComponent(floatX, floatY, floatZ);
	} else if (valueType.equalsIgnoreCase("color")) {
		Value colorR = valueData.getChildWithName(treeId::valueColor).getPropertyAsValue(treeId::valueColorR, nullptr);
		Value colorG = valueData.getChildWithName(treeId::valueColor).getPropertyAsValue(treeId::valueColorG, nullptr);
		Value colorB = valueData.getChildWithName(treeId::valueColor).getPropertyAsValue(treeId::valueColorB, nullptr);
		specificTypeEditor = new ColorEditorComponent(colorR, colorG, colorB);
	} else {
		specificTypeEditor = new Component();
	}

	addAndMakeVisible(specificTypeEditor);
}

void ValueEditorComponent::resized() {
	specificTypeEditor->setBounds(getLocalBounds());
}
