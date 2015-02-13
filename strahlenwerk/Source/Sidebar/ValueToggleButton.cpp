#include "ValueToggleButton.h"

#include <Timeline/TimelineData.h>

ValueToggleButton::ValueToggleButton(const String transactionName_) :
	transactionName(transactionName_)
{
	valueData.addListener(this);
}

ValueToggleButton::~ValueToggleButton() {
	valueData.removeListener(this);
}

void ValueToggleButton::setValueData(const Value& valueData_) {
	valueData.referTo(valueData_);
}

void ValueToggleButton::clicked() {
	TimelineData& data = TimelineData::getTimelineData();
	data.getUndoManager().beginNewTransaction(transactionName);
	valueData = getToggleState();
}

void ValueToggleButton::valueChanged(Value& /*value*/) {
	// valueData changed
	setToggleState(valueData.getValue(), NotificationType::dontSendNotification);
}
