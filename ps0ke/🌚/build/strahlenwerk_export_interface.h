#ifndef STRAHLENWERK_EXPORT_INTERFACE_H
#define STRAHLENWERK_EXPORT_INTERFACE_H
#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Sequence.h"
#include "Uniform.h"
#include "AmbientLight.h"
#include "Texture.h"
extern Shader postproc[2];
extern Input final_inputs[1];
extern Input output_inputs[1];
extern Shader scenes[1];
extern AmbientLight environments[1];
extern Texture textures[0];
extern Framebuffer fbos[2];
extern Scene scenes_data[1];
extern float keyframe_data[233];
extern Sequence sequence_data[7];
extern int sequence_index[7];
extern int keyframe_index[7];
extern Uniform uniforms[6];
extern int keyframe_time[75];
extern int keyframe_ease_toward[75];
extern int keyframe_ease_away[75];
extern int keyframe_time_index[7];
const int time_uniform_location = 64;
#ifndef BPM
#define BPM 130
#endif
const unsigned int textureOffsetAmbient = 20;
const unsigned int textureOffsetPostproc = 25;
#endif
