#ifndef SECTIONMANAGER_H
#define SECTIONMANAGER_H

#include <juce>

class TimelineData;

namespace sectionTreeId {
	const Identifier section("section");
	const Identifier sectionName("sectionName");
	const Identifier sectionCollapsed("sectionCollapsed");
	const Identifier sectionsArray("sectionsArray");
	const Identifier uniformsArray("uniformsArray");
		const Identifier uniform("uniform");
			const Identifier uniformName("uniformName");
}

namespace SectionTypes {
	using SectionTree = ValueTree;
	using SectionsArray = ValueTree;
	using Section = ValueTree;
	using UniformsArray = ValueTree;
	using Uniform = ValueTree;
}

class SectionManager {
	public:
		SectionManager();

		void reloadAllUniforms();

		SectionTypes::UniformsArray getUniformsArray(SectionTypes::Section& section);
		int getNumUniforms(SectionTypes::Section& section);
		SectionTypes::Uniform getUniform(SectionTypes::Section& section, const int nthUniform);
		bool isUniform(const SectionTypes::Uniform& uniform);
		SectionTypes::Uniform& addUniform(SectionTypes::Section& section, SectionTypes::Uniform& uniform);
		SectionTypes::Uniform addUniform(SectionTypes::Section& section, const var& uniformName);
		SectionTypes::Uniform addUniform(const var& uniformName);
		SectionTypes::Uniform& addUniformUnchecked(SectionTypes::Section& section, SectionTypes::Uniform& uniform);
		void removeUniform(SectionTypes::Section& section, SectionTypes::Uniform& uniform);
		var getUniformName(const SectionTypes::Uniform& uniform);
		void setUniformName(SectionTypes::Uniform& uniform, const var& name);
		int compareUniforms(const SectionTypes::Uniform& first, const SectionTypes::Uniform& second);

		SectionTypes::SectionsArray getSectionsArray(SectionTypes::Section& parentSection);
		int getNumSections(SectionTypes::Section& parentSection);
		SectionTypes::Section getSection(SectionTypes::Section& parentSection, const int nthSection);
		SectionTypes::Section getSection(SectionTypes::Section& parentSection, const var& name);
		bool isSection(const SectionTypes::Section& section);
		SectionTypes::Section& addSection(SectionTypes::Section& parentSection, SectionTypes::Section& subSection);
		SectionTypes::Section addSection(SectionTypes::Section& parentSection, const var& sectionName, const var& sectionCollapsed);
		SectionTypes::Section& addSectionUnchecked(SectionTypes::Section& parentSection, SectionTypes::Section& subSection);
		void removeSection(SectionTypes::Section& parentSection, SectionTypes::Section& subSection);
		var getSectionName(const SectionTypes::Section& section);
		var getSectionCollapsed(const SectionTypes::Section& section);
		void setSectionName(SectionTypes::Section& section, const var& name);
		void setSectionCollapsed(SectionTypes::Section& section, const var& collapsed);
		int compareSections(const SectionTypes::Section& first, const SectionTypes::Section& second);
		SectionTypes::Section getSectionForUniformName(const var& uniformName);

	private:
		TimelineData& data;
		SectionTypes::Section rootSection;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SectionManager)
};

#endif // SECTIONMANAGER_H
