#include "QuaternionEditor.h"

#include <Timeline/TimelineData.h>

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

void QuaternionEditor::resized() {
	orientation.setViewport(getLocalBounds());
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

	Draggable3DOrientation::QuaternionType& juceQuat = orientation.getQuaternion();
	quatX = juceQuat.vector.x;
	quatY = juceQuat.vector.y;
	quatZ = juceQuat.vector.z;
	quatW = juceQuat.scalar;
}

void QuaternionEditor::valueChanged(Value& /*value*/) {
	Draggable3DOrientation::QuaternionType& juceQuat = orientation.getQuaternion();
	juceQuat = Draggable3DOrientation::QuaternionType(quatX.getValue(), quatY.getValue(), quatZ.getValue(), quatW.getValue());
}
