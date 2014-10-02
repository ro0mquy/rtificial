#ifndef STRAHLENWERK_EXPORT_INTERFACE_H
#define STRAHLENWERK_EXPORT_INTERFACE_H
#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Keyframe.h"
#include "Sequence.h"
#include "Uniform.h"
extern Shader postproc[12];
extern Input bokeh_pass1_inputs[2];
extern Input bloom_small_vert_inputs[1];
extern Input bloom_medium_vert_inputs[1];
extern Input bloom_big_vert_inputs[1];
extern Input bokeh_pass2_inputs[2];
extern Input bloom_small_horiz_inputs[1];
extern Input bloom_medium_horiz_inputs[1];
extern Input bloom_big_horiz_inputs[1];
extern Input blend_inputs[5];
extern Input tonemap_inputs[2];
extern Input fxaa_inputs[1];
extern Input final_inputs[1];
extern Shader scenes[5];
extern Framebuffer fbos[12];
extern Scene scenes_data[5];
extern float keyframe_data[177];
extern Sequence sequence_data[14];
extern int sequence_index[55];
extern int keyframe_index[55];
extern Uniform uniforms[54];
extern float keyframe_time[42];
extern int keyframe_time_index[55];
#endif
