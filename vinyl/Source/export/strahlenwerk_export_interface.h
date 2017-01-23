#ifndef STRAHLENWERK_EXPORT_INTERFACE_H
#define STRAHLENWERK_EXPORT_INTERFACE_H
#include "../Shader.h"
#include "../Framebuffer.h"
#include "../Scene.h"
#include "../Sequence.h"
#include "../Uniform.h"
#include "../AmbientLight.h"
#include "../Texture.h"
extern Shader postproc[25];
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
extern Input edge_sobel_inputs[2];
extern Input bloom_up_final_inputs[2];
extern Input edge_nonmax_inputs[1];
extern Input final_inputs[2];
extern Input output_inputs[1];
extern Shader scenes[11];
extern AmbientLight environments[11];
extern Texture textures[0];
extern Framebuffer fbos[25];
extern Scene scenes_data[10];
extern float keyframe_data[682];
extern Sequence sequence_data[108];
extern int sequence_index[164];
extern int keyframe_index[164];
extern Uniform uniforms[163];
extern int keyframe_time[224];
extern int keyframe_ease_toward[224];
extern int keyframe_ease_away[224];
extern int keyframe_time_index[164];
const int time_uniform_location = 66;
#ifndef BPM
#define BPM 125
#endif
const unsigned int textureOffsetAmbient = 20;
const unsigned int textureOffsetPostproc = 25;
#endif
