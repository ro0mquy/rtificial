#include "DraggableQuaternion.h"

using VectorType = DraggableQuaternion::VectorType;
using QuaternionType = DraggableQuaternion::QuaternionType;

DraggableQuaternion::DraggableQuaternion(float objectRadius) noexcept :
	radius (jmax (0.1f, objectRadius)),
	quaternion (VectorType::xAxis(), 0)
{
}

DraggableQuaternion::DraggableQuaternion(const Quaternion<GLfloat>& quaternionToUse, float objectRadius) noexcept :
	radius(jmax(0.1f, objectRadius)),
	quaternion(quaternionToUse)
{
}

void DraggableQuaternion::reset(const VectorType& axis) noexcept {
	quaternion = QuaternionType(axis, 0);
}

void DraggableQuaternion::setViewport(const Rectangle<int>& newArea) noexcept {
	area = newArea;
}

void DraggableQuaternion::setRadius(float newRadius) noexcept {
	radius = jmax(0.1f, newRadius);
}

void DraggableQuaternion::mouseDown(Point<float> mousePos) noexcept {
	origMouse = mousePosToProportion(mousePos);
	origQuat = quaternion.normalised();
}

void DraggableQuaternion::mouseDrag(Point<float> mousePos) noexcept {
	const Point<float> currentMouse = mousePosToProportion(mousePos);

	QuaternionType newQuat(origQuat);
	newQuat *= rotationFromMove(origMouse, currentMouse);;
	quaternion = newQuat;
}

Matrix3D<GLfloat> DraggableQuaternion::getRotationMatrix() const noexcept {
	return quaternion.getRotationMatrix();
}

QuaternionType& DraggableQuaternion::getQuaternion() noexcept {
	return quaternion;
}

Point<float> DraggableQuaternion::mousePosToProportion(const Point<float> mousePos) const noexcept {
	const int scale = jmin(area.getWidth(), area.getHeight()) / 2;

	// You must call setViewport() to give this object a valid window size before
	// calling any of the mouse input methods!
	jassert(scale > 0);

	return Point<float>((mousePos.x - area.getCentreX()) / scale,
			(area.getCentreY() - mousePos.y) / scale);
}

QuaternionType DraggableQuaternion::rotationFromMove(const Point<float> orig, const Point<float> current) const noexcept {
	VectorType vOld = VectorType(orig.x, orig.y, 1.);
	VectorType vNew = VectorType(current.x, current.y, 1.);

	if (vOld.lengthIsBelowEpsilon() || vNew.lengthIsBelowEpsilon()) {
		return QuaternionType(VectorType::xAxis(), 0.f);
	}

	vOld = vOld.normalised();
	vNew = vNew.normalised();

	const VectorType axis = vOld ^ vNew;
	const float sina = std::sqrt(axis * axis);
	const float cosa = vOld * vNew;
	float angle = std::atan2(sina, cosa);

	// if outside of the viewport, make the angle a bit bigger
	// with this it's possible to rotate more than 90 degrees
	const float currentLength = current.getDistanceFromOrigin();
	if (currentLength > 1.f) {
		angle *= 1.f + .2f * (currentLength - 1.f);
	}

	return QuaternionType::fromAngle(angle, axis);
}
