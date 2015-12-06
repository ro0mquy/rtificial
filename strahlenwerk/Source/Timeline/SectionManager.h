#ifndef SECTIONMANAGER_H
#define SECTIONMANAGER_H

#include <juce>

class TimelineData;

namespace sectionTreeId {
	// the whole tree is build out of sections, each section can hold uniforms and subsections
	const Identifier section("section");
		// the section name
		const Identifier sectionName("sectionName");
		// true if the section is collapsed
		const Identifier sectionCollapsed("sectionCollapsed");
		// the array for the subsections
		const Identifier sectionsArray("sectionsArray");
		// the array holding the uniform in this section
		const Identifier uniformsArray("uniformsArray");
			// an individual uniform, data is reduced to just the name
			const Identifier uniform("uniform");
				// the uniform name
				const Identifier uniformName("uniformName");
}

// the different types of ValueTrees, named like their identifiers
namespace SectionTypes {
	using SectionsArray = ValueTree;
	using Section = ValueTree;
	using UniformsArray = ValueTree;
	using Uniform = ValueTree;
}

class SectionManager :
	private ValueTree::Listener
{
	public:
		SectionManager();
		~SectionManager();

		void reloadAllUniforms();
		int getTotalHeightInRows();
		int getTotalHeightInRows(SectionTypes::Section section);
		int getUniformYPosInRows(const var& uniformName);

		SectionTypes::UniformsArray getUniformsArray(SectionTypes::Section& section);
		int getNumUniforms(SectionTypes::Section& section);
		SectionTypes::Uniform getUniform(SectionTypes::Section& section, const int nthUniform);
		SectionTypes::Uniform getUniform(SectionTypes::Section& section, const var& name);
		bool isUniform(const SectionTypes::Uniform& uniform);
		SectionTypes::Uniform& addUniform(SectionTypes::Section& section, SectionTypes::Uniform& uniform);
		SectionTypes::Uniform addUniform(SectionTypes::Section& section, const var& uniformName);
		SectionTypes::Uniform addUniform(const var& uniformName);
		SectionTypes::Uniform& addUniformUnchecked(SectionTypes::Section& section, SectionTypes::Uniform& uniform);
		void removeUniform(SectionTypes::Section& section, SectionTypes::Uniform& uniform);
		void removeUniform(const var& uniformName);
		var getUniformName(const SectionTypes::Uniform& uniform);
		void setUniformName(SectionTypes::Uniform& uniform, const var& name);
		int getUniformIndex(SectionTypes::Uniform& uniform);
		int compareUniforms(const SectionTypes::Uniform& first, const SectionTypes::Uniform& second);

		SectionTypes::SectionsArray getSectionsArray(SectionTypes::Section& parentSection);
		int getNumSections(SectionTypes::Section& parentSection);
		SectionTypes::Section getSection(SectionTypes::Section& parentSection, const int nthSection);
		SectionTypes::Section getSection(SectionTypes::Section& parentSection, const var& name);
		bool isSection(const SectionTypes::Section& section);
		SectionTypes::Section& addSection(SectionTypes::Section& parentSection, SectionTypes::Section& subSection);
		SectionTypes::Section addSection(SectionTypes::Section& parentSection, const var& sectionName, const var& sectionCollapsed = false);
		SectionTypes::Section& addSectionUnchecked(SectionTypes::Section& parentSection, SectionTypes::Section& subSection);
		void removeSection(SectionTypes::Section& parentSection, SectionTypes::Section& subSection);
		var getSectionName(const SectionTypes::Section& section);
		var getSectionCollapsed(const SectionTypes::Section& section);
		void setSectionName(SectionTypes::Section& section, const var& name);
		void setSectionCollapsed(SectionTypes::Section& section, const var& collapsed);
		SectionTypes::Section getSectionParentSection(SectionTypes::Section& subsection);
		int getSectionIndex(SectionTypes::Section& section);
		int compareSections(const SectionTypes::Section& first, const SectionTypes::Section& second);
		SectionTypes::Section getSectionForUniformName(const var& uniformName);
		SectionTypes::Section& getRootSection();

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree, int oldIndex, int newIndex) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;
		void valueTreeRedirected(ValueTree& treeWhoWasRedirected) override;

	private:
		TimelineData& data;
		SectionTypes::Section rootSection;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SectionManager)
};

#endif // SECTIONMANAGER_H
