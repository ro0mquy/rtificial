#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Keyframe.h"
#include "Sequence.h"
#include "shaders/bokeh_pass1.h"
#include "shaders/bloom_small_vert.h"
#include "shaders/bloom_medium_vert.h"
#include "shaders/bloom_big_vert.h"
#include "shaders/bokeh_pass2.h"
#include "shaders/bloom_small_horiz.h"
#include "shaders/bloom_medium_horiz.h"
#include "shaders/bloom_big_horiz.h"
#include "shaders/blend.h"
#include "shaders/tonemap.h"
#include "shaders/fxaa.h"
#include "shaders/final.h"
#include "shaders/conicbobbel.h"
#include "shaders/raymarching.h"
#include "shaders/schalter.h"
#include "shaders/syntakischerspalt.h"
#include "shaders/zapfen.h"
Input bokeh_pass1_inputs[2] = {
	{0, 0},
	{1, 0},
};
Input bloom_small_vert_inputs[1] = {
	{0, 2},
};
Input bloom_medium_vert_inputs[1] = {
	{0, 3},
};
Input bloom_big_vert_inputs[1] = {
	{0, 4},
};
Input bokeh_pass2_inputs[2] = {
	{2, 0},
	{3, 0},
};
Input bloom_small_horiz_inputs[1] = {
	{4, 0},
};
Input bloom_medium_horiz_inputs[1] = {
	{5, 0},
};
Input bloom_big_horiz_inputs[1] = {
	{6, 0},
};
Input blend_inputs[5] = {
	{10, 0},
	{9, 0},
	{8, 0},
	{7, 0},
	{0, 0},
};
Input tonemap_inputs[2] = {
	{12, 11},
	{11, 0},
};
Input fxaa_inputs[1] = {
	{13, 0},
};
Input final_inputs[1] = {
	{14, 0},
};
Shader postproc[12] = {
	Shader(bokeh_pass1_source, 2, bokeh_pass1_inputs),
	Shader(bloom_small_vert_source, 1, bloom_small_vert_inputs),
	Shader(bloom_medium_vert_source, 1, bloom_medium_vert_inputs),
	Shader(bloom_big_vert_source, 1, bloom_big_vert_inputs),
	Shader(bokeh_pass2_source, 2, bokeh_pass2_inputs),
	Shader(bloom_small_horiz_source, 1, bloom_small_horiz_inputs),
	Shader(bloom_medium_horiz_source, 1, bloom_medium_horiz_inputs),
	Shader(bloom_big_horiz_source, 1, bloom_big_horiz_inputs),
	Shader(blend_source, 5, blend_inputs),
	Shader(tonemap_source, 2, tonemap_inputs),
	Shader(fxaa_source, 1, fxaa_inputs),
	Shader(final_source, 1, final_inputs),
};
Shader scenes[5] = {
	Shader(conicbobbel_source, 0, nullptr),
	Shader(raymarching_source, 0, nullptr),
	Shader(schalter_source, 0, nullptr),
	Shader(syntakischerspalt_source, 0, nullptr),
	Shader(zapfen_source, 0, nullptr),
};
Output input_outputs[2] = {
	{3, 0, 4},
	{1, 1, 0},
};
Output bokeh_pass1_outputs[2] = {
	{4, 2, 0},
	{4, 3, 0},
};
Output bloom_small_vert_outputs[1] = {
	{3, 4, 0},
};
Output bloom_medium_vert_outputs[1] = {
	{3, 5, 0},
};
Output bloom_big_vert_outputs[1] = {
	{3, 6, 0},
};
Output bokeh_pass2_outputs[1] = {
	{4, 7, 0},
};
Output bloom_small_horiz_outputs[1] = {
	{3, 8, 0},
};
Output bloom_medium_horiz_outputs[1] = {
	{3, 9, 0},
};
Output bloom_big_horiz_outputs[1] = {
	{3, 10, 0},
};
Output blend_outputs[2] = {
	{3, 11, 0},
	{1, 12, 11},
};
Output tonemap_outputs[1] = {
	{3, 13, 0},
};
Output fxaa_outputs[1] = {
	{3, 14, 0},
};
Framebuffer fbos[12] = {
	Framebuffer(0, 2, input_outputs),
	Framebuffer(0, 2, bokeh_pass1_outputs),
	Framebuffer(2, 1, bloom_small_vert_outputs),
	Framebuffer(3, 1, bloom_medium_vert_outputs),
	Framebuffer(4, 1, bloom_big_vert_outputs),
	Framebuffer(0, 1, bokeh_pass2_outputs),
	Framebuffer(2, 1, bloom_small_horiz_outputs),
	Framebuffer(3, 1, bloom_medium_horiz_outputs),
	Framebuffer(4, 1, bloom_big_horiz_outputs),
	Framebuffer(0, 2, blend_outputs),
	Framebuffer(0, 1, tonemap_outputs),
	Framebuffer(0, 1, fxaa_outputs),
};
Scene scenes_data[5] = {
	{0, 52, 2},
	{52, 92, 0},
	{92, 156, 4},
	{156, 232, 3},
	{232, 280, 1},
};
float keyframe_data[165] = {
	0.000000, 0.000000, 0.000000,
	0.004000,
	0.000000, 0.000000, 0.000000,
	-0.092100, 0.000000, 6.554260,
	12.343600, 8.471510, 3.440830,
	-3.469000, 4.078000, 12.865000,
	1.670300, 4.078000, 9.579030,
	5.721320, 4.078000, 5.401750,
	7.239230, 4.078000, -5.904910,
	2.589800, 7.789590, -8.642990,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	-0.124379, 0.598562, 0.094759, 0.785668,
	-0.117000, -0.195000, -0.023000, 0.974000,
	-0.114333, 0.353813, 0.043625, 0.927276,
	-0.061116, 0.832120, 0.093621, 0.543209,
	-0.087458, 0.903775, 0.354868, 0.222736,
	0.295822, 0.543238, 0.380202,
	1.500000,
	-1.000000,
	0.229901, 0.401021, 0.040421,
	5.000000,
	0.840000,
	1.000000,
	0.001000,
	0.005000,
	2.200000,
	0.019000,
	5.100000,
	1.000000, 1.000000, 1.000000,
	1.000000, 1.000000, 1.000000,
	0.433000,
	0.020000,
	0.000000,
	0.500000,
	0.000000, 0.000000, 0.000159,
	0.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	0.000000,
	-1.000000,
	-2.000000,
	-3.000000,
	5.000000,
	0.427000,
	0.313000,
	0.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.041424, 0.038151, 0.220577,
	0.000000, 0.000000, 0.000000,
	-0.249000, 0.039000, 0.377000,
	-0.713000, 0.128000, -0.416000,
	-0.504000, 0.576000, 1.000000,
	-0.192000, 0.328000, -0.456000,
	0.624000, 0.512000, 0.000000,
	0.216040, 0.023355, 0.007217,
	1.000000,
	0.440000,
	-0.480000,
	0.004141, 0.004141, 0.007517,
	1.000000,
	0.051000,
	0.774000,
	0.000000, 0.000000, 0.000000,
	0.006296, 0.025176, 0.005390,
	0.061000,
	0.149000,
	2.000000,
	3.000000,
	1.000000, 1.000000, 1.000000,
	0.000000, 20.000000, 0.000000,
	0.180000, 0.180000, 0.180000,
	30.000000, 50.000000, -100.000000,
	0.232000,
	0.603000,
	0.773000,
};
Sequence sequence_data[12] = {
	{2, 6, 1, 2},
	{236, 248, 2, 5},
	{2, 6, 1, 2},
	{236, 248, 2, 4},
	{14, 24, 1, 2},
	{2, 14, 1, 2},
	{24, 28, 1, 2},
	{2, 32, 0, 5},
	{14, 20, 1, 2},
	{24, 26, 1, 2},
	{30, 31, 1, 2},
	{235, 249, 2, 5},
};
int sequence_index[52] = {
	0,
	0,
	0,
	2,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	7,
	8,
	8,
	8,
	11,
	11,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
};
int keyframe_index[52] = {
	0,
	3,
	4,
	28,
	56,
	59,
	60,
	61,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	75,
	78,
	79,
	80,
	81,
	82,
	85,
	92,
	98,
	99,
	100,
	107,
	110,
	128,
	131,
	132,
	133,
	134,
	137,
	138,
	139,
	140,
	143,
	146,
	147,
	148,
	149,
	150,
	153,
	156,
	159,
	162,
	163,
	164,
	165,
};
