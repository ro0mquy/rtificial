#ifndef UNIFORMSBARCOMPONENT_H
#define UNIFORMSBARCOMPONENT_H

#include <juce>
#include "SelfResizingComponent.h"
#include <Timeline/SectionManager.h>

class TimelineData;

class UniformsBarComponent :
	public Component,
	public SelfResizingComponent,
	private ValueTree::Listener
{
	public:
		UniformsBarComponent(SectionManager& sectionManager_);
		~UniformsBarComponent();

		void updateSize() override;
		void paint(Graphics& g) override;
		void mouseUp(const MouseEvent& event) override;

		// ValueTree::Listener callbacks
		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree, int oldIndex, int newIndex) override;
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
		SectionManager& sectionManager;

		void drawSection(Graphics& g, SectionTypes::Section& section, juce::Rectangle<int>& targetBounds) const;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UniformsBarComponent)
};

#endif // UNIFORMSBARCOMPONENT_H
