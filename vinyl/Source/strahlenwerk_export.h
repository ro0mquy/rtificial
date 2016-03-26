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
#include "shaders/bridge.h"
#include "shaders/end.h"
#include "shaders/f16_beginning.h"
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
Shader scenes[10] = {
	Shader(bridge_source, 0, nullptr),
	Shader(end_source, 0, nullptr),
	Shader(f16_beginning_source, 0, nullptr),
	Shader(f16_takeoff_source, 0, nullptr),
	Shader(funk_source, 0, nullptr),
	Shader(landscape_source, 0, nullptr),
	Shader(parl_source, 0, nullptr),
	Shader(parl_schwurbel_source, 0, nullptr),
	Shader(part_source, 0, nullptr),
	Shader(rain_source, 0, nullptr),
};
AmbientLight environments[10] = {
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
Scene scenes_data[10] = {
	{0, 58500, 2},
	{58500, 124000, 8},
	{124000, 156000, 9},
	{156000, 188000, 4},
	{188000, 236000, 3},
	{236000, 307500, 5},
	{307500, 344000, 0},
	{344000, 363500, 6},
	{363500, 395500, 7},
	{395500, 457500, 1},
};
float keyframe_data[852] = {
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
	0.655000,
	0.084000,
	0.463000,
	0.184000,
	1.000000,
	0.121000,
	0.623000,
	0.121000,
	0.162000,
	0.121000,
	0.366000,
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
	0.073828, 0.119264, 0.447871,
	0.421510, 0.462616, 0.693094,
	0.000805, 0.011881, 0.254916,
	7.316000,
	6.091000, 2.099000, 0.000000,
	0.476177, 0.035614, 0.007751,
	10.842000,
	-0.615000, -1.684000, -3.480000,
	-1.233000, 0.739000,
	0.000000, 0.000000, 0.000000,
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
	0.487765, 0.113921, 0.659224,
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
	3.000000,
	3.000000,
	2.400000,
	-6.000000,
	-10.000000,
	0.000000,
	2.500000,
	2.000000,
	0.000000,
	-20.000000,
	-20.000000,
	-2.000000,
	-2.000000,
	0.000000,
	-30.000000,
	-30.000000,
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
	137.764282, -0.197992, -7.604085,
	138.559128, -0.197992, -30.915073,
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
	-4.070845, 7.297157, -10.818769,
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
	0.060953, -0.063901, 0.003910, 0.996084,
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
	0.003000,
	0.005000,
	0.000000,
	0.000000,
	0.000000, 0.000000, 1.000000,
	0.478431, 0.035294, 0.007843,
	0.176471, 0.176471, 0.176471,
	8.780000,
	1.200000,
	0.103634, 0.027755, 0.133209,
	6.797000,
	0.900000,
	15.120000,
	3.314000,
	3.557000,
	20.000000,
	0.180000, 0.180000, 0.180000,
	1.000000,
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
	0.560499, 0.722672, 1.000000,
	2.301000,
	0.000000,
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
	0.260000,
	1.000000,
	7.124000, 2.098000,
	-2.445000, -0.118000,
	60.068001,
	0.611000,
	1.000000,
	0.260000,
	0.230000,
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
	0.487765, 0.116576, 0.659224,
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
	170.000000,
	170.000000,
	0.000000,
	292.000000,
	0.000000,
	384.000000,
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
	0.499505, 0.859174, 0.774227,
	0.103634, 0.176774, 0.141980,
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
	1.000000,
	1.200000,
	1.200000,
	1.200000,
	1.200000,
	0.700000,
	0.700000,
	0.000000,
	1.000000,
	0.850000,
	200.000000,
	1.000000,
	0.061000,
	0.020000,
	0.040000,
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
	0.486275, 0.113725, 0.658824,
	6.860000,
	30.000000,
	30.000000,
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
	1.196552, -0.009508, 0.950300, 0.030311,
	1.236288, -0.011238, 1.123220, 0.035826,
	1.000000,
	3.000000,
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
	5.000000,
	21.736000,
	5.000000,
	5.000000,
	34.318001,
	5.000000,
	0.500000, 0.500000, 0.500000,
	0.500000, 0.500000, 0.500000,
	0.415148, 0.399293, 0.431340,
	0.415148, 0.399293, 0.431340,
	0.442323, 0.420508, 0.453456,
	0.442323, 0.420508, 0.453456,
	0.383775, 0.339223, 0.399293,
	0.368591, 0.311181, 0.383775,
	0.383775, 0.339223, 0.399293,
	0.436813, 0.353741, 0.459080,
	0.364706, 0.309804, 0.380392,
	0.500000, 0.500000, 0.500000,
	0.573159, 0.573159, 0.573159,
	0.476177, 0.476177, 0.476177,
	0.476177, 0.476177, 0.476177,
	0.694081, 0.694081, 0.694081,
	0.694081, 0.694081, 0.694081,
	0.883180, 0.883180, 0.883180,
	0.751895, 0.751895, 0.751895,
	0.573159, 0.573159, 0.573159,
	0.425905, 0.425905, 0.425905,
	0.476177, 0.476177, 0.476177,
	0.573159, 0.573159, 0.573159,
	0.453456, 0.453456, 0.453456,
	0.573159, 0.573159, 0.573159,
	0.573159, 0.573159, 0.573159,
	0.368591, 0.368591, 0.368591,
	0.573159, 0.573159, 0.573159,
	1.000000,
	1.005000,
	0.994000,
	0.994000,
	0.999000,
	0.999000,
	0.988000,
	0.988000,
	1.004000,
	1.004000,
	1.001000,
	1.001000,
	1.013000,
	1.002000,
	0.000000,
	0.000000,
	0.000000,
	3.393000,
	0.411000,
	4.184000,
	0.000000,
	0.002000,
	0.016000,
	-0.083000,
	0.016000,
	-0.012000,
	0.000000,
	0.025000,
	0.780000,
	0.058000, 0.724000,
	0.039947, 0.242796, 0.320382,
	0.039947, 0.242796, 0.320382,
	0.039947, 0.093876, 0.320382,
	0.039947, 0.242796, 0.320382,
	0.019918, 0.009021, 0.566810,
	5.655000,
	19.080999,
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
	1.000000, 1.000000, 1.000000,
	0.000000,
	-100.000000,
	0.000000,
	1.000000,
	1.000000,
	0.000000,
	-50.000000,
	-8.437500,
	-8.437000,
	0.000000,
	0.000000,
	0.000000,
	0.298000,
	0.298000,
	14.000000,
	0.000000,
	0.300000,
	0.300000,
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
Sequence sequence_data[123] = {
	{358000, 363500, 1, 1},
	{357000, 363500, 1, 1},
	{356000, 363500, 1, 1},
	{27500, 59250, 0, 8},
	{27750, 31000, 2, 2},
	{31750, 35000, 2, 2},
	{35750, 39000, 2, 2},
	{39750, 41750, 2, 2},
	{43750, 45875, 2, 2},
	{47750, 51000, 2, 2},
	{51750, 55000, 2, 2},
	{55750, 58500, 2, 2},
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
	{0, 49000, 1, 3},
	{56500, 58500, 1, 3},
	{58500, 59500, 1, 2},
	{389375, 395500, 1, 4},
	{395500, 396000, 1, 2},
	{396000, 403000, 0, 1},
	{452500, 457500, 1, 3},
	{0, 58500, 2, 4},
	{58500, 156000, 0, 1},
	{236000, 299500, 0, 1},
	{363500, 379750, 1, 4},
	{379750, 395500, 2, 3},
	{446000, 457500, 2, 5},
	{299500, 333500, 1, 2},
	{156000, 188000, 0, 1},
	{188000, 206000, 2, 4},
	{206000, 236000, 0, 1},
	{395500, 446000, 1, 6},
	{333500, 363500, 1, 3},
	{0, 58500, 0, 1},
	{58500, 156000, 0, 1},
	{188000, 206000, 1, 2},
	{363500, 379750, 1, 4},
	{379750, 395500, 2, 2},
	{446000, 457500, 1, 5},
	{299500, 333500, 1, 2},
	{156000, 188000, 0, 1},
	{206000, 236000, 0, 1},
	{236000, 299500, 0, 1},
	{395500, 446000, 1, 6},
	{333500, 363500, 1, 3},
	{300000, 364000, 1, 2},
	{395500, 403000, 0, 1},
	{395500, 457500, 0, 1},
	{0, 58500, 0, 1},
	{156000, 235500, 0, 1},
	{235500, 236000, 1, 2},
	{236000, 245000, 1, 2},
	{143250, 157000, 2, 4},
	{262500, 266500, 2, 4},
	{266500, 270500, 2, 4},
	{363500, 395500, 2, 4},
	{307500, 344000, 1, 2},
	{188000, 236000, 1, 2},
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
	{134000, 155500, 1, 2},
	{295000, 311500, 1, 2},
	{343500, 395500, 1, 4},
	{92000, 96000, 0, 1},
	{363500, 395500, 0, 1},
	{156000, 188000, 0, 1},
	{124000, 156000, 0, 1},
	{404000, 457000, 0, 1},
	{236000, 363500, 0, 1},
	{387750, 389062, 2, 1},
	{391750, 393062, 2, 1},
	{59750, 60750, 1, 1},
	{63750, 66000, 2, 2},
	{67750, 70000, 2, 2},
	{71750, 74000, 2, 2},
	{71750, 74000, 0, 1},
	{188000, 236000, 1, 4},
	{75500, 97500, 1, 6},
	{63750, 66000, 0, 1},
	{67750, 70000, 0, 1},
	{156000, 188000, 2, 6},
	{0, 395500, 1, 7},
	{395500, 457500, 0, 2},
	{0, 262125, 0, 1},
	{363500, 395500, 1, 4},
	{395500, 457500, 0, 2},
	{270500, 363500, 0, 3},
	{262125, 266312, 1, 3},
	{266312, 270500, 1, 3},
	{0, 58500, 0, 1},
	{58000, 395500, 1, 10},
	{395500, 457500, 0, 2},
	{203000, 211000, 2, 1},
	{156000, 188000, 1, 2},
	{131000, 138000, 1, 1},
	{125000, 156000, 1, 4},
	{133000, 141000, 1, 1},
	{188000, 194000, 0, 1},
	{200000, 236000, 1, 3},
	{188000, 236000, 1, 3},
	{204000, 236000, 1, 4},
	{223500, 236000, 1, 2},
};
int sequence_index[232] = {
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
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	24,
	30,
	30,
	42,
	54,
	54,
	54,
	55,
	55,
	55,
	55,
	56,
	56,
	56,
	56,
	56,
	57,
	57,
	57,
	57,
	57,
	57,
	57,
	57,
	57,
	57,
	57,
	57,
	57,
	57,
	60,
	60,
	61,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	62,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	73,
	73,
	73,
	73,
	74,
	74,
	74,
	76,
	79,
	79,
	81,
	81,
	81,
	81,
	81,
	84,
	85,
	85,
	85,
	85,
	90,
	90,
	90,
	90,
	90,
	90,
	90,
	90,
	90,
	90,
	90,
	90,
	90,
	92,
	92,
	92,
	92,
	92,
	92,
	92,
	96,
	96,
	97,
	98,
	98,
	98,
	98,
	98,
	99,
	99,
	99,
	99,
	99,
	100,
	101,
	101,
	101,
	101,
	101,
	101,
	101,
	101,
	101,
	102,
	102,
	104,
	110,
	113,
	113,
	113,
	113,
	113,
	113,
	113,
	114,
	115,
	115,
	116,
	116,
	116,
	117,
	118,
	118,
	118,
	118,
	118,
	118,
	118,
	118,
	118,
	118,
	118,
	118,
	118,
	118,
	118,
	120,
	121,
	122,
	122,
	123,
	123,
	123,
	123,
	123,
	123,
	123,
	123,
	123,
	123,
	123,
};
int keyframe_index[232] = {
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
	79,
	80,
	83,
	86,
	87,
	90,
	92,
	95,
	96,
	97,
	98,
	99,
	100,
	102,
	103,
	105,
	106,
	108,
	109,
	110,
	113,
	114,
	115,
	116,
	117,
	119,
	120,
	122,
	123,
	125,
	126,
	142,
	143,
	251,
	371,
	372,
	373,
	376,
	377,
	378,
	381,
	387,
	388,
	389,
	392,
	393,
	395,
	396,
	397,
	398,
	401,
	402,
	403,
	404,
	405,
	406,
	407,
	408,
	409,
	410,
	415,
	418,
	421,
	426,
	427,
	428,
	429,
	430,
	433,
	434,
	436,
	437,
	438,
	439,
	440,
	441,
	442,
	443,
	444,
	445,
	446,
	448,
	450,
	451,
	452,
	453,
	454,
	455,
	458,
	467,
	468,
	469,
	470,
	471,
	474,
	475,
	476,
	477,
	479,
	481,
	483,
	484,
	485,
	486,
	487,
	488,
	489,
	490,
	512,
	515,
	518,
	521,
	526,
	528,
	529,
	533,
	538,
	539,
	543,
	546,
	547,
	550,
	551,
	560,
	562,
	563,
	564,
	565,
	571,
	572,
	573,
	574,
	576,
	577,
	578,
	580,
	581,
	582,
	584,
	585,
	588,
	591,
	592,
	593,
	595,
	597,
	599,
	601,
	609,
	612,
	614,
	619,
	620,
	621,
	622,
	623,
	651,
	652,
	653,
	654,
	655,
	657,
	659,
	660,
	661,
	662,
	663,
	664,
	665,
	666,
	667,
	674,
	677,
	707,
	758,
	772,
	773,
	774,
	775,
	776,
	777,
	778,
	780,
	783,
	784,
	786,
	787,
	789,
	804,
	806,
	808,
	809,
	810,
	811,
	812,
	813,
	814,
	815,
	816,
	817,
	818,
	819,
	820,
	823,
	828,
	832,
	837,
	838,
	841,
	842,
	843,
	844,
	845,
	846,
	847,
	848,
	849,
	850,
	852,
};
Uniform uniforms[231] = {
	{2, 125 },
	{0, 111 },
	{0, 117 },
	{2, 130 },
	{0, 120 },
	{0, 142 },
	{4, 126 },
	{0, 133 },
	{0, 135 },
	{0, 127 },
	{0, 176 },
	{0, 183 },
	{4, 181 },
	{0, 178 },
	{2, 180 },
	{4, 182 },
	{0, 184 },
	{2, 177 },
	{1, 179 },
	{4, 98 },
	{0, 122 },
	{0, 137 },
	{0, 107 },
	{0, 123 },
	{0, 109 },
	{1, 113 },
	{0, 106 },
	{1, 105 },
	{0, 103 },
	{1, 102 },
	{0, 153 },
	{0, 121 },
	{4, 97 },
	{0, 141 },
	{0, 129 },
	{0, 96 },
	{0, 157 },
	{1, 154 },
	{0, 148 },
	{1, 140 },
	{0, 152 },
	{1, 134 },
	{0, 156 },
	{0, 119 },
	{0, 69 },
	{2, 67 },
	{5, 68 },
	{0, 70 },
	{0, 65 },
	{0, 64 },
	{0, 159 },
	{0, 161 },
	{2, 160 },
	{4, 173 },
	{0, 172 },
	{0, 175 },
	{4, 171 },
	{0, 170 },
	{0, 174 },
	{0, 131 },
	{0, 147 },
	{0, 146 },
	{4, 100 },
	{0, 99 },
	{0, 144 },
	{0, 116 },
	{0, 143 },
	{0, 145 },
	{0, 139 },
	{0, 136 },
	{0, 110 },
	{0, 118 },
	{0, 112 },
	{4, 104 },
	{0, 155 },
	{0, -1 },
	{0, -1 },
	{0, 195 },
	{0, 197 },
	{0, 196 },
	{4, 201 },
	{0, 193 },
	{1, 211 },
	{0, 194 },
	{0, 209 },
	{0, 200 },
	{0, 189 },
	{0, 202 },
	{0, 216 },
	{0, 238 },
	{0, 228 },
	{0, 242 },
	{0, -1 },
	{1, 150 },
	{1, 149 },
	{0, 243 },
	{0, 237 },
	{0, 219 },
	{0, 217 },
	{0, 218 },
	{4, 215 },
	{0, 222 },
	{0, 214 },
	{0, 239 },
	{0, 227 },
	{0, 229 },
	{4, 241 },
	{0, 213 },
	{0, 230 },
	{0, 235 },
	{1, 232 },
	{1, 226 },
	{1, 231 },
	{0, 224 },
	{0, -1 },
	{0, -1 },
	{0, 221 },
	{0, 225 },
	{0, 220 },
	{0, 223 },
	{0, 163 },
	{4, 128 },
	{4, 101 },
	{4, 264 },
	{0, 251 },
	{1, 164 },
	{0, 165 },
	{0, 166 },
	{0, 167 },
	{0, -1 },
	{0, 168 },
	{4, 114 },
	{0, 124 },
	{2, 132 },
	{0, 115 },
	{0, 191 },
	{6, 169 },
	{0, 108 },
	{0, 162 },
	{0, 158 },
	{0, 151 },
	{0, 138 },
	{0, -1 },
	{0, -1 },
	{1, 203 },
	{0, 205 },
	{0, 204 },
	{1, 198 },
	{0, 210 },
	{0, 207 },
	{1, 199 },
	{0, 206 },
	{4, 245 },
	{0, 244 },
	{0, 208 },
	{0, -1 },
	{1, 265 },
	{1, 258 },
	{1, 257 },
	{1, 256 },
	{0, 249 },
	{4, 95 },
	{6, 259 },
	{0, 94 },
	{0, 252 },
	{0, 267 },
	{0, 262 },
	{0, 263 },
	{5, 260 },
	{0, 266 },
	{0, 254 },
	{0, 255 },
	{0, 253 },
	{6, 250 },
	{6, 261 },
	{0, -1 },
	{0, 74 },
	{0, 76 },
	{0, 77 },
	{0, 73 },
	{0, 72 },
	{0, 71 },
	{0, 79 },
	{0, 78 },
	{4, 90 },
	{4, 91 },
	{4, 92 },
	{0, 80 },
	{6, 75 },
	{6, 89 },
	{6, 93 },
	{0, 87 },
	{0, 86 },
	{0, 88 },
	{0, 81 },
	{0, 82 },
	{0, 83 },
	{0, 85 },
	{0, 84 },
	{1, 269 },
	{4, 212 },
	{0, 268 },
	{1, 270 },
	{0, 271 },
	{6, 246 },
	{0, 248 },
	{0, 247 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{4, 192 },
	{0, 186 },
	{0, 185 },
	{0, 188 },
	{0, 190 },
	{0, 187 },
	{0, -1 },
	{0, 234 },
	{0, 233 },
	{0, 240 },
	{0, 236 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{0, -1 },
	{1, -1 },
};
int keyframe_time[290] = {
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
	15500,
	26500,
	36000,
	0,
	1687,
	2000,
	0,
	1000,
	2250,
	3250,
	4125,
	6125,
	0,
	500,
	0,
	0,
	2500,
	5000,
	0,
	8000,
	28000,
	58500,
	0,
	0,
	0,
	3750,
	7000,
	16250,
	0,
	10250,
	15750,
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
	16250,
	0,
	15750,
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
	44500,
	0,
	0,
	0,
	0,
	0,
	500,
	0,
	2000,
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
	24500,
	32000,
	0,
	36500,
	0,
	48000,
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
	7000,
	21500,
	6000,
	12500,
	6000,
	23000,
	35000,
	52000,
	0,
	0,
	0,
	0,
	0,
	0,
	250,
	250,
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
	15375,
	18500,
	0,
	0,
	4000,
	6000,
	10000,
	14000,
	23000,
	28000,
	0,
	344000,
	346000,
	363500,
	365000,
	370500,
	395500,
	0,
	7500,
	0,
	0,
	1500,
	7000,
	32000,
	0,
	7500,
	0,
	37000,
	93000,
	0,
	2125,
	4187,
	0,
	2188,
	4188,
	0,
	0,
	65000,
	67000,
	97000,
	99000,
	128000,
	132000,
	168500,
	178000,
	337500,
	0,
	7500,
	3000,
	16000,
	26000,
	5000,
	5000,
	15000,
	25000,
	29500,
	4000,
	0,
	0,
	4000,
	36000,
	0,
	31000,
	48000,
	0,
	1500,
	31500,
	32000,
	11000,
	12500,
};
bool keyframe_ease_toward[290] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
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
	false,
	false,
	false,
	false,
	false,
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
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
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
};
bool keyframe_ease_away[290] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
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
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
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
int keyframe_time_index[232] = {
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
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	52,
	67,
	67,
	102,
	131,
	131,
	131,
	133,
	133,
	133,
	133,
	134,
	134,
	134,
	134,
	134,
	135,
	135,
	135,
	135,
	135,
	135,
	135,
	135,
	135,
	135,
	135,
	135,
	135,
	135,
	139,
	139,
	141,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	145,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	153,
	174,
	174,
	174,
	174,
	178,
	178,
	178,
	181,
	185,
	185,
	188,
	188,
	188,
	188,
	188,
	196,
	197,
	197,
	197,
	197,
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
	204,
	204,
	204,
	204,
	204,
	204,
	204,
	211,
	211,
	212,
	216,
	216,
	216,
	216,
	216,
	222,
	222,
	222,
	222,
	222,
	223,
	224,
	224,
	224,
	224,
	224,
	224,
	224,
	224,
	224,
	230,
	230,
	239,
	255,
	268,
	268,
	268,
	268,
	268,
	268,
	268,
	269,
	271,
	271,
	272,
	272,
	272,
	276,
	277,
	277,
	277,
	277,
	277,
	277,
	277,
	277,
	277,
	277,
	277,
	277,
	277,
	277,
	277,
	281,
	284,
	288,
	288,
	290,
	290,
	290,
	290,
	290,
	290,
	290,
	290,
	290,
	290,
	290,
};
#endif
