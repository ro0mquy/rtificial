#ifndef STRAHLENWERK_EXPORT_INTERFACE_H
#define STRAHLENWERK_EXPORT_INTERFACE_H
#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Sequence.h"
#include "Uniform.h"
#include "AmbientLight.h"
extern Shader postproc[18];
extern Input upscale_inputs[2];
extern Input bokeh_pass1_inputs[2];
extern Input bokeh_pass2_inputs[2];
extern Input bokeh_blend_inputs[2];
extern Input bloom_down_1st_inputs[1];
extern Input bloom_down_2nd_inputs[1];
extern Input bloom_down_3rd_inputs[1];
extern Input bloom_down_4th_inputs[1];
extern Input bloom_down_5th_inputs[1];
extern Input bloom_up_5th_inputs[1];
extern Input bloom_up_4th_inputs[2];
extern Input bloom_up_3rd_inputs[2];
extern Input bloom_up_2nd_inputs[2];
extern Input bloom_up_1st_inputs[2];
extern Input bloom_up_final_inputs[2];
extern Input tonemap_inputs[1];
extern Input final_inputs[1];
extern Input fxaa_inputs[1];
extern Shader scenes[3];
extern AmbientLight environments[3];
extern Framebuffer fbos[18];
extern Scene scenes_data[7];
extern float keyframe_data[712];
extern Sequence sequence_data[81];
extern int sequence_index[73];
extern int keyframe_index[73];
extern Uniform uniforms[72];
extern int keyframe_time[288];
extern int keyframe_time_index[73];
#endif
