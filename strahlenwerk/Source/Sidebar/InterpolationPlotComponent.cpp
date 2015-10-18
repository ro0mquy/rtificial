#include "InterpolationPlotComponent.h"

#include <vector>
#include <Timeline/TimelineData.h>
#include <Timeline/Interpolator.h>

InterpolationPlotComponent::InterpolationPlotComponent(ValueTree sequence_) :
	sequence(sequence_),
	data(TimelineData::getTimelineData())
{
	setBufferedToImage(true);
}

void InterpolationPlotComponent::paint(Graphics& g) {
	ValueTree uniform = data.getSequenceParentUniform(sequence);
	const String uniformType = data.getUniformType(uniform);
	Interpolator& interpolator = data.getInterpolator();
	const int duration = data.getSequenceDuration(sequence);
	const int width = g.getClipBounds().getWidth();
	const int height = g.getClipBounds().getHeight();

	if (uniformType == "float") {
		const int stepSize = 2 * duration / width; // sample every 2px
		const int padding = 10;

		Path graph;
		bool first = true;
		for (int t = 0; t <= duration; t += stepSize) {
			const ValueTree uniformState = interpolator.calculateInterpolatedState(sequence, t).first;
			const float y = data.getFloatFromValue(uniformState);

			if (first) {
				graph.startNewSubPath(t, y);
				first = false;
			} else {
				graph.lineTo(t, y);
			}
		}

		std::vector<Point<float>> keyPoints;
		const int numKeyframes = data.getNumKeyframes(sequence);
		for (int i = 0; i < numKeyframes; i++) {
			const ValueTree keyFrame = data.getKeyframe(sequence, i);
			const float t = data.getKeyframePosition(keyFrame);
			const float y = data.getValueFloatX(data.getKeyframeValue(keyFrame));
			keyPoints.push_back(Point<float>(t, y));
		}

		/*
		 * Transform the path to fit into the bounds. If the graph is a total line
		 * we cannot use scaleToFit() but have to translate the path to the
		 * center of the bounds.
		 */
		AffineTransform scaleGraphToFit;
		if (std::abs(graph.getBounds().getHeight()) < 0.00005f) {
			const float graphHeight = graph.getCurrentPosition().getY();
			scaleGraphToFit = AffineTransform::translation(0.0f, -graphHeight + height/2.0f);
		} else {
			scaleGraphToFit = graph.getTransformToScaleToFit(0, padding, width, height-2*padding, false);
		}
		scaleGraphToFit = scaleGraphToFit.followedBy(AffineTransform::verticalFlip(height));


		g.setColour(findColour(InterpolationPlotComponent::backgroundColourId));
		g.fillAll();

		g.setColour(findColour(InterpolationPlotComponent::axisLineColourId));
		Path tAxis;
		tAxis.startNewSubPath(0, 0);
		tAxis.lineTo(duration, 0);
		tAxis.applyTransform(scaleGraphToFit);
		g.strokePath(tAxis, PathStrokeType(1.0f));

		g.setColour(findColour(InterpolationPlotComponent::plotLineColourId));
		graph.applyTransform(scaleGraphToFit);
		g.strokePath(graph, PathStrokeType(1.0f));

		g.setColour(findColour(InterpolationPlotComponent::keyPointColourId));
		const float keyPointRadius = 3.0f;
		for (Point<float>& keyPoint : keyPoints) {
			keyPoint.applyTransform(scaleGraphToFit);
			g.fillEllipse(
				keyPoint.getX() - keyPointRadius,
				keyPoint.getY() - keyPointRadius,
				2 * keyPointRadius,
				2 * keyPointRadius
			);
		}
	} else if (uniformType == "vec2") {
		const int numSteps = 100;
		const int padding = 10;

		Path graph;
		bool first = true;
		for (int i = 0; i < numSteps; i++) {
			const ValueTree uniformState = interpolator.calculateInterpolatedState(sequence, (float) i / (numSteps-1) * duration).first;
			const float x = data.getValueVec2X(uniformState);
			const float y = data.getValueVec2Y(uniformState);

			if (first) {
				graph.startNewSubPath(x, y);
				first = false;
			} else {
				graph.lineTo(x, y);
			}
		}

		std::vector<Point<float>> keyPoints;
		const int numKeyframes = data.getNumKeyframes(sequence);
		for (int i = 0; i < numKeyframes; i++) {
			const ValueTree keyFrame = data.getKeyframe(sequence, i);
			const float x = data.getValueVec2X(data.getKeyframeValue(keyFrame));
			const float y = data.getValueVec2Y(data.getKeyframeValue(keyFrame));
			keyPoints.push_back(Point<float>(x, y));
		}

		/*
		 * Transform the path to fit into the bounds. If the graph is a total line
		 * we cannot use scaleToFit() but have to translate the path to the
		 * center of the bounds.
		 */
		AffineTransform scaleGraphToFit;
		const Rectangle<float> graphBounds = graph.getBounds();
		if (std::abs(graphBounds.getHeight()) < 0.00005f) {
			const float graphWidth = graphBounds.getX();
			const float graphHeight = graphBounds.getY();
			scaleGraphToFit = AffineTransform::translation(graphWidth - width/2.0f, -graphHeight + height/2.0f);
		} else {
			scaleGraphToFit = graph.getTransformToScaleToFit(padding, padding, width-2*padding, height-2*padding, false);
		}
		scaleGraphToFit = scaleGraphToFit.followedBy(AffineTransform::verticalFlip(height));


		g.setColour(findColour(InterpolationPlotComponent::backgroundColourId));
		g.fillAll();

		g.setColour(findColour(InterpolationPlotComponent::axisLineColourId));
		Path axis;
		axis.startNewSubPath(graphBounds.getX(), 0);
		axis.lineTo(graphBounds.getRight(), 0);
		axis.closeSubPath();
		axis.startNewSubPath(0, graphBounds.getY());
		axis.lineTo(0, graphBounds.getBottom());
		axis.closeSubPath();
		axis.applyTransform(scaleGraphToFit);
		g.strokePath(axis, PathStrokeType(1.0f));

		g.setColour(findColour(InterpolationPlotComponent::plotLineColourId));
		graph.applyTransform(scaleGraphToFit);
		g.strokePath(graph, PathStrokeType(1.0f));

		g.setColour(findColour(InterpolationPlotComponent::keyPointColourId));
		const float keyPointRadius = 3.0f;
		for (Point<float>& keyPoint : keyPoints) {
			keyPoint.applyTransform(scaleGraphToFit);
			g.fillEllipse(
				keyPoint.getX() - keyPointRadius,
				keyPoint.getY() - keyPointRadius,
				2 * keyPointRadius,
				2 * keyPointRadius
			);
		}
	}
}
