#ifndef STRAHLENWERK_EXPORT
#define STRAHLENWERK_EXPORT
#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Sequence.h"
#include "Uniform.h"
#include "AmbientLight.h"
#include "Texture.h"
#include "shaders/output.h"
#include "shaders/test.h"
#include "shaders/texture_7.h"
Input output_inputs[1] = {
	{0, 0},
};
Shader postproc[1] = {
	Shader(output_source, 1, output_inputs),
};
Shader scenes[1] = {
	Shader(test_source, 0, nullptr),
};
AmbientLight environments[1] = {
	AmbientLight(nullptr),
};
Texture textures[1] = {
	Texture(7, texture_7_source, 1, 1),
};
Output input_outputs[2] = {
	{3, 0, 0},
	{1, 1, 0},
};
Framebuffer fbos[1] = {
	Framebuffer(0, 2, input_outputs),
};
Scene scenes_data[1] = {
	{0, 63500, 0},
};
float keyframe_data[233] = {
	1.000000, 1.000000, 1.000000,
	0.000000, 0.139022, 1.000000,
	0.481952, 0.003697, 0.003697,
	1.000000, 0.766744, 0.436813,
	0.306635, 0.246800, 0.505432,
	0.230998, 0.084642, 0.415148,
	1.000000, 1.000000, 1.000000,
	1.000000, 0.039947, 0.012664,
	0.883180, 0.481952, 0.013473,
	0.694081, 0.812241, 0.632043,
	0.063815, 1.000000, 0.781751,
	0.715465, 0.541798, 0.004560,
	0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000,
	1.000000, 1.000000, 1.000000,
	1.000000, 0.991393, 0.368591,
	0.035614, 0.035614, 0.007155,
	1.000000, 1.000000, 1.000000,
	0.026549, 0.007751, 0.000638,
	0.027755, 0.130352, 0.000992,
	1.000000, 1.000000, 1.000000,
	0.000175, 0.023104, 0.049433,
	0.037029, 0.000000, 0.000000,
	1.000000, 1.000000, 1.000000,
	0.000262, 0.026549, 0.001963,
	0.002263, 0.041452, 0.000107,
	1.000000, 1.000000, 1.000000,
	0.006041, 0.000493, 0.052842,
	0.176774, 0.442323, 0.459080,
	1.000000, 1.000000, 1.000000,
	0.363604, 0.133209, 0.061907,
	0.267358, 0.000000, 0.215764,
	1.000000, 1.000000, 1.000000,
	0.108711, 0.010398, 0.201096,
	0.075926, 0.049433, 0.049433,
	1.000000, 1.000000, 1.000000,
	0.108711, 0.000262, 0.000262,
	0.071761, 0.000000, 0.409826,
	1.000000, 1.000000, 1.000000,
	0.000262, 0.186989, 0.006585,
	0.065754, 0.000493, 0.000493,
	1.000000, 1.000000, 1.000000,
	0.208360, 0.288816, 0.000005,
	0.000000, 0.078057, 0.409826,
	1.000000, 1.000000, 1.000000,
	0.023104, 0.275833, 0.431340,
	0.024223, 0.024223, 0.024223,
	1.000000, 1.000000, 1.000000,
	0.000000, 0.009021, 0.147998,
	0.573159, 0.000367, 0.000367,
	1.000000, 1.000000, 1.000000,
	0.061907, 0.007155, 0.121986,
	0.722672, 0.566810, 0.001687,
	1.000000, 1.000000, 1.000000,
	0.103634, 0.579547, 0.001687,
	0.240099, 0.083001, 0.089360,
	1.000000, 1.000000, 1.000000,
	0.245376, 0.089370, 0.095684,
	0.034230, 0.147998, 0.073828,
	1.000000, 1.000000, 1.000000,
	1.000000, 0.000000, 0.982826,
	0.106156, 0.011881, 0.011881,
	1.000000, 1.000000, 1.000000,
	0.176774, 0.005028, 0.005028,
	0.250840, 0.008373, 0.008373,
	0.541798, 0.000000, 0.000000,
	0.009696, 0.032876, 0.011126,
	0.851252, 0.851252, 0.851252,
	1.000000, 1.000000, 1.000000,
	0.046149, 0.061907, 0.186989,
	0.737205, 0.505432, 0.005522,
	1.000000, 1.000000, 1.000000,
	1.000000, 1.000000, 1.000000,
	0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000,
	0.180000, 0.180000, 0.180000,
	0.102000,
	0.011000,
	0.001000,
	0.000000,
	1.000000,
};
Sequence sequence_data[7] = {
	{0, 27125, 1, 4},
	{27125, 40500, 1, 3},
	{40500, 63500, 1, 5},
	{0, 27125, 1, 50},
	{27125, 40500, 1, 3},
	{40500, 63500, 1, 7},
	{0, 63500, 2, 3},
};
int sequence_index[7] = {
	0,
	0,
	3,
	6,
	6,
	7,
	7,
};
int keyframe_index[7] = {
	0,
	3,
	42,
	225,
	228,
	232,
	233,
};
Uniform uniforms[6] = {
	{2, 67 },
	{4, 65 },
	{4, 64 },
	{4, -1 },
	{0, 66 },
	{0, -1 },
};
int keyframe_time[75] = {
	0,
	10000,
	21000,
	27125,
	0,
	9625,
	13375,
	0,
	9000,
	13250,
	17250,
	23000,
	0,
	1125,
	1375,
	1812,
	2750,
	3000,
	3437,
	4375,
	4500,
	4937,
	6000,
	6125,
	6500,
	7750,
	7875,
	8312,
	9250,
	9375,
	10000,
	11000,
	11125,
	11500,
	12750,
	12875,
	13375,
	14625,
	14750,
	15250,
	16125,
	16375,
	16750,
	17625,
	17750,
	18281,
	19500,
	19625,
	20000,
	21375,
	21500,
	21937,
	23000,
	23125,
	23625,
	24500,
	24625,
	25093,
	26250,
	26375,
	26656,
	27125,
	0,
	9625,
	13375,
	0,
	9750,
	14750,
	17250,
	21250,
	21750,
	23000,
	0,
	11500,
	48000,
};
bool keyframe_ease_toward[75] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
};
bool keyframe_ease_away[75] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
};
int keyframe_time_index[7] = {
	0,
	0,
	12,
	72,
	72,
	75,
	75,
};
#endif
