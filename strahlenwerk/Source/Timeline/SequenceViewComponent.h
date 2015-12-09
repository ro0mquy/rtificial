#ifndef SEQUENCEVIEWCOMPONENT_H
#define SEQUENCEVIEWCOMPONENT_H

#include <juce>
#include "SelfResizingComponent.h"
#include "MouseCallbackClasses.h"
#include "TimeMarkerComponent.h"
#include <Timeline/SectionManager.h>

class TimelineData;
class AudioManager;
class ZoomFactor;
class SequenceComponent;

class SequenceViewComponent :
	public McbComponent,
	public SelfResizingComponent,
	private ValueTree::Listener,
	private ChangeListener
{
	public:
		SequenceViewComponent(SectionManager& sectionManager_, ZoomFactor& zoomFactor_);
		~SequenceViewComponent();

		void updateSize() override;
		void paint(Graphics& g) override;
		void addSequenceComponent(ValueTree sequenceData);
		void addSequenceComponent(ValueTree sequenceData, const int rowHeight);
		void addAllSequenceComponents();
		SequenceComponent* getSequenceComponentForData(ValueTree sequenceData);

		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;

		void changeListenerCallback(ChangeBroadcaster* source) override;

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree, int oldIndex, int newIndex) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;
		void valueTreeRedirected(ValueTree& treeWhoWasRedirected) override;

		enum ColourIds{
			evenRowColourId = 0x2300401,
			oddRowColourId,
			inactiveAreaColourId,
			uniformSeperatorColourId,
			sectionHeaderSeperatorColourId,
		};

	private:
		bool uniformActiveForScene(ValueTree uniform, ValueTree scene);
		void drawSectionHeader(Graphics& g, SectionTypes::Section& section, Rectangle<int>& targetBounds) const;

		TimelineData& data;
		SectionManager& sectionManager;
		ZoomFactor& zoomFactor;
		TimelineTimeMarkerComponent timeMarker;
		OwnedArray<SequenceComponent> sequenceComponentsArray;

		ValueTree currentlyCreatedSequenceData;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceViewComponent)
};

#endif // SEQUENCEVIEWCOMPONENT_H
