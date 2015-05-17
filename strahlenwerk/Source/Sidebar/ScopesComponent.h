#ifndef SCOPESCOMPONENT_H
#define SCOPESCOMPONENT_H

#include <juce>

class ScopesComponent :
	public Component
{
	public:
		ScopesComponent();
		~ScopesComponent();

	private:
		void paint(Graphics& g) override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScopesComponent)
};

#endif // SCOPESCOMPONENT_H
