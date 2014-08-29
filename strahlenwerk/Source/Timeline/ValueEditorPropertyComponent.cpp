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
			colorR.addListener(this);
			colorG.addListener(this);
			colorB.addListener(this);
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
			if(value == colorR || value == colorG || value == colorB) {
				// ignore if this class changed the value
				return;
			}
			if (value.refersToSameSourceAs(colorR) ||
					value.refersToSameSourceAs(colorG) ||
					value.refersToSameSourceAs(colorB)) {
				colorLab = LabColor::getLabColorFromFloatRGB(colorR.getValue(), colorG.getValue(), colorB.getValue());
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

PropertyComponent* ValueEditorPropertyComponent::newValueEditorPropertyComponent(const String& propertyName, ValueTree valueData) {
	if (valueData.hasProperty(treeId::valueBoolState)) {
		Value boolState = valueData.getPropertyAsValue(treeId::valueBoolState, nullptr);
		return new BoolEditorPropertyComponent(propertyName, boolState);
	} else if (valueData.hasProperty(treeId::valueFloatX)) {
		Value floatX = valueData.getPropertyAsValue(treeId::valueFloatX, nullptr);
		return new FloatEditorPropertyComponent(propertyName, floatX);
	} else if (valueData.hasProperty(treeId::valueVec2X)) {
		Value floatX = valueData.getPropertyAsValue(treeId::valueVec2X, nullptr);
		Value floatY = valueData.getPropertyAsValue(treeId::valueVec2Y, nullptr);
		return new Vec2EditorPropertyComponent(propertyName, floatX, floatY);
	} else if (valueData.hasProperty(treeId::valueVec3X)) {
		Value floatX = valueData.getPropertyAsValue(treeId::valueVec3X, nullptr);
		Value floatY = valueData.getPropertyAsValue(treeId::valueVec3Y, nullptr);
		Value floatZ = valueData.getPropertyAsValue(treeId::valueVec3Z, nullptr);
		return new Vec3EditorPropertyComponent(propertyName, floatX, floatY, floatZ);
	} else if (valueData.hasProperty(treeId::valueColorR)) {
		Value colorR = valueData.getPropertyAsValue(treeId::valueColorR, nullptr);
		Value colorG = valueData.getPropertyAsValue(treeId::valueColorG, nullptr);
		Value colorB = valueData.getPropertyAsValue(treeId::valueColorB, nullptr);
		return new ColorEditorPropertyComponent(propertyName, colorR, colorG, colorB);
	}
	return new TextPropertyComponent(Value(propertyName), propertyName, 30, false);
}
