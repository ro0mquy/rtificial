#include "ValueEditorComponent.h"
#include "TreeIdentifiers.h"

class BoolEditorComponent : public Component {
	public:
		BoolEditorComponent() {
		}

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolEditorComponent)
};

class FloatEditorComponent : public Component {
	public:
		FloatEditorComponent() {
		}

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatEditorComponent)
};

class Vec2EditorComponent : public Component {
	public:
		Vec2EditorComponent() {
		}

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vec2EditorComponent)
};

class Vec3EditorComponent : public Component {
	public:
		Vec3EditorComponent() {
		}

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Vec3EditorComponent)
};

class ColorEditorComponent : public Component {
	public:
		ColorEditorComponent() {
		}

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorEditorComponent)
};

ValueEditorComponent::ValueEditorComponent(ValueTree valueData_) :
	valueData(valueData_)
{
	String valueType = valueData.getProperty(treeId::valueType);
	if (valueType.equalsIgnoreCase("bool")) {
		specificTypeEditor = new BoolEditorComponent();
	} else if (valueType.equalsIgnoreCase("float")) {
		specificTypeEditor = new FloatEditorComponent();
	} else if (valueType.equalsIgnoreCase("vec2")) {
		specificTypeEditor = new Vec2EditorComponent();
	} else if (valueType.equalsIgnoreCase("vec3")) {
		specificTypeEditor = new Vec3EditorComponent();
	} else if (valueType.equalsIgnoreCase("color")) {
		specificTypeEditor = new ColorEditorComponent();
	} else {
		specificTypeEditor = new BoolEditorComponent();
	}

	addAndMakeVisible(specificTypeEditor);
}

void ValueEditorComponent::resized() {
	specificTypeEditor->setBounds(getLocalBounds());
}
