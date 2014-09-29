#ifndef UNIFORMSBARCOMPONENT_H
#define UNIFORMSBARCOMPONENT_H

#include <juce>

class TimelineData;

class UniformsBarComponent :
	public Component,
	private ValueTree::Listener
{
	public:
		UniformsBarComponent();
		~UniformsBarComponent();

		void updateSize();
		void paint(Graphics& g) override;
		void mouseUp(const MouseEvent& event) override;

		// ValueTree::Listener callbacks
		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;
		void valueTreeRedirected(ValueTree& treeWhoWasRedirected) override;

		enum ColourIds{
			evenRowColourId = 0x2300301,
			oddRowColourId = 0x2300302,
			uniformTextColourId = 0x2300303,
			seperatorColourId = 0x2300304
		};

	private:
		TimelineData& data;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UniformsBarComponent)
};

#endif // UNIFORMSBARCOMPONENT_H
