#ifndef DRAGGABLEQUATERNION_H
#define DRAGGABLEQUATERNION_H

#include <juce>

class DraggableQuaternion
{
	public:
		typedef Vector3D<GLfloat> VectorType;
		typedef Quaternion<GLfloat> QuaternionType;

		DraggableQuaternion(float objectRadius = 0.5f) noexcept;
		DraggableQuaternion(const Quaternion<GLfloat>& quaternionToUse, float objectRadius = 0.5f) noexcept;

		void reset(const VectorType& axis) noexcept;
		void setViewport(const Rectangle<int>& newArea) noexcept;
		void setRadius(float newRadius) noexcept;

		void mouseDown(Point<float> mousePos) noexcept;
		void mouseDrag(Point<float> mousePos) noexcept;

		Matrix3D<GLfloat> getRotationMatrix() const noexcept;
		QuaternionType& getQuaternion() noexcept;

	private:
		Rectangle<int> area;
		float radius;
		QuaternionType quaternion;
		Point<float> origMouse;
		QuaternionType origQuat;

		Point<float> mousePosToProportion(const Point<float> mousePos) const noexcept;
		QuaternionType rotationFromMove(const Point<float> orig, const Point<float> current) const noexcept;
};

#endif // DRAGGABLEQUATERNION_H
