#ifndef STRAHLENWERK_EXPORT_INTERFACE_H
#define STRAHLENWERK_EXPORT_INTERFACE_H
#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Sequence.h"
#include "Uniform.h"
#include "AmbientLight.h"
#include "Texture.h"
extern Shader postproc[26];
extern Input scale_inputs[2];
extern Input ao_inputs[1];
extern Input ao_blur_h_inputs[2];
extern Input ao_blur_v_inputs[2];
extern Input remove_fireflies_inputs[1];
extern Input ao_blend_inputs[2];
extern Input bloom_down_1st_inputs[1];
extern Input bloom_down_2nd_inputs[1];
extern Input bloom_down_3rd_inputs[1];
extern Input bloom_down_4th_inputs[1];
extern Input bloom_down_5th_inputs[1];
extern Input bloom_down_6th_inputs[1];
extern Input bloom_down_7th_inputs[1];
extern Input bloom_up_7th_inputs[1];
extern Input bloom_up_6th_inputs[2];
extern Input bloom_up_5th_inputs[2];
extern Input bloom_up_4th_inputs[2];
extern Input bloom_up_3rd_inputs[2];
extern Input bloom_up_2nd_inputs[2];
extern Input bloom_up_1st_inputs[2];
extern Input bloom_up_final_inputs[2];
extern Input godrays_inputs[1];
extern Input final_inputs[1];
extern Input colorseparation_inputs[1];
extern Input scanlines_inputs[1];
extern Input output_inputs[1];
extern Shader scenes[2];
extern AmbientLight environments[2];
extern Texture textures[4];
extern Framebuffer fbos[26];
extern Scene scenes_data[3];
extern float keyframe_data[1048];
extern Sequence sequence_data[244];
extern int sequence_index[135];
extern int keyframe_index[135];
extern Uniform uniforms[134];
extern int keyframe_time[666];
extern int keyframe_time_index[135];
const int time_uniform_location = 68;
#ifndef BPM
#define BPM 125
#endif
const unsigned int textureOffsetAmbient = 20;
const unsigned int textureOffsetPostproc = 25;
#endif
