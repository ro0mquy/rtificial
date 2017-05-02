#ifndef STRAHLENWERK_EXPORT_INTERFACE_H
#define STRAHLENWERK_EXPORT_INTERFACE_H
#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Sequence.h"
#include "Uniform.h"
#include "AmbientLight.h"
#include "Texture.h"
extern Shader postproc[25];
extern Input scale_inputs[2];
extern Input remove_fireflies_inputs[1];
extern Input coc_map_inputs[2];
extern Input dof_diagonal_inputs[2];
extern Input dof_horizontal_inputs[2];
extern Input dof_diagonal2_inputs[2];
extern Input dof_vertical_inputs[2];
extern Input dof_final_inputs[3];
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
extern Input final_inputs[1];
extern Input output_inputs[1];
extern Shader scenes[4];
extern AmbientLight environments[4];
extern Texture textures[1];
extern Framebuffer fbos[25];
extern Scene scenes_data[16];
extern float keyframe_data[1283];
extern Sequence sequence_data[259];
extern int sequence_index[210];
extern int keyframe_index[210];
extern Uniform uniforms[209];
extern int keyframe_time[644];
extern int keyframe_ease_toward[644];
extern int keyframe_ease_away[644];
extern int keyframe_time_index[210];
const int time_uniform_location = 64;
#ifndef BPM
#define BPM 124
#endif
const unsigned int textureOffsetAmbient = 20;
const unsigned int textureOffsetPostproc = 25;
#endif
