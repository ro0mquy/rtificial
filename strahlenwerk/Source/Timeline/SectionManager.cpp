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

// completly reloads the whole manager
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

int SectionManager::getTotalHeightInRows() {
	return getTotalHeightInRows(rootSection) - 1; // ignore header for root section
}

int SectionManager::getTotalHeightInRows(SectionTypes::Section& section) {
	int totalHeight = 1; // header is always visible
	if (getSectionCollapsed(section)) {
		return totalHeight;
	}

	totalHeight += getNumUniforms(section);

	const int numSections = getNumSections(section);
	for (int i = 0; i < numSections; i++) {
		SectionTypes::Section subsection = getSection(section, i);
		totalHeight += getTotalHeightInRows(subsection);
	}

	return totalHeight;
}


// retrieves the uniformsArray of a section
SectionTypes::UniformsArray SectionManager::getUniformsArray(SectionTypes::Section& section) {
	return section.getOrCreateChildWithName(sectionTreeId::uniformsArray, nullptr);
}

// returns the number of uniforms in a section
int SectionManager::getNumUniforms(SectionTypes::Section& section) {
	return getUniformsArray(section).getNumChildren();
}

// gets the uniform with index nthUniform
// returns invalid ValueTree if out of bounds
SectionTypes::Uniform SectionManager::getUniform(SectionTypes::Section& section, const int nthUniform) {
	return getUniformsArray(section).getChild(nthUniform);
}

// gets the Uniform with corresponding name
// returns invalid ValueTree if no uniform matches
SectionTypes::Uniform SectionManager::getUniform(SectionTypes::Section& section, const var& name) {
	return getUniformsArray(section).getChildWithProperty(sectionTreeId::uniformName, name);
}

// checks whether the given valueTree is a uniform
bool SectionManager::isUniform(const SectionTypes::Uniform& uniform) {
	bool isUniform = uniform.hasType(sectionTreeId::uniform);
	isUniform &= uniform.hasProperty(sectionTreeId::uniformName);
	return isUniform;
}

// adds a uniform to the section at a sorted position
// returns the uniform again
SectionTypes::Uniform& SectionManager::addUniform(SectionTypes::Section& section, SectionTypes::Uniform& uniform) {
	jassert(isUniform(uniform));
	addUniformUnchecked(section, uniform);
	return uniform;
}

// adds a uniform with the given name at a sorted position
// returns the assembled uniform
SectionTypes::Uniform SectionManager::addUniform(SectionTypes::Section& section, const var& uniformName) {
	SectionTypes::Uniform uniform(sectionTreeId::uniform);
	setUniformName(uniform, uniformName);
	addUniformUnchecked(section, uniform);
	return uniform;
}

// adds a uniform with the given name at a sorted position
// the section is determined from the name
// returns the assembled uniform
SectionTypes::Uniform SectionManager::addUniform(const var& uniformName) {
	SectionTypes::Section section = getSectionForUniformName(uniformName);
	return addUniform(section, uniformName);
}

// adds a uniform to the uniforms array at a sorted position
// returns the uniform again
// doesn't perform any checking (you should use addUniform())
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

// removes a uniform from the uniformsArray of a section
void SectionManager::removeUniform(SectionTypes::Section& section, SectionTypes::Uniform& uniform) {
	getUniformsArray(section).removeChild(uniform, nullptr);
}

// removes a uniform from the section that is determined by the given name
void SectionManager::removeUniform(const var& uniformName) {
	SectionTypes::Section section = getSectionForUniformName(uniformName);
	SectionTypes::Uniform uniform = getUniform(section, uniformName);
	return removeUniform(section, uniform);
}

// gets the name of a uniform
var SectionManager::getUniformName(const SectionTypes::Uniform& uniform) {
	return uniform.getProperty(sectionTreeId::uniformName);
}

// sets the name of a uniform
void SectionManager::setUniformName(SectionTypes::Uniform& uniform, const var& name) {
	uniform.setProperty(sectionTreeId::uniformName, name, nullptr);
}

// comparator function for uniforms
// normal comparator convention:
// < 0: first < second; == 0: first == second; > 0: first > second
int SectionManager::compareUniforms(const SectionTypes::Uniform& first, const SectionTypes::Uniform& second) {
		const String firstName = getUniformName(first);
		const String secondName = getUniformName(second);
		return firstName.compareNatural(secondName);
}


// retrieves the sectionsArray of a parent section
SectionTypes::SectionsArray SectionManager::getSectionsArray(SectionTypes::Section& parentSection) {
	return parentSection.getOrCreateChildWithName(sectionTreeId::sectionsArray, nullptr);
}

// returns the number of subsections in a parent section
int SectionManager::getNumSections(SectionTypes::Section& parentSection) {
	return getSectionsArray(parentSection).getNumChildren();
}

// gets the subsection with index nthSection
// returns invalid ValueTree if out of bounds
SectionTypes::Section SectionManager::getSection(SectionTypes::Section& parentSection, const int nthSection) {
	return getSectionsArray(parentSection).getChild(nthSection);
}

// returns the subsection with the given name
// returns invalid ValueTree if no subsection with the name exists
SectionTypes::Section SectionManager::getSection(SectionTypes::Section& parentSection, const var& name) {
	return getSectionsArray(parentSection).getChildWithProperty(sectionTreeId::sectionName, name);
}

// checks if the Tree is a section
bool SectionManager::isSection(const SectionTypes::Section& section) {
	bool isSection = section.hasType(sectionTreeId::section);
	isSection &= section.hasProperty(sectionTreeId::sectionName);
	isSection &= section.hasProperty(sectionTreeId::sectionCollapsed);
	return isSection;
}

// adds a section to a parent section at a sorted position
// returns the subsection again
SectionTypes::Section& SectionManager::addSection(SectionTypes::Section& parentSection, SectionTypes::Section& subSection) {
	jassert(isSection(subSection));
	addSectionUnchecked(parentSection, subSection);
	return subSection;
}

// adds a section with the given vars at a sorted position
// returns the assembled section
// sectionCollapsed defaults to false
SectionTypes::Section SectionManager::addSection(SectionTypes::Section& parentSection, const var& sectionName, const var& sectionCollapsed) {
	SectionTypes::Section subSection(sectionTreeId::section);
	setSectionName(subSection, sectionName);
	setSectionCollapsed(subSection, sectionCollapsed);
	addSectionUnchecked(parentSection, subSection);
	return subSection;
}

// adds a section to the sections array of a parent section at a sorted position
// returns the section again
// doesn't perform any checking (you should use addSection())
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

// removes a section from its parent section
void SectionManager::removeSection(SectionTypes::Section& parentSection, SectionTypes::Section& subSection) {
	getSectionsArray(parentSection).removeChild(subSection, nullptr);
}

// gets the name of a section
var SectionManager::getSectionName(const SectionTypes::Section& section) {
	return section.getProperty(sectionTreeId::sectionName);
}

// gets the collapsed state of a section
var SectionManager::getSectionCollapsed(const SectionTypes::Section& section) {
	return section.getProperty(sectionTreeId::sectionCollapsed);
}

// sets the name of a section
void SectionManager::setSectionName(SectionTypes::Section& section, const var& name) {
	section.setProperty(sectionTreeId::sectionName, name, nullptr);
}

// sets the collapsed state of a section
void SectionManager::setSectionCollapsed(SectionTypes::Section& section, const var& collapsed) {
	section.setProperty(sectionTreeId::sectionCollapsed, collapsed, nullptr);
}

// comparator function for sections
// normal comparator convention:
// < 0: first < second; == 0: first == second; > 0: first > second
int SectionManager::compareSections(const SectionTypes::Section& first, const SectionTypes::Section& second) {
		const String firstName = getSectionName(first);
		const String secondName = getSectionName(second);
		return firstName.compareNatural(secondName);
}

// returns the section that a uniform with this name should be added
SectionTypes::Section SectionManager::getSectionForUniformName(const var& uniformName) {
	const String sectionCandidateName = uniformName.toString().upToFirstOccurrenceOf("_", false, false);
	ValueTree sectionCandidate = getSection(rootSection, var(sectionCandidateName));
	if (! sectionCandidate.isValid()) {
		sectionCandidate = addSection(rootSection, sectionCandidateName);
	}
	return sectionCandidate;
}

// returns the root section
SectionTypes::Section& SectionManager::getRootSection() {
	return rootSection;
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
		// uniform was removed
		// also remove it from the manager
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
