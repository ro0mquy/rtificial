#ifndef STRAHLENWERK_EXPORT_INTERFACE_H
#define STRAHLENWERK_EXPORT_INTERFACE_H
#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Sequence.h"
#include "Uniform.h"
extern Shader postproc[15];
extern Input upscale_inputs[1];
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
extern Shader scenes[6];
extern Framebuffer fbos[15];
extern Scene scenes_data[6];
extern float keyframe_data[129];
extern Sequence sequence_data[7];
extern int sequence_index[78];
extern int keyframe_index[78];
extern Uniform uniforms[77];
extern int keyframe_time[16];
extern int keyframe_time_index[78];
#endif
