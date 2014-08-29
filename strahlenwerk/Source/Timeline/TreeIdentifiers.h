// This file contains the Identifiers used to describe the ValueTree
// of the Timeline.

#ifndef TIMELINETREE_H
#define TIMELINETREE_H

#include <juce>

namespace treeId {
	// the main ValueTree object
	const Identifier timelineTree("timelineTree");
		// the different scenes, that are shown one after another
		const Identifier scenesArray("scenesArray");
			// a single scene object, contained in the scenesArray
			const Identifier scene("scene");
				// unique Id
				const Identifier sceneId("sceneId");
				// the starting time of a scene
				const Identifier sceneStart("sceneStart");
				// how long the scene goes
				const Identifier sceneDuration("sceneDuration");
				// the filename of the glsl shader to be used in this scene
				// (maybe another unique identifier?)
				const Identifier sceneShaderSource("sceneShaderSource");
		// an array with all uniforms from all shaders
		const Identifier uniformsArray("uniformsArray");
			// an individual uniform
			const Identifier uniform("uniform");
				// the name of the uniform
				const Identifier uniformName("uniformName");
				// the data type of the uniform (float, vec3, color, ...)
				const Identifier uniformType("uniformType");
				// standard value that is used when no sequence is present
				const Identifier uniformStandardValue("uniformStandardValue");
					// value data, see keyframeValue for content
					// ...
				// array of sequences for this uniform
				const Identifier sequencesArray("sequencesArray");
					// a single sequence for a uniform
					const Identifier sequence("sequence");
						// the sceneId the sequence belongs to
						const Identifier sequenceSceneId("sequenceSceneId");
						// the sequence starting time, relative to the scene starting time
						const Identifier sequenceStart("sequenceStart");
						// the sequence duration
						const Identifier sequenceDuration("sequenceDuration");
						// the method used to interpolate between keyframes
						const Identifier sequenceInterpolation("sequenceInterpolation");
						// array of keyframes, there must be at least a start and end frame
						const Identifier keyframesArray("keyframesArray");
							// individual keyframe
							const Identifier keyframe("keyframe");
								// time position, relative to keyframe start
								const Identifier keyframePosition("keyframePosition");
								// the value of this keyframe
								const Identifier keyframeValue("keyframeValue");
									// the value holds a set of properties, depending on it's type
									// the data type, must be the same as the one of the uniform
									// the value types are:
									// bool
									const Identifier valueBoolState("valueBoolState");
									// float
									const Identifier valueFloatX("valueFloatX");
									// vec2
									const Identifier valueVec2X("valueVec2X");
									const Identifier valueVec2Y("valueVec2Y");
									// vec3
									const Identifier valueVec3X("valueVec3X");
									const Identifier valueVec3Y("valueVec3Y");
									const Identifier valueVec3Z("valueVec3Z");
									// color
									const Identifier valueColorR("valueColorR");
									const Identifier valueColorG("valueColorG");
									const Identifier valueColorB("valueColorB");

	// layout for tree holding the current state of every uniform
	const Identifier uniformStatesArray("uniformStatesArray");
		// every uniform as a child
		const Identifier uniformState("uniformState");
			// the name of the uniform, same as in timelineTree
			//const Identifier uniformName("uniformName");
			// the current value of the uniform
			const Identifier uniformValue("uniformValue");
				// value data, see keyframeValue for content
				// ...
			// is a keyframe at the current time
			const Identifier uniformOnKeyframe("uniformOnKeyframe");

}

#endif // TIMELINETREE_H
