// This file contains the Identifiers used to describe the ValueTree
// of the Timeline.

#ifndef TIMELINETREE_H
#define TIMELINETREE_H

#include "../../JuceLibraryCode/JuceHeader.h"

namespace treeId {
	// the main ValueTree object
	Identifier timelineTree("timelineTree");
		// the different scenes, that are shown one after another
		Identifier scenesArray("scenesArray");
			// a single scene object, contained in the scenesArray
			Identifier scene("scene");
				// unique Id
				Identifier sceneId("sceneId");
				// the starting time of a scene
				Identifier sceneStart("sceneStart");
				// how long the scene goes
				Identifier sceneDuration("sceneDuration");
				// the filename of the glsl shader to be used in this scene
				// (maybe another unique identifier?)
				Identifier sceneShaderSource("sceneShaderSource");
		// an array with all uniforms from all shaders
		Identifier uniformsArray("uniformsArray");
			// an individual uniform
			Identifier uniform("uniform");
				// the name of the uniform
				Identifier uniformName("uniformName");
				// the data type of the uniform (float, vec3, color, ...)
				Identifier uniformType("uniformType");
				// standard value that is used when no sequence is present
				Identifier uniformStandardValue("uniformStandardValue");
				// array of sequences for this uniform
				Identifier sequencesArray("sequencesArray");
					// a single sequence for a uniform
					Identifier sequence("sequence");
						// the sceneId the sequence belongs to
						Identifier sequenceSceneId("sequenceSceneId");
						// the sequence starting time, relative to the scene starting time
						Identifier sequenceStart("sequenceStart");
						// the sequence duration
						Identifier sequenceDuration("sequenceDuration");
						// the method used to interpolate between keyframes
						Identifier sequenceInterpolation("sequenceInterpolation");
						// array of keyframes, there must be at least a start and end frame
						Identifier keyframesArray("keyframesArray");
							// individual keyframe
							Identifier keyframe("keyframe");
								// time position, relative to keyframe start
								Identifier keyframePosition("keyframePosition");
								// the value of this keyframe
								Identifier keyframeValue("keyframeValue");
}

#endif // TIMELINETREE_H
