#ifndef DRAGGABLEQUATERNION_H
#define DRAGGABLEQUATERNION_H

#include <juce>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class DraggableQuaternion
{
	public:
		DraggableQuaternion();
		DraggableQuaternion(const glm::quat& quaternionToUse);

		void mouseDown(Point<float> mousePos);
		void mouseDrag(Point<float> mousePos);

		glm::quat& getQuaternion();

	private:
		glm::quat quaternion;
		Point<float> lastMousePos;

		static glm::quat rotationFromMove(const Point<float> last, const Point<float> current);
};

#endif // DRAGGABLEQUATERNION_H
