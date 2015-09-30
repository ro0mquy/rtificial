#ifndef STRAHLENWERK_EXPORT
#define STRAHLENWERK_EXPORT
#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Sequence.h"
#include "Uniform.h"
#include "AmbientLight.h"
#include "shaders/scale.h"
#include "shaders/ao.h"
#include "shaders/ao_blur_h.h"
#include "shaders/ao_blur_v.h"
#include "shaders/remove_fireflies.h"
#include "shaders/ao_blend.h"
#include "shaders/bloom_down_1st.h"
#include "shaders/bloom_down_2nd.h"
#include "shaders/bloom_down_3rd.h"
#include "shaders/bloom_down_4th.h"
#include "shaders/bloom_down_5th.h"
#include "shaders/bloom_down_6th.h"
#include "shaders/bloom_down_7th.h"
#include "shaders/bloom_up_7th.h"
#include "shaders/bloom_up_6th.h"
#include "shaders/bloom_up_5th.h"
#include "shaders/bloom_up_4th.h"
#include "shaders/bloom_up_3rd.h"
#include "shaders/bloom_up_2nd.h"
#include "shaders/bloom_up_1st.h"
#include "shaders/bloom_up_final.h"
#include "shaders/final.h"
#include "shaders/output.h"
#include "shaders/aman.h"
Input scale_inputs[2] = {
	{0, 0},
	{1, 0},
};
Input ao_inputs[1] = {
	{3, 1},
};
Input ao_blur_h_inputs[2] = {
	{4, 0},
	{3, 0},
};
Input ao_blur_v_inputs[2] = {
	{5, 0},
	{3, 0},
};
Input remove_fireflies_inputs[1] = {
	{2, 0},
};
Input ao_blend_inputs[2] = {
	{6, 0},
	{7, 0},
};
Input bloom_down_1st_inputs[1] = {
	{8, 0},
};
Input bloom_down_2nd_inputs[1] = {
	{9, 0},
};
Input bloom_down_3rd_inputs[1] = {
	{10, 0},
};
Input bloom_down_4th_inputs[1] = {
	{11, 0},
};
Input bloom_down_5th_inputs[1] = {
	{12, 0},
};
Input bloom_down_6th_inputs[1] = {
	{13, 0},
};
Input bloom_down_7th_inputs[1] = {
	{14, 0},
};
Input bloom_up_7th_inputs[1] = {
	{15, 0},
};
Input bloom_up_6th_inputs[2] = {
	{14, 0},
	{16, 0},
};
Input bloom_up_5th_inputs[2] = {
	{13, 0},
	{17, 0},
};
Input bloom_up_4th_inputs[2] = {
	{12, 0},
	{18, 0},
};
Input bloom_up_3rd_inputs[2] = {
	{11, 0},
	{19, 0},
};
Input bloom_up_2nd_inputs[2] = {
	{10, 0},
	{20, 0},
};
Input bloom_up_1st_inputs[2] = {
	{9, 0},
	{21, 0},
};
Input bloom_up_final_inputs[2] = {
	{8, 0},
	{22, 0},
};
Input final_inputs[1] = {
	{23, 0},
};
Input output_inputs[1] = {
	{24, 0},
};
Shader postproc[23] = {
	Shader(scale_source, 2, scale_inputs),
	Shader(ao_source, 1, ao_inputs),
	Shader(ao_blur_h_source, 2, ao_blur_h_inputs),
	Shader(ao_blur_v_source, 2, ao_blur_v_inputs),
	Shader(remove_fireflies_source, 1, remove_fireflies_inputs),
	Shader(ao_blend_source, 2, ao_blend_inputs),
	Shader(bloom_down_1st_source, 1, bloom_down_1st_inputs),
	Shader(bloom_down_2nd_source, 1, bloom_down_2nd_inputs),
	Shader(bloom_down_3rd_source, 1, bloom_down_3rd_inputs),
	Shader(bloom_down_4th_source, 1, bloom_down_4th_inputs),
	Shader(bloom_down_5th_source, 1, bloom_down_5th_inputs),
	Shader(bloom_down_6th_source, 1, bloom_down_6th_inputs),
	Shader(bloom_down_7th_source, 1, bloom_down_7th_inputs),
	Shader(bloom_up_7th_source, 1, bloom_up_7th_inputs),
	Shader(bloom_up_6th_source, 2, bloom_up_6th_inputs),
	Shader(bloom_up_5th_source, 2, bloom_up_5th_inputs),
	Shader(bloom_up_4th_source, 2, bloom_up_4th_inputs),
	Shader(bloom_up_3rd_source, 2, bloom_up_3rd_inputs),
	Shader(bloom_up_2nd_source, 2, bloom_up_2nd_inputs),
	Shader(bloom_up_1st_source, 2, bloom_up_1st_inputs),
	Shader(bloom_up_final_source, 2, bloom_up_final_inputs),
	Shader(final_source, 1, final_inputs),
	Shader(output_source, 1, output_inputs),
};
Shader scenes[1] = {
	Shader(aman_source, 0, nullptr),
};
AmbientLight environments[1] = {
	AmbientLight(nullptr),
};
Output input_outputs[2] = {
	{3, 0, 0},
	{1, 1, 0},
};
Output scale_outputs[2] = {
	{3, 2, 0},
	{1, 3, 1},
};
Output ao_outputs[1] = {
	{1, 4, 0},
};
Output ao_blur_h_outputs[1] = {
	{1, 5, 0},
};
Output ao_blur_v_outputs[1] = {
	{1, 6, 0},
};
Output remove_fireflies_outputs[1] = {
	{3, 7, 0},
};
Output ao_blend_outputs[1] = {
	{3, 8, 0},
};
Output bloom_down_1st_outputs[1] = {
	{3, 9, 0},
};
Output bloom_down_2nd_outputs[1] = {
	{3, 10, 0},
};
Output bloom_down_3rd_outputs[1] = {
	{3, 11, 0},
};
Output bloom_down_4th_outputs[1] = {
	{3, 12, 0},
};
Output bloom_down_5th_outputs[1] = {
	{3, 13, 0},
};
Output bloom_down_6th_outputs[1] = {
	{3, 14, 0},
};
Output bloom_down_7th_outputs[1] = {
	{3, 15, 0},
};
Output bloom_up_7th_outputs[1] = {
	{3, 16, 0},
};
Output bloom_up_6th_outputs[1] = {
	{3, 17, 0},
};
Output bloom_up_5th_outputs[1] = {
	{3, 18, 0},
};
Output bloom_up_4th_outputs[1] = {
	{3, 19, 0},
};
Output bloom_up_3rd_outputs[1] = {
	{3, 20, 0},
};
Output bloom_up_2nd_outputs[1] = {
	{3, 21, 0},
};
Output bloom_up_1st_outputs[1] = {
	{3, 22, 0},
};
Output bloom_up_final_outputs[1] = {
	{3, 23, 0},
};
Output final_outputs[1] = {
	{3, 24, 0},
};
Framebuffer fbos[23] = {
	Framebuffer(0, 2, input_outputs),
	Framebuffer(0, 2, scale_outputs),
	Framebuffer(1, 1, ao_outputs),
	Framebuffer(1, 1, ao_blur_h_outputs),
	Framebuffer(1, 1, ao_blur_v_outputs),
	Framebuffer(0, 1, remove_fireflies_outputs),
	Framebuffer(0, 1, ao_blend_outputs),
	Framebuffer(1, 1, bloom_down_1st_outputs),
	Framebuffer(2, 1, bloom_down_2nd_outputs),
	Framebuffer(3, 1, bloom_down_3rd_outputs),
	Framebuffer(4, 1, bloom_down_4th_outputs),
	Framebuffer(5, 1, bloom_down_5th_outputs),
	Framebuffer(6, 1, bloom_down_6th_outputs),
	Framebuffer(7, 1, bloom_down_7th_outputs),
	Framebuffer(6, 1, bloom_up_7th_outputs),
	Framebuffer(5, 1, bloom_up_6th_outputs),
	Framebuffer(4, 1, bloom_up_5th_outputs),
	Framebuffer(3, 1, bloom_up_4th_outputs),
	Framebuffer(2, 1, bloom_up_3rd_outputs),
	Framebuffer(1, 1, bloom_up_2nd_outputs),
	Framebuffer(0, 1, bloom_up_1st_outputs),
	Framebuffer(0, 1, bloom_up_final_outputs),
	Framebuffer(0, 1, final_outputs),
};
Scene scenes_data[1] = {
	{0, 242000, 0},
};
float keyframe_data[119] = {
	10.000000,
	0.000000,
	1.000000,
	0.100000,
	0.000000,
	17.000000,
	16.000000,
	0.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	2.000000,
	4.000000,
	-5.000000,
	0.022000,
	0.000000, 0.000000, 0.000000,
	-1.404096, 6.280515, 0.750497,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.024000,
	3.000000,
	0.000000,
	0.000000,
	0.000000,
	0.000000, 1.000000, 0.000000,
	200.000000,
	1.000000,
	0.132000,
	0.941000,
	0.100000,
	1.500000,
	6.000000,
	2.000000,
	2.327000,
	1.209000,
	0.500000, 0.500000, 0.500000,
	0.500000, 0.500000, 0.500000,
	0.500000, 0.500000, 0.500000,
	1.000000,
	1.000000,
	1.000000,
	1.540000,
	0.005000,
	1.000000,
	0.000000,
	0.000000,
	0.000000,
	1.000000,
	0.800000,
	6.000000,
	2.000000,
	0.000000,
	0.500000,
};
Sequence sequence_data[31] = {
	{0, 64000, 0, 1},
	{0, 8000, 0, 1},
	{8000, 24000, 1, 1},
	{83500, 85500, 0, 3},
	{62000, 64000, 0, 2},
	{83500, 85500, 0, 3},
	{32000, 33000, 1, 2},
	{34000, 35000, 1, 2},
	{36000, 37000, 1, 2},
	{38000, 39000, 1, 2},
	{40000, 41000, 1, 2},
	{42000, 43000, 1, 2},
	{44000, 45000, 1, 2},
	{46000, 47000, 1, 2},
	{48000, 49000, 1, 2},
	{49000, 50000, 1, 2},
	{50000, 51000, 1, 2},
	{51000, 52000, 1, 2},
	{52000, 53000, 1, 2},
	{53000, 54000, 1, 2},
	{54000, 55000, 1, 2},
	{55000, 56000, 1, 2},
	{56000, 57000, 1, 2},
	{57000, 58000, 1, 2},
	{58000, 59000, 1, 2},
	{59000, 60000, 1, 2},
	{60000, 61000, 1, 2},
	{61000, 62000, 1, 2},
	{62875, 63875, 1, 1},
	{62875, 63875, 0, 1},
	{0, 64000, 0, 1},
};
int sequence_index[47] = {
	0,
	0,
	1,
	1,
	3,
	5,
	6,
	29,
	30,
	30,
	30,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
	31,
};
int keyframe_index[47] = {
	0,
	1,
	3,
	4,
	7,
	13,
	17,
	63,
	65,
	66,
	67,
	73,
	77,
	78,
	79,
	80,
	81,
	82,
	85,
	86,
	87,
	88,
	89,
	90,
	91,
	92,
	93,
	94,
	95,
	98,
	101,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
};
Uniform uniforms[46] = {
	{0, 64 },
	{6, 65 },
	{0, 108 },
	{0, 99 },
	{6, 109 },
	{6, 110 },
	{0, 100 },
	{0, 101 },
	{0, 102 },
	{0, 71 },
	{2, 69 },
	{5, 70 },
	{0, 72 },
	{0, 67 },
	{0, 66 },
	{0, 104 },
	{0, 106 },
	{2, 105 },
	{0, 107 },
	{0, 103 },
	{0, 76 },
	{0, 78 },
	{0, 79 },
	{0, 75 },
	{0, 74 },
	{0, 73 },
	{0, 81 },
	{0, 80 },
	{4, 95 },
	{4, 96 },
	{4, 97 },
	{6, 77 },
	{6, 90 },
	{6, 98 },
	{0, 88 },
	{0, 87 },
	{0, 89 },
	{0, 82 },
	{0, 83 },
	{0, 84 },
	{6, 91 },
	{0, 94 },
	{0, 92 },
	{0, 93 },
	{0, 86 },
	{0, 85 },
};
int keyframe_time[58] = {
	0,
	0,
	0,
	0,
	1000,
	2000,
	0,
	1000,
	0,
	1000,
	2000,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	250,
	500,
	500,
	0,
	0,
};
int keyframe_time_index[47] = {
	0,
	0,
	1,
	1,
	3,
	8,
	11,
	56,
	57,
	57,
	57,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
	58,
};
#endif
