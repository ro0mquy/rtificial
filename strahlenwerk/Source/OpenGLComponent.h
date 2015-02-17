#ifndef OPENGLCOMPONENT_H
#define OPENGLCOMPONENT_H

#include <juce>

#include "Renderer.h"
#include "OpenGLTargetComponent.h"
#include <Project/Project.h>

class OpenGLComponent :
	public Component,
	private ApplicationCommandManagerListener,
	private ValueTree::Listener,
	private ChangeListener,
	private Project::Listener
{
	public:
		OpenGLComponent();
		~OpenGLComponent();

		void resized() override;
		void paint(Graphics& g) override;
		void applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) override;
		void applicationCommandListChanged() override;
		void changeListenerCallback(ChangeBroadcaster* source) override;
		void postprocChanged() override;
		void scenesChanged() override;
		void doToggleGrid();

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;
		void valueTreeRedirected(ValueTree& treeWhoWasRedirected) override;

		enum CommandIDs {
			toggleGrid = 0x7f81da00,
		};

	private:
		OpenGLContext context;
		Renderer renderer;
		OpenGLTargetComponent fixedAspectRatioComponent;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLComponent)
};

#endif
