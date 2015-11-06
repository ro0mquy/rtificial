#include "QuaternionEditor.h"

#include <Timeline/TimelineData.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

QuaternionEditor::QuaternionEditor(const String& transactionName_) :
	transactionName(transactionName_)
{
	quatX.addListener(this);
	quatY.addListener(this);
	quatZ.addListener(this);
	quatW.addListener(this);
}

QuaternionEditor::~QuaternionEditor() {
	quatX.removeListener(this);
	quatY.removeListener(this);
	quatZ.removeListener(this);
	quatW.removeListener(this);
}

void QuaternionEditor::setValueData(const Value& quatX_, const Value& quatY_, const Value& quatZ_, const Value& quatW_) {
	quatX.referTo(quatX_);
	quatY.referTo(quatY_);
	quatZ.referTo(quatZ_);
	quatW.referTo(quatW_);
}

void QuaternionEditor::paint(Graphics& g) {
	g.setColour(Colours::white);
	g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(.5f), 5.f, 1.f);
}

void QuaternionEditor::mouseDown(const MouseEvent& event) {
	TimelineData::getTimelineData().getUndoManager().beginNewTransaction(transactionName);
	orientation.mouseDown(event.position);
}

void QuaternionEditor::mouseDrag(const MouseEvent& event) {
	orientation.mouseDrag(event.position);

	const glm::quat& orientationQuat = orientation.getQuaternion();
	quatX = orientationQuat.x;
	quatY = orientationQuat.y;
	quatZ = orientationQuat.z;
	quatW = orientationQuat.w;
}

void QuaternionEditor::valueChanged(Value& /*value*/) {
	glm::quat& orientationQuat = orientation.getQuaternion();
	orientationQuat = glm::quat(quatW.getValue(), quatX.getValue(), quatY.getValue(), quatZ.getValue());
}
