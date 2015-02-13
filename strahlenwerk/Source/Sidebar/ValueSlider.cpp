#include "ValueSlider.h"

#include <Timeline/TimelineData.h>

static const double sliderRange = 1000.;
static const double sliderStepSize = .001;

ValueSlider::ValueSlider(const String transactionName_) :
	Slider(Slider::IncDecButtons, Slider::TextBoxLeft),
	transactionName(transactionName_)
{
	setIncDecButtonsMode(Slider::incDecButtonsDraggable_Horizontal);
	setRange(-sliderRange, sliderRange, sliderStepSize);
	setMouseDragSensitivity(500 * sliderRange);
	valueData.addListener(this);
}

ValueSlider::~ValueSlider() {
	valueData.removeListener(this);
}

void ValueSlider::setValueData(const Value& valueData_) {
	valueData.referTo(valueData_);
}

void ValueSlider::startedDragging() {
	TimelineData& data = TimelineData::getTimelineData();
	data.getUndoManager().beginNewTransaction(transactionName);
}

void ValueSlider::valueChanged() {
	// slider changed
	valueData = getValue();
}

void ValueSlider::valueChanged(Value& /*value*/) {
	// valueData changed
	setValue(valueData.getValue(), NotificationType::dontSendNotification);
}
