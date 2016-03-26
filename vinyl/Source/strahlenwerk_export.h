#ifndef STRAHLENWERK_EXPORT
#define STRAHLENWERK_EXPORT
#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Sequence.h"
#include "Uniform.h"
#include "AmbientLight.h"
#include "Texture.h"
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
#include "shaders/background_test.h"
#include "shaders/bridge.h"
#include "shaders/end.h"
#include "shaders/f16_beginning.h"
#include "shaders/f16_build.h"
#include "shaders/f16_takeoff.h"
#include "shaders/funk.h"
#include "shaders/landscape.h"
#include "shaders/parl.h"
#include "shaders/parl_schwurbel.h"
#include "shaders/part.h"
#include "shaders/rain.h"
#include "shaders/texture_7.h"
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
Shader scenes[12] = {
	Shader(background_test_source, 0, nullptr),
	Shader(bridge_source, 0, nullptr),
	Shader(end_source, 0, nullptr),
	Shader(f16_beginning_source, 0, nullptr),
	Shader(f16_build_source, 0, nullptr),
	Shader(f16_takeoff_source, 0, nullptr),
	Shader(funk_source, 0, nullptr),
	Shader(landscape_source, 0, nullptr),
	Shader(parl_source, 0, nullptr),
	Shader(parl_schwurbel_source, 0, nullptr),
	Shader(part_source, 0, nullptr),
	Shader(rain_source, 0, nullptr),
};
AmbientLight environments[12] = {
	AmbientLight(nullptr),
	AmbientLight(nullptr),
	AmbientLight(nullptr),
	AmbientLight(nullptr),
	AmbientLight(nullptr),
	AmbientLight(nullptr),
	AmbientLight(nullptr),
	AmbientLight(nullptr),
	AmbientLight(nullptr),
	AmbientLight(nullptr),
	AmbientLight(nullptr),
	AmbientLight(nullptr),
};
Texture textures[1] = {
	Texture(7, texture_7_source, 333, 226),
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
Scene scenes_data[11] = {
	{0, 58500, 3},
	{58500, 124000, 10},
	{124000, 156000, 11},
	{156000, 188000, 6},
	{188000, 236000, 5},
	{236000, 307500, 7},
	{307500, 344000, 1},
	{344000, 363500, 8},
	{363500, 395500, 9},
	{395500, 457500, 2},
	{512000, 611500, 0},
};
float keyframe_data[695] = {
	-0.112000, 0.089000, -1.000000,
	0.014000,
	29.145000,
	0.437000, 0.336000, -1.000000,
	0.050000,
	14.344000,
	0.006041, 0.006041, 0.008373,
	0.300000,
	100.000000,
	0.150000,
	25.000000,
	0.150000,
	6.250000,
	0.000000,
	0.000000,
	1.100000,
	2.100000,
	3.100000,
	4.100000,
	5.100000,
	6.100000,
	7.100000,
	0.000000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	1.000000,
	0.121000,
	2.000000,
	0.121000,
	0.579547, 0.618686, 0.804559,
	7.316000,
	6.091000, 2.099000, 0.000000,
	0.051122, 0.000262, 0.113921,
	10.842000,
	-0.615000, -1.684000, -3.480000,
	-1.233000, 0.739000,
	0.008370, 0.201100, 0.288820,
	0.215000,
	1.020000,
	1.880000,
	0.346000,
	1.055000,
	1.290000, -0.860000,
	0.983000,
	1.157000, 1.567000,
	0.799000,
	2.217000, -3.253000,
	6.438000,
	1.301000,
	0.051120, 0.000260, 0.113920,
	3.909000,
	0.047000,
	1.049000,
	0.090000,
	0.659000, 0.671000,
	-1.095000,
	1.000000, 2.000000,
	3.724000,
	1.500000, 0.700000,
	0.500000,
	0.000000,
	0.000000,
	0.000000,
	-6.000000,
	-10.000000,
	0.000000,
	-20.000000,
	0.000000,
	-4.000000,
	0.000000,
	-30.000000,
	-30.000000,
	0.000000,
	0.022000,
	0.000000, 0.000000, 0.000000,
	-1.544470, 0.825506, 0.430652,
	-1.544470, 0.056730, 0.858811,
	0.743080, 0.626954, 6.105698,
	2.003400, 1.186515, 11.294835,
	0.000000, 0.000000, 18.748707,
	0.000000, 0.000000, 18.748707,
	144.802246, 0.000000, 18.748707,
	144.802246, 0.000000, 18.748707,
	145.191925, 5.790454, 53.667583,
	145.191925, 5.790454, 53.667583,
	145.191925, 5.790454, 53.667583,
	137.764282, 3.357788, -7.604085,
	148.261414, 5.147290, -49.693699,
	0.000000, 14.315627, 38.314686,
	0.000000, 14.315627, 38.314686,
	0.000000, -5.298665, 24.879808,
	0.239437, 0.516333, 2.532136,
	0.129752, 0.335551, -0.396102,
	0.000000, 0.000000, 18.748707,
	83.886711, 0.000000, 18.748707,
	0.000000, 0.000000, 18.748707,
	0.000000, 0.000000, 18.748707,
	-2.427220, 6.322762, 0.423990,
	-4.070845, 6.322762, -10.818769,
	0.000000, 34.998001, -13.000000,
	0.000000, 34.998001, -13.000000,
	0.000000, 29.284887, 34.559925,
	0.000000, 28.889145, 34.327026,
	0.000000, 17.412588, 27.572929,
	0.000000, 14.315627, 38.314686,
	0.000000, 14.315627, 38.314686,
	0.000000, 14.315627, 38.314686,
	83.886711, 0.000000, 18.748707,
	144.802246, 0.000000, 18.748707,
	144.802246, 0.000000, 18.748707,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	-0.499985, -0.500013, -0.500013, 0.499985,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.007454, -0.017045, 0.000127, 0.999827,
	0.019866, 0.087807, -0.001751, 0.995937,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.102092, 0.000000, 0.000000, 0.994775,
	0.098840, 0.000000, 0.000000, 0.995103,
	0.208092, 0.000000, 0.000000, 0.978109,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	-0.499985, -0.500013, -0.500013, 0.499985,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.024000,
	3.000000,
	0.000000,
	0.000000,
	0.000000,
	0.000000, 0.000000, 1.000000,
	1.200000,
	0.060032, 0.019918, 0.242796,
	6.797000,
	0.900000,
	15.120000,
	3.314000,
	3.557000,
	20.000000,
	0.073000,
	0.624000,
	0.275000,
	0.015000,
	4.941000,
	3.444000,
	0.009000,
	0.777000,
	0.000000,
	-1.000000,
	-1.000000,
	-1.000000,
	-0.944000,
	0.666117, 0.789314, 0.991393,
	2.301000,
	0.000000,
	1.000000,
	0.250000,
	1.500000,
	2.025000,
	10.000000,
	1.000000,
	4.000000,
	6.632000,
	0.500000,
	0.478431, 0.035294, 0.007843,
	8.000000,
	1.192000, 1.000000,
	300.000000,
	-1.600000,
	4.036000,
	0.291000,
	0.102000,
	250.000000,
	16.011000,
	0.086000,
	0.152000,
	1.000000,
	7.124000, 2.098000,
	-2.445000, -0.118000,
	60.068001,
	0.611000,
	1.000000,
	1.000000,
	1.000000,
	0.560499, 0.511398, 0.796917,
	0.017000,
	0.137000,
	1.000000,
	0.137000,
	0.000000,
	0.137000,
	1.000000,
	0.137000,
	0.000000,
	1.061000,
	35.499001,
	1.397000,
	59.500000,
	0.598942, 0.819964, 0.325037,
	1.253000,
	13.541000,
	30.000000,
	0.000000, -4.224000,
	2.468000, 0.000000,
	-5.000000, -1.992000,
	52.527000,
	0.003000,
	100.000000,
	-0.060000,
	-8.974000,
	0.125000,
	0.111000,
	0.000000,
	0.000000,
	160.000000,
	160.000000,
	0.000000,
	292.000000,
	0.000000,
	156.000000,
	0.000000,
	256.000000,
	0.000000,
	256.000000,
	0.000000,
	524.000000,
	0.000000,
	576.000000,
	0.000000,
	448.000000,
	448.000000,
	0.493616, 0.932277, 0.737205,
	0.907547, 0.867136, 0.915750,
	0.388910, 0.160444, 0.124741,
	1.000000,
	0.500000,
	0.500000,
	4.000000,
	4.000000,
	8.000000, 5.000000,
	20.000000,
	20.000000,
	60.000000,
	60.000000,
	63.000000,
	1.000000,
	2.000000,
	2.000000,
	0.500000,
	0.988000,
	-0.721000,
	0.050000,
	0.300000,
	0.300000,
	0.224000,
	1.000000, 1.000000, 1.000000,
	15.319000,
	0.000000, 0.000000, 247.561005,
	16.700001,
	0.000000,
	1.000000,
	200.000000,
	1.000000,
	0.061000,
	0.020000,
	0.010000,
	0.020000,
	0.089000,
	0.025000,
	0.170000,
	0.000000,
	0.000000,
	0.728000, 0.594000,
	0.490000,
	0.536000,
	0.690000, 2.636000,
	0.251000,
	0.659000,
	0.189000, 0.435000,
	0.234000,
	3.704000,
	1.000000,
	0.000000, 0.000000,
	-8.884000, 0.668000,
	6.764000, 0.836000,
	0.000000, 0.000000,
	0.000000,
	1.000000,
	1.000000,
	0.047000,
	1.000000,
	0.047000,
	1.000000,
	0.047000,
	0.481952, 0.035614, 0.009696,
	0.000000,
	1.000000,
	8.780000,
	2.000000,
	2.000000,
	0.000000,
	0.000000,
	2.000000,
	0.100000,
	1.000000,
	0.145000,
	-0.407928, -0.009129, 0.912422, 0.029102,
	0.158380, -0.009129, 0.912422, 0.029102,
	0.722817, -0.009435, 0.943013, 0.030078,
	-0.025457, -0.009696, 0.969049, 0.030909,
	-0.305443, -0.009741, 0.973609, 0.031054,
	1.206115, -0.009508, 0.950300, 0.030311,
	1.236288, -0.011238, 1.123220, 0.035826,
	1.000000,
	2.976000,
	0.500000,
	0.700000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	1.000000,
	0.132000,
	0.941000,
	0.100000,
	1.000000,
	6.000000,
	2.000000,
	0.408000,
	1.209000,
	0.500000, 0.500000, 0.500000,
	0.500000, 0.500000, 0.500000,
	1.000000, 1.000000, 1.000000,
	0.000000,
	0.000000,
	0.000000,
	3.393000,
	0.411000,
	4.184000,
	0.000000,
	0.016000,
	-0.012000,
	0.000000,
	0.848000,
	0.058000, 0.724000,
	0.008373, 0.201096, 0.288816,
	5.655000,
	1.000000, 1.000000,
	1.000000,
	0.000000,
	0.000000,
	0.000000,
	1.740000,
	0.463000,
	0.832000,
	1.085000,
	0.913000,
	0.371000,
	0.050000,
	0.527000,
	0.000000,
	-100.000000,
	0.000000,
	1.000000,
	1.000000,
	0.000000,
	-50.000000,
	20.000000,
	0.000000,
	0.000000,
	0.000000,
	0.298000,
	0.298000,
	14.000000,
	0.000000,
	0.200000,
	0.200000,
	1.000000,
	0.500000,
	0.400000,
	0.059000,
	1.266000,
	1.000000,
	1.000000,
	1.000000,
	1.000000,
	1.000000, 1.000000,
};
Sequence sequence_data[99] = {
	{358000, 363500, 1, 1},
	{357000, 363500, 1, 1},
	{356000, 363500, 1, 1},
	{27500, 59250, 0, 8},
	{27750, 31000, 2, 2},
	{31750, 35000, 2, 2},
	{35750, 39000, 2, 2},
	{39750, 43000, 2, 2},
	{43750, 47000, 2, 2},
	{47750, 51000, 2, 2},
	{51750, 55000, 2, 2},
	{55750, 59000, 2, 2},
	{75750, 79000, 2, 2},
	{79750, 83000, 2, 2},
	{83750, 87000, 2, 2},
	{87750, 91000, 2, 2},
	{91750, 95000, 2, 2},
	{95750, 99000, 2, 2},
	{103750, 107000, 2, 2},
	{111750, 115000, 2, 2},
	{107750, 111000, 2, 2},
	{115750, 119000, 2, 2},
	{119750, 123000, 2, 2},
	{0, 56500, 0, 1},
	{56500, 58500, 1, 2},
	{58500, 59500, 1, 2},
	{392500, 395500, 1, 1},
	{59500, 124000, 0, 1},
	{395500, 403000, 0, 1},
	{452500, 457500, 1, 3},
	{512000, 612000, 0, 1},
	{0, 58500, 2, 4},
	{58500, 156000, 0, 1},
	{236000, 299500, 0, 1},
	{363500, 383500, 1, 4},
	{383500, 395500, 2, 3},
	{446000, 457500, 2, 5},
	{300000, 334000, 1, 2},
	{156000, 188000, 0, 1},
	{188000, 206000, 2, 4},
	{206000, 236000, 0, 1},
	{395500, 446000, 1, 6},
	{333500, 363500, 1, 3},
	{0, 58500, 0, 1},
	{58500, 156000, 0, 1},
	{188000, 206000, 1, 2},
	{363500, 383500, 1, 4},
	{383500, 395500, 1, 3},
	{446000, 457500, 1, 5},
	{300000, 334000, 1, 2},
	{156000, 188000, 0, 1},
	{206000, 236000, 0, 1},
	{236000, 299500, 0, 1},
	{395500, 446000, 1, 6},
	{333500, 363500, 1, 3},
	{395500, 455500, 0, 1},
	{0, 58500, 0, 1},
	{156000, 235500, 0, 1},
	{235500, 236000, 1, 2},
	{236000, 238000, 0, 1},
	{143250, 157000, 2, 4},
	{262500, 266500, 2, 4},
	{266500, 270500, 2, 4},
	{363500, 395500, 1, 3},
	{307500, 344000, 1, 2},
	{344000, 363500, 1, 2},
	{156000, 188000, 1, 2},
	{124000, 156000, 1, 2},
	{58500, 124000, 1, 2},
	{236000, 307500, 1, 2},
	{395500, 457500, 1, 3},
	{58000, 124000, 1, 4},
	{370500, 395500, 1, 2},
	{188000, 236000, 0, 1},
	{370500, 395500, 1, 2},
	{188000, 236000, 0, 1},
	{395000, 456000, 0, 1},
	{370500, 395500, 1, 2},
	{395000, 456000, 0, 1},
	{92000, 96000, 0, 1},
	{363500, 395500, 0, 1},
	{156000, 188000, 0, 1},
	{124000, 156000, 0, 1},
	{404000, 457000, 0, 1},
	{236500, 364000, 0, 1},
	{59750, 60750, 1, 1},
	{63750, 66000, 2, 2},
	{67750, 70000, 2, 2},
	{71750, 74000, 2, 2},
	{71750, 74000, 0, 1},
	{188000, 236000, 1, 4},
	{75500, 97500, 1, 6},
	{63750, 66000, 0, 1},
	{67750, 70000, 0, 1},
	{188000, 194000, 0, 1},
	{200000, 236000, 1, 3},
	{188000, 236000, 1, 2},
	{204000, 236000, 1, 4},
	{223500, 236000, 1, 2},
};
int sequence_index[222] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1,
	2,
	3,
	4,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	31,
	31,
	43,
	55,
	55,
	55,
	55,
	55,
	55,
	55,
	55,
	55,
	55,
	56,
	56,
	56,
	56,
	56,
	56,
	56,
	56,
	56,
	56,
	56,
	56,
	59,
	59,
	60,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	61,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	63,
	71,
	71,
	71,
	71,
	72,
	72,
	72,
	74,
	77,
	77,
	79,
	79,
	79,
	79,
	79,
	80,
	80,
	80,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	89,
	89,
	90,
	91,
	91,
	91,
	91,
	91,
	92,
	92,
	92,
	92,
	92,
	93,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	94,
	96,
	97,
	98,
	98,
	99,
	99,
	99,
	99,
	99,
	99,
	99,
	99,
	99,
	99,
	99,
};
int keyframe_index[222] = {
	0,
	3,
	4,
	5,
	8,
	9,
	10,
	13,
	15,
	17,
	19,
	28,
	67,
	70,
	71,
	74,
	77,
	78,
	81,
	83,
	86,
	87,
	88,
	89,
	90,
	91,
	93,
	94,
	96,
	97,
	99,
	100,
	101,
	104,
	105,
	106,
	107,
	108,
	110,
	111,
	113,
	114,
	116,
	117,
	130,
	131,
	239,
	363,
	364,
	365,
	366,
	367,
	368,
	371,
	372,
	375,
	376,
	378,
	379,
	380,
	381,
	382,
	383,
	384,
	385,
	386,
	387,
	388,
	389,
	394,
	397,
	399,
	404,
	405,
	406,
	407,
	408,
	411,
	412,
	414,
	415,
	416,
	417,
	418,
	419,
	420,
	421,
	422,
	423,
	424,
	426,
	428,
	429,
	430,
	431,
	432,
	433,
	436,
	445,
	446,
	447,
	448,
	449,
	452,
	453,
	454,
	455,
	457,
	459,
	461,
	462,
	463,
	464,
	465,
	466,
	467,
	468,
	487,
	490,
	493,
	496,
	501,
	503,
	504,
	508,
	513,
	514,
	518,
	521,
	522,
	525,
	526,
	528,
	529,
	530,
	536,
	537,
	538,
	539,
	541,
	542,
	543,
	545,
	546,
	547,
	549,
	550,
	551,
	552,
	554,
	556,
	558,
	560,
	568,
	571,
	573,
	578,
	579,
	580,
	581,
	582,
	610,
	611,
	612,
	613,
	614,
	616,
	618,
	619,
	620,
	621,
	622,
	623,
	624,
	625,
	626,
	627,
	630,
	633,
	636,
	637,
	638,
	639,
	640,
	641,
	642,
	643,
	644,
	645,
	646,
	647,
	649,
	652,
	653,
	655,
	656,
	657,
	658,
	659,
	660,
	661,
	662,
	663,
	664,
	665,
	666,
	667,
	672,
	675,
	680,
	681,
	684,
	685,
	686,
	687,
	688,
	689,
	690,
	691,
	692,
	693,
	695,
};
Uniform uniforms[221] = {
	{2, 98 },
	{0, 103 },
	{0, 102 },
	{2, 99 },
	{0, 94 },
	{0, 96 },
	{4, 95 },
	{0, 100 },
	{0, 101 },
	{0, 97 },
	{0, 170 },
	{0, 175 },
	{4, 174 },
	{0, 172 },
	{2, 173 },
	{4, 177 },
	{0, 176 },
	{2, 171 },
	{1, 178 },
	{4, 113 },
	{0, 129 },
	{0, 146 },
	{0, 120 },
	{0, 130 },
	{0, 121 },
	{1, 124 },
	{0, 119 },
	{1, 118 },
	{0, 116 },
	{1, 115 },
	{0, 154 },
	{0, 128 },
	{4, 112 },
	{0, 143 },
	{0, 133 },
	{0, 111 },
	{0, 158 },
	{1, 155 },
	{0, 134 },
	{1, 142 },
	{0, 153 },
	{1, 136 },
	{0, 157 },
	{0, 93 },
	{0, 69 },
	{2, 67 },
	{5, 68 },
	{0, 70 },
	{0, 65 },
	{0, 64 },
	{0, 105 },
	{0, 107 },
	{2, 106 },
	{0, 169 },
	{4, 167 },
	{0, 166 },
	{0, 168 },
	{0, 126 },
	{0, 139 },
	{0, 122 },
	{0, 148 },
	{0, 137 },
	{0, 147 },
	{0, 149 },
	{0, 141 },
	{0, 145 },
	{0, 144 },
	{0, 138 },
	{0, 123 },
	{4, 117 },
	{0, 156 },
	{0, -1 },
	{0, -1 },
	{0, 187 },
	{0, 189 },
	{0, 188 },
	{4, 193 },
	{0, 185 },
	{1, 203 },
	{0, 186 },
	{0, 201 },
	{0, 192 },
	{0, 183 },
	{0, 194 },
	{0, 208 },
	{0, 230 },
	{0, 221 },
	{0, 234 },
	{0, -1 },
	{1, 151 },
	{1, 150 },
	{0, 235 },
	{0, 229 },
	{0, 211 },
	{0, 209 },
	{0, 210 },
	{4, 207 },
	{0, 214 },
	{0, 206 },
	{0, 231 },
	{0, 225 },
	{0, 219 },
	{4, 233 },
	{0, 205 },
	{0, 222 },
	{0, 227 },
	{1, 220 },
	{1, 218 },
	{1, 226 },
	{0, 216 },
	{0, -1 },
	{0, -1 },
	{0, 213 },
	{0, 217 },
	{0, 212 },
	{0, 215 },
	{0, 159 },
	{4, 132 },
	{4, 114 },
	{4, 254 },
	{0, 241 },
	{1, 160 },
	{0, 161 },
	{0, 162 },
	{0, 163 },
	{0, -1 },
	{0, 164 },
	{4, 125 },
	{0, 131 },
	{2, 135 },
	{0, 127 },
	{6, 165 },
	{0, 108 },
	{0, 104 },
	{0, 152 },
	{0, 140 },
	{0, -1 },
	{0, -1 },
	{1, 195 },
	{0, 197 },
	{0, 196 },
	{1, 190 },
	{0, 202 },
	{0, 199 },
	{1, 191 },
	{0, 198 },
	{0, 200 },
	{0, -1 },
	{1, 255 },
	{1, 248 },
	{1, 247 },
	{1, 246 },
	{0, 239 },
	{4, 110 },
	{6, 249 },
	{0, 109 },
	{0, 242 },
	{0, 257 },
	{0, 252 },
	{0, 253 },
	{5, 250 },
	{0, 256 },
	{0, 244 },
	{0, 245 },
	{0, 243 },
	{6, 240 },
	{6, 251 },
	{0, -1 },
	{0, 74 },
	{0, 76 },
	{0, 77 },
	{0, 73 },
	{0, 72 },
	{0, 71 },
	{0, 79 },
	{0, 78 },
	{4, 89 },
	{4, 90 },
	{4, 91 },
	{6, 75 },
	{6, 88 },
	{6, 92 },
	{0, 86 },
	{0, 85 },
	{0, 87 },
	{0, 80 },
	{0, 81 },
	{0, 82 },
	{0, 84 },
	{0, 83 },
	{1, 259 },
	{4, 204 },
	{0, 258 },
	{1, 260 },
	{0, 261 },
	{6, 236 },
	{0, 238 },
	{0, 237 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, 180 },
	{0, 179 },
	{0, 182 },
	{0, 184 },
	{0, 181 },
	{0, -1 },
	{0, 223 },
	{0, 224 },
	{0, 232 },
	{0, 228 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{1, -1 },
};
int keyframe_time[214] = {
	0,
	0,
	0,
	0,
	4000,
	8000,
	12000,
	16000,
	20000,
	24000,
	28000,
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
	0,
	0,
	2000,
	0,
	1000,
	3000,
	0,
	0,
	0,
	2500,
	5000,
	0,
	0,
	8000,
	28000,
	58500,
	0,
	0,
	0,
	3750,
	7000,
	20000,
	0,
	5000,
	12000,
	0,
	4500,
	5750,
	6750,
	11500,
	0,
	34000,
	0,
	0,
	3000,
	8000,
	18000,
	0,
	0,
	7000,
	7500,
	15500,
	31500,
	50500,
	0,
	22500,
	30000,
	0,
	0,
	0,
	18000,
	0,
	3750,
	7000,
	20000,
	0,
	5000,
	12000,
	0,
	4500,
	5750,
	6750,
	11500,
	0,
	34000,
	0,
	0,
	0,
	0,
	7000,
	7500,
	15500,
	31500,
	50500,
	0,
	22500,
	30000,
	0,
	0,
	0,
	0,
	500,
	0,
	0,
	11000,
	12000,
	13750,
	1500,
	2000,
	2500,
	4000,
	1500,
	2000,
	2500,
	4000,
	0,
	20000,
	32000,
	0,
	36500,
	0,
	19500,
	0,
	32000,
	0,
	32000,
	0,
	65500,
	0,
	71500,
	0,
	56000,
	62000,
	0,
	62000,
	64000,
	66000,
	7000,
	25000,
	0,
	7000,
	25000,
	0,
	0,
	7000,
	25000,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	250,
	250,
	875,
	250,
	875,
	250,
	875,
	0,
	0,
	37500,
	44000,
	48000,
	3000,
	6000,
	7000,
	11500,
	15500,
	18500,
	0,
	0,
	0,
	0,
	4000,
	36000,
	0,
	48000,
	0,
	1500,
	31500,
	32000,
	11000,
	12500,
};
bool keyframe_ease_toward[214] = {
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
	true,
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
	true,
	false,
	false,
	false,
	false,
	true,
	false,
	false,
	false,
	true,
	false,
	false,
	false,
	false,
	true,
	false,
	false,
	false,
	false,
	false,
	true,
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
	true,
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
	true,
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
	true,
	false,
	false,
	false,
	false,
	false,
	true,
	false,
	false,
	false,
};
bool keyframe_ease_away[214] = {
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
	true,
	false,
	false,
	false,
	false,
	false,
	false,
	true,
	false,
	false,
	false,
	false,
	false,
	false,
	true,
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
	true,
	false,
	false,
	false,
	false,
	false,
	false,
	true,
	false,
	true,
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
	true,
	false,
	false,
	false,
	false,
};
int keyframe_time_index[222] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1,
	2,
	3,
	11,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	49,
	61,
	61,
	96,
	126,
	126,
	126,
	126,
	126,
	126,
	126,
	126,
	126,
	126,
	127,
	127,
	127,
	127,
	127,
	127,
	127,
	127,
	127,
	127,
	127,
	127,
	131,
	131,
	132,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	136,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	144,
	162,
	162,
	162,
	162,
	166,
	166,
	166,
	169,
	173,
	173,
	176,
	176,
	176,
	176,
	176,
	177,
	177,
	177,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	182,
	189,
	189,
	190,
	194,
	194,
	194,
	194,
	194,
	200,
	200,
	200,
	200,
	200,
	201,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	202,
	206,
	208,
	212,
	212,
	214,
	214,
	214,
	214,
	214,
	214,
	214,
	214,
	214,
	214,
	214,
};
#endif
