#include "SectionManager.h"

#include <Timeline/TimelineData.h>

SectionManager::SectionManager() :
	data(TimelineData::getTimelineData()),
	rootSection(sectionTreeId::section)
{
	reloadAllUniforms();
	data.addListenerToTree(this);
}

SectionManager::~SectionManager() {
	data.removeListenerFromTree(this);
}

void SectionManager::reloadAllUniforms() {
	// clear everything
	getSectionsArray(rootSection).removeAllChildren(nullptr);
	getUniformsArray(rootSection).removeAllChildren(nullptr);

	const int numUniforms = data.getNumUniforms();
	for (int i = 0; i < numUniforms; i++) {
		const var uniformName = data.getUniformName(data.getUniform(i));
		addUniform(uniformName);
	}
}


SectionTypes::UniformsArray SectionManager::getUniformsArray(SectionTypes::Section& section) {
	return section.getOrCreateChildWithName(sectionTreeId::uniformsArray, nullptr);
}

int SectionManager::getNumUniforms(SectionTypes::Section& section) {
	return getUniformsArray(section).getNumChildren();
}

SectionTypes::Uniform SectionManager::getUniform(SectionTypes::Section& section, const int nthUniform) {
	return getUniformsArray(section).getChild(nthUniform);
}

SectionTypes::Uniform SectionManager::getUniform(SectionTypes::Section& section, const var& name) {
	return getUniformsArray(section).getChildWithProperty(sectionTreeId::uniformName, name);
}

bool SectionManager::isUniform(const SectionTypes::Uniform& uniform) {
	bool isUniform = uniform.hasType(sectionTreeId::uniform);
	isUniform &= uniform.hasProperty(sectionTreeId::uniformName);
	return isUniform;
}

SectionTypes::Uniform& SectionManager::addUniform(SectionTypes::Section& section, SectionTypes::Uniform& uniform) {
	jassert(isUniform(uniform));
	addUniformUnchecked(section, uniform);
	return uniform;
}

SectionTypes::Uniform SectionManager::addUniform(SectionTypes::Section& section, const var& uniformName) {
	SectionTypes::Uniform uniform(sectionTreeId::uniform);
	setUniformName(uniform, uniformName);
	addUniformUnchecked(section, uniform);
	return uniform;
}

SectionTypes::Uniform SectionManager::addUniform(const var& uniformName) {
	SectionTypes::Section section = getSectionForUniformName(uniformName);
	return addUniform(section, uniformName);
}

SectionTypes::Uniform& SectionManager::addUniformUnchecked(SectionTypes::Section& section, SectionTypes::Uniform& uniform) {
	const int numUniforms = getNumUniforms(section);
	int sortedPosition = 0;
	for (; sortedPosition < numUniforms; sortedPosition++) {
		const SectionTypes::Uniform otherUniform = getUniform(section, sortedPosition);
		if (compareUniforms(uniform, otherUniform) < 0) {
			// break if the new uniform comes before the currently checked one
			break;
		}
		// if the new uniform comes last sortedPosition will contain numUniforms
		// and the new uniform will be added at the end
	}
	getUniformsArray(section).addChild(uniform, sortedPosition, nullptr);
	return uniform;
}

void SectionManager::removeUniform(SectionTypes::Section& section, SectionTypes::Uniform& uniform) {
	getUniformsArray(section).removeChild(uniform, nullptr);
}

void SectionManager::removeUniform(const var& uniformName) {
	SectionTypes::Section section = getSectionForUniformName(uniformName);
	SectionTypes::Uniform uniform = getUniform(section, uniformName);
	return removeUniform(section, uniform);
}

var SectionManager::getUniformName(const SectionTypes::Uniform& uniform) {
	return uniform.getProperty(sectionTreeId::uniformName);
}

void SectionManager::setUniformName(SectionTypes::Uniform& uniform, const var& name) {
	uniform.setProperty(sectionTreeId::uniformName, name, nullptr);
}

int SectionManager::compareUniforms(const SectionTypes::Uniform& first, const SectionTypes::Uniform& second) {
		const String firstName = getUniformName(first);
		const String secondName = getUniformName(second);
		return firstName.compareNatural(secondName);
}


SectionTypes::SectionsArray SectionManager::getSectionsArray(SectionTypes::Section& parentSection) {
	return parentSection.getOrCreateChildWithName(sectionTreeId::sectionsArray, nullptr);
}

int SectionManager::getNumSections(SectionTypes::Section& parentSection) {
	return getSectionsArray(parentSection).getNumChildren();
}

SectionTypes::Section SectionManager::getSection(SectionTypes::Section& parentSection, const int nthSection) {
	return getSectionsArray(parentSection).getChild(nthSection);
}

SectionTypes::Section SectionManager::getSection(SectionTypes::Section& parentSection, const var& name) {
	return getSectionsArray(parentSection).getChildWithProperty(sectionTreeId::sectionName, name);
}

bool SectionManager::isSection(const SectionTypes::Section& section) {
	bool isSection = section.hasType(sectionTreeId::section);
	isSection &= section.hasProperty(sectionTreeId::sectionName);
	isSection &= section.hasProperty(sectionTreeId::sectionCollapsed);
	return isSection;
}

SectionTypes::Section& SectionManager::addSection(SectionTypes::Section& parentSection, SectionTypes::Section& subSection) {
	jassert(isSection(subSection));
	addSectionUnchecked(parentSection, subSection);
	return subSection;
}

SectionTypes::Section SectionManager::addSection(SectionTypes::Section& parentSection, const var& sectionName, const var& sectionCollapsed) {
	SectionTypes::Section subSection(sectionTreeId::section);
	setSectionName(subSection, sectionName);
	setSectionCollapsed(subSection, sectionCollapsed);
	addSectionUnchecked(parentSection, subSection);
	return subSection;
}

SectionTypes::Section& SectionManager::addSectionUnchecked(SectionTypes::Section& parentSection, SectionTypes::Section& subSection) {
	const int numSections = getNumSections(parentSection);
	int sortedPosition = 0;
	for (; sortedPosition < numSections; sortedPosition++) {
		const SectionTypes::Section otherSubSection = getSection(parentSection, sortedPosition);
		if (compareSections(subSection, otherSubSection) < 0) {
			// break if the new section comes before the currently checked one
			break;
		}
		// if the new section comes last sortedPosition will contain numSections
		// and the new section will be added at the end
	}
	getSectionsArray(parentSection).addChild(subSection, sortedPosition, nullptr);
	return subSection;
}

void SectionManager::removeSection(SectionTypes::Section& parentSection, SectionTypes::Section& subSection) {
	getSectionsArray(parentSection).removeChild(subSection, nullptr);
}

var SectionManager::getSectionName(const SectionTypes::Section& section) {
	return section.getProperty(sectionTreeId::sectionName);
}

var SectionManager::getSectionCollapsed(const SectionTypes::Section& section) {
	return section.getProperty(sectionTreeId::sectionCollapsed);
}

void SectionManager::setSectionName(SectionTypes::Section& section, const var& name) {
	section.setProperty(sectionTreeId::sectionName, name, nullptr);
}

void SectionManager::setSectionCollapsed(SectionTypes::Section& section, const var& collapsed) {
	section.setProperty(sectionTreeId::sectionCollapsed, collapsed, nullptr);
}

int SectionManager::compareSections(const SectionTypes::Section& first, const SectionTypes::Section& second) {
		const String firstName = getSectionName(first);
		const String secondName = getSectionName(second);
		return firstName.compareNatural(secondName);
}

SectionTypes::Section SectionManager::getSectionForUniformName(const var& uniformName) {
	const String sectionCandidateName = uniformName.toString().upToFirstOccurrenceOf("_", false, false);
	ValueTree sectionCandidate = getSection(rootSection, var(sectionCandidateName));
	if (! sectionCandidate.isValid()) {
		sectionCandidate = addSection(rootSection, sectionCandidateName);
	}
	return sectionCandidate;
}

// ValueTree::Listener callbacks for TimelineData
void SectionManager::valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& /*property*/) {
}

void SectionManager::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenAdded) {
	if (data.isUniform(childWhichHasBeenAdded)) {
		// uniform was added
		// also add it to the manager
		const var uniformName = data.getUniformName(childWhichHasBeenAdded);
		addUniform(uniformName);
	}
}

void SectionManager::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenRemoved, int /*indexFromWhichChildWasRemoved*/) {
	if (data.isUniform(childWhichHasBeenRemoved)) {
		const var uniformName = data.getUniformName(childWhichHasBeenRemoved);
		removeUniform(uniformName);
	}
}

void SectionManager::valueTreeChildOrderChanged(ValueTree& /*parentTree*/, int /*oldIndex*/, int /*newIndex*/) {
}

void SectionManager::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

void SectionManager::valueTreeRedirected(ValueTree& /*treeWhoWasRedirected*/) {
	// always the root tree
	reloadAllUniforms();
}
