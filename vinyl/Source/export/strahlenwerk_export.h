#ifndef STRAHLENWERK_EXPORT
#define STRAHLENWERK_EXPORT
#include "../Shader.h"
#include "../Framebuffer.h"
#include "../Scene.h"
#include "../Sequence.h"
#include "../Uniform.h"
#include "../AmbientLight.h"
#include "../Texture.h"
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
#include "shaders/edge_sobel.h"
#include "shaders/bloom_up_final.h"
#include "shaders/edge_nonmax.h"
#include "shaders/final.h"
#include "shaders/output.h"
#include "shaders/blasen.h"
#include "shaders/extruder.h"
#include "shaders/hx.h"
#include "shaders/jas.h"
#include "shaders/jazz.h"
#include "shaders/jibjab.h"
#include "shaders/mandala.h"
#include "shaders/obszenchen.h"
#include "shaders/shadow.h"
#include "shaders/star.h"
#include "shaders/szenchen.h"
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
Input edge_sobel_inputs[2] = {
	{0, 0},
	{1, 0},
};
Input bloom_up_final_inputs[2] = {
	{8, 0},
	{22, 0},
};
Input edge_nonmax_inputs[1] = {
	{23, 0},
};
Input final_inputs[2] = {
	{24, 0},
	{25, 0},
};
Input output_inputs[1] = {
	{26, 0},
};
Shader postproc[25] = {
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
	Shader(edge_sobel_source, 2, edge_sobel_inputs),
	Shader(bloom_up_final_source, 2, bloom_up_final_inputs),
	Shader(edge_nonmax_source, 1, edge_nonmax_inputs),
	Shader(final_source, 2, final_inputs),
	Shader(output_source, 1, output_inputs),
};
Shader scenes[11] = {
	Shader(blasen_source, 0, nullptr),
	Shader(extruder_source, 0, nullptr),
	Shader(hx_source, 0, nullptr),
	Shader(jas_source, 0, nullptr),
	Shader(jazz_source, 0, nullptr),
	Shader(jibjab_source, 0, nullptr),
	Shader(mandala_source, 0, nullptr),
	Shader(obszenchen_source, 0, nullptr),
	Shader(shadow_source, 0, nullptr),
	Shader(star_source, 0, nullptr),
	Shader(szenchen_source, 0, nullptr),
};
AmbientLight environments[11] = {
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
Texture textures[0] = {
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
Output edge_sobel_outputs[1] = {
	{2, 23, 0},
};
Output bloom_up_final_outputs[1] = {
	{3, 24, 0},
};
Output edge_nonmax_outputs[1] = {
	{1, 25, 0},
};
Output final_outputs[1] = {
	{3, 26, 0},
};
Framebuffer fbos[25] = {
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
	Framebuffer(0, 1, edge_sobel_outputs),
	Framebuffer(0, 1, bloom_up_final_outputs),
	Framebuffer(0, 1, edge_nonmax_outputs),
	Framebuffer(0, 1, final_outputs),
};
Scene scenes_data[10] = {
	{0, 53000, 6},
	{53000, 92375, 6},
	{92375, 162000, 1},
	{166000, 186000, 9},
	{186000, 221000, 4},
	{222000, 244000, 0},
	{232000, 250000, 7},
	{244000, 288000, 2},
	{266000, 290000, 3},
	{288000, 340000, 8},
};
float keyframe_data[682] = {
	3.113000, 0.627000, 0.475000,
	3.113000, 0.627000, 0.475000,
	3.113000, -0.110000, 0.475000,
	2.909000, 0.644000, 0.475000,
	3.654000, 0.287000, 0.488000,
	1.857000,
	2.133000, 0.044000, 1.805000,
	2.133000, -0.321000, 1.805000,
	2.390000, 0.301000, 1.536000,
	2.290000, -0.270000, 1.676000,
	1.400000,
	0.155000,
	-0.777000, -2.229000, 1.788000,
	-0.777000, -2.088000, 1.788000,
	-0.777000, -2.257000, 1.788000,
	-0.456000, -2.188000, 2.463000,
	3.453000,
	0.555000,
	1.324000, 0.431000, 0.268000,
	1.324000, 0.287000, 0.268000,
	1.324000, -0.082000, 0.505000,
	1.480000, 0.804000, -0.069000,
	1.857000,
	0.191000,
	0.000000, 0.000000, 0.000000,
	0.241000,
	1.000000,
	0.000000,
	3.000000,
	-1.079000, 0.000000, -1.305000,
	-0.085000,
	0.000000,
	0.500000,
	0.011000,
	1.000000, 0.000000, 0.000000,
	-1.538000, 1.253000, 0.410000,
	1.117000,
	0.754000,
	0.267000,
	-5.000000,
	-5.288000,
	0.022000,
	0.000000, 0.000000, 0.000000,
	9.659001, 8.046491, 6.969036,
	-0.765631, -1.236915, 55.534149,
	9.659001, 8.046491, 6.969036,
	-0.765631, -1.236915, 55.534149,
	9.659001, 8.046491, 6.969036,
	0.925561, -4.312619, 22.913223,
	-38.810787, -4.312619, 33.588474,
	0.000000, 84.439552, -0.307163,
	0.000000, 84.439552, -0.307163,
	0.000000, 84.439552, -0.307163,
	0.000000, 26.262243, -0.307163,
	12.120131, 8.920786, 50.126095,
	49.532288, 8.920786, 10.235818,
	0.000000, 84.439552, -0.307163,
	-0.003889, 3.080715, 13.904042,
	-0.003889, 3.080715, -78.858368,
	-3.137317, 6.710683, 12.663469,
	0.000000, 84.439552, -0.307163,
	0.000000, 84.439552, -0.307163,
	0.000000, 279.159760, -0.307163,
	21.584818, 15.868284, 19.574572,
	0.000000, 0.000000, 0.000000, 1.000000,
	-0.239921, 0.395641, 0.107875, 0.879926,
	0.012299, -0.015550, 0.000191, 0.999805,
	-0.239921, 0.395641, 0.107875, 0.879926,
	0.012299, -0.015550, 0.000191, 0.999805,
	-0.239921, 0.395641, 0.107875, 0.879926,
	0.102464, 0.130150, -0.013524, 0.986093,
	0.102464, 0.130150, -0.013524, 0.986093,
	-0.709127, 0.000000, 0.000000, 0.705081,
	-0.709127, 0.000000, 0.000000, 0.705081,
	-0.709127, 0.000000, 0.000000, 0.705081,
	-0.709127, 0.000000, 0.000000, 0.705081,
	-0.071520, 0.121298, 0.008763, 0.989998,
	-0.055952, 0.628463, 0.045402, 0.774496,
	-0.709127, 0.000000, 0.000000, 0.705081,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.235283, 0.230774, 0.057617, -0.942374,
	-0.709127, 0.000000, 0.000000, 0.705081,
	-0.709127, 0.000000, 0.000000, 0.705081,
	-0.709127, 0.000000, 0.000000, 0.705081,
	-0.137992, 0.193734, 0.027535, 0.970910,
	0.024000,
	3.000000,
	0.000000,
	0.000000,
	0.000000,
	10.000000,
	0.873000, 0.270000,
	0.196000, 0.193000,
	-0.001000,
	0.464000,
	0.387000,
	0.192000, -0.193000,
	0.027755, 0.069727, 0.124741,
	0.766744, 0.311181, 0.227137,
	0.358654, 1.000000, 0.819964,
	8.000000,
	8.000000,
	16.000000,
	24.000000,
	12.000000,
	10.000000,
	50.000000,
	6.000000,
	16.000000,
	25.490000,
	25.490000,
	25.490000,
	13.904531,
	11.500000,
	11.500000,
	11.500000,
	16.000000,
	15.000000,
	16.000000,
	15.000000,
	16.000000,
	15.000000,
	16.000000,
	15.000000,
	16.000000,
	15.000000,
	16.000000,
	15.000000,
	16.000000,
	15.000000,
	16.000000,
	15.000000,
	16.000000,
	3.452000,
	0.500000,
	2.300000,
	2.300000,
	2.300000,
	0.500000,
	3.561000,
	0.500000,
	0.365000,
	3.561000,
	10.000000,
	0.000000,
	0.000000,
	1.000000,
	1.000000,
	0.000000,
	0.183000,
	1.169000,
	0.500000,
	1.250000,
	0.000000,
	0.000000,
	-0.103516,
	-0.125000,
	-0.125000,
	-0.125000,
	0.876000,
	0.876000,
	1.876000,
	1.376000,
	1.376000,
	2.869000,
	1.376000,
	2.869000,
	0.569000,
	1.703000,
	2.814000,
	1.061000,
	0.824000,
	2.258000,
	0.000000,
	0.000000,
	0.246000,
	0.371000,
	0.383070,
	0.565000,
	2.650000,
	0.000000,
	0.125000,
	-4.421000, 6.067000,
	-4.421000, 6.067000,
	-33.000000, 0.000000,
	-24.613926, 0.000000,
	-23.000000, 0.000000,
	-17.369511, 0.000000,
	-16.000000, 0.000000,
	-4.640372, 0.000000,
	-4.250000, 0.000000,
	-4.250000, 0.000000,
	-4.250000, 0.000000,
	-4.250000, 4.761719,
	-4.250000, 5.750000,
	-4.250000, 5.750000,
	-4.250000, 5.750000,
	2.337000, -6.633000,
	-4.250000, 1.758000,
	-2.109000, 1.758000,
	-1.611000, -3.644000,
	-4.848000, 1.613000,
	-1.611000, -3.644000,
	11.962000, 0.757000,
	-1.134000, 1.633000,
	-0.186000, -5.230000,
	2.735000, -4.793000,
	-1.611000, -3.644000,
	-4.421000, 6.067000,
	-4.421000, 6.067000,
	-5.541000, 6.067000,
	0.000000,
	0.000000,
	0.000000, 1.000000, 0.000000,
	1.000000,
	0.356863, 1.000000, 0.819608,
	0.356863, 1.000000, 0.819608,
	0.000107, 0.015175, 0.010398,
	0.356863, 1.000000, 0.819608,
	0.356863, 1.000000, 0.819608,
	0.000000, 0.000000, 0.000000,
	0.027451, 0.066667, 0.121569,
	0.027451, 0.066667, 0.121569,
	0.027451, 0.066667, 0.121569,
	0.764706, 0.309804, 0.223529,
	0.764706, 0.309804, 0.223529,
	0.000000, 0.708298, 0.163641,
	2.200000,
	0.000000, 1.000000, 1.000000,
	0.040000,
	0.000000, 0.000000, 0.000000,
	0.200000,
	0.200000,
	0.000000, 0.000000, 0.000000,
	0.000000, 1.000000, 1.000000,
	0.180000, 0.180000, 0.180000,
	1.000000, 0.000000, 1.000000,
	6.000000, 5.000000, 15.000000,
	0.400000,
	0.400000,
	0.180000, 0.180000, 0.180000,
	2.000000,
	1.000000,
	0.000000, 0.000000, 0.000000,
	1.000000, 1.000000, 0.000000,
	1.000000, 0.000000, 1.000000,
	0.215764, 1.000000, 0.000000,
	0.271577, 0.311181, 0.493616,
	0.000000, 1.000000, 1.000000,
	0.000000, 0.000000, 0.000000,
	6.000000, 1.000000, 15.000000,
	0.000000, 0.000000, 0.000000,
	0.400000,
	0.000000, 1.000000, 0.000000,
	0.400000,
	2.000000,
	1.000000,
	7.000000,
	10.842000,
	10.842000,
	16.658001,
	4.935000,
	10.842000,
	10.842000,
	16.658001,
	4.935000,
	10.842000,
	10.061000, 6.948000,
	0.156000,
	0.000000,
	0.248000,
	0.717000,
	2.000000,
	1.019000, 1.288000, 1.268000,
	1.000000, 0.065754, 0.566810,
	0.000000, 0.666117, 1.000000,
	0.016000,
	0.018234,
	0.121000,
	0.027000,
	0.018047,
	0.018234,
	0.121000,
	0.027000,
	0.018047,
	0.018234,
	0.121000,
	0.027000,
	0.018047,
	0.432000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	1.708000,
	0.039000,
	0.000000,
	0.500000,
	4.615000,
	0.310000,
	0.272000,
	1.000000,
	1.000000,
	1.000000,
	1.000000,
	1.000000,
	2.299000,
	4.800000,
	0.158166, 0.050513, -0.426009, -0.889308,
	-0.507292, -0.350279, -0.288931, -0.732388,
	0.534269, -0.575582, 0.581907, -0.211097,
	-0.421951, -0.592868, 0.293420, -0.619907,
	-0.026766, 0.895797, 0.134806, -0.422573,
	1.869000,
	2.079000,
	0.767000, 1.331000,
	1.224000,
	2.737000,
	0.673049, 0.227137, 0.020951,
	1.755000,
	1.968000,
	200.000000,
	1.000000,
	1.000000,
	1.000000,
	0.132000,
	0.941000,
	0.100000,
	1.500000,
	6.000000,
	2.000000,
	2.327000,
	4.487000,
	9.523000,
	1.209000,
	0.500000, 0.500000, 0.500000,
	0.500000, 0.500000, 0.500000,
	0.500000, 0.500000, 0.500000,
	1.000000,
	0.000000,
	1.000000,
	0.000000,
	0.000000,
	1.000000,
	1.000000,
	0.014311, 0.014311, 0.014311,
	0.263175, 0.560499, 0.476177,
	0.204710, 1.000000, 0.517401,
	0.000000,
	0.026000,
	0.026000,
	0.045000,
	0.045000,
	1.540000,
	0.005000,
	1.000000,
	0.000000,
	0.000000,
	0.000000,
	0.000000,
	0.500000,
	0.841000,
	-0.500000,
	-0.500000,
	0.889000,
	30.000000,
	1.219000,
	5.848000, 7.692000, 5.608000,
	500.000000,
	8.020000,
	0.843000,
	5.868000, 6.368000, 7.020000,
	-10.257000, 2.294000, -16.457001,
	1.820000,
	0.143000,
	0.500000,
	0.002000,
	50.000000,
	2.339000,
	6.284000,
	3.324000,
	0.000000,
	1.000000,
	0.349000,
};
Sequence sequence_data[108] = {
	{222000, 244000, 2, 4},
	{222000, 244000, 2, 3},
	{222000, 244000, 2, 3},
	{222000, 244000, 2, 3},
	{222000, 244000, 1, 2},
	{222000, 244000, 1, 2},
	{109500, 135000, 0, 1},
	{186000, 221000, 0, 5},
	{109500, 124000, 1, 2},
	{92375, 109500, 0, 1},
	{0, 28000, 0, 1},
	{28000, 32000, 1, 2},
	{32000, 53000, 1, 2},
	{53000, 84000, 0, 1},
	{244000, 288000, 1, 2},
	{222000, 244000, 0, 1},
	{88500, 92375, 0, 1},
	{84000, 88500, 1, 2},
	{168000, 181000, 0, 1},
	{186000, 221000, 0, 5},
	{109500, 124000, 1, 2},
	{92375, 109500, 0, 1},
	{0, 28000, 0, 1},
	{28000, 32000, 1, 2},
	{32000, 53000, 1, 2},
	{53000, 84000, 0, 1},
	{244000, 288000, 1, 2},
	{222000, 244000, 0, 1},
	{88500, 92375, 0, 1},
	{84000, 88500, 1, 2},
	{168000, 181000, 0, 1},
	{92000, 160000, 1, 2},
	{53000, 88500, 0, 3},
	{88500, 89250, 0, 1},
	{89250, 90000, 0, 1},
	{90000, 90750, 0, 1},
	{90750, 91500, 0, 1},
	{91500, 92375, 0, 0},
	{4000, 16000, 0, 1},
	{16000, 32000, 1, 5},
	{32000, 53000, 1, 1},
	{72250, 80250, 2, 17},
	{53000, 61000, 0, 1},
	{49000, 53000, 1, 1},
	{61000, 68250, 1, 2},
	{88500, 89250, 0, 1},
	{89250, 90000, 0, 1},
	{90000, 90750, 0, 1},
	{90750, 91500, 0, 1},
	{91500, 92375, 1, 2},
	{32000, 53000, 1, 1},
	{0, 4000, 1, 2},
	{4000, 16000, 0, 1},
	{16000, 32000, 1, 4},
	{32000, 53000, 2, 2},
	{53000, 61000, 1, 3},
	{72250, 80250, 1, 2},
	{61000, 72250, 0, 1},
	{80250, 88500, 0, 1},
	{88500, 89250, 1, 2},
	{89250, 90000, 0, 1},
	{90000, 90750, 1, 2},
	{90750, 91500, 1, 1},
	{61000, 88500, 1, 6},
	{89250, 90000, 1, 2},
	{0, 4000, 0, 1},
	{4000, 16000, 2, 7},
	{16000, 32000, 1, 5},
	{32000, 53000, 1, 3},
	{53000, 68250, 0, 1},
	{72250, 80250, 0, 1},
	{80250, 88500, 2, 3},
	{68250, 72250, 2, 4},
	{88500, 89250, 0, 1},
	{89250, 90000, 0, 1},
	{90000, 90750, 0, 1},
	{92375, 109500, 0, 1},
	{109500, 124000, 0, 1},
	{92375, 109500, 0, 1},
	{109500, 124000, 0, 1},
	{92375, 109500, 0, 1},
	{109500, 124000, 0, 1},
	{92375, 109500, 0, 1},
	{109500, 124000, 0, 1},
	{0, 26000, 0, 1},
	{192500, 195500, 2, 4},
	{203000, 206000, 2, 4},
	{186000, 221000, 2, 4},
	{198500, 208500, 1, 4},
	{208500, 221000, 1, 4},
	{186000, 198500, 1, 4},
	{186000, 221000, 0, 5},
	{186000, 220500, 1, 2},
	{267000, 288500, 2, 4},
	{186000, 221000, 0, 1},
	{109500, 124000, 0, 1},
	{92000, 125000, 0, 1},
	{186000, 221000, 0, 1},
	{92000, 125000, 0, 1},
	{92000, 125000, 0, 2},
	{186000, 221000, 0, 1},
	{92000, 125000, 0, 2},
	{109500, 132000, 0, 1},
	{53000, 92000, 0, 1},
	{0, 53000, 0, 1},
	{222000, 244000, 0, 1},
	{168000, 181000, 1, 2},
	{92375, 160000, 1, 2},
};
int sequence_index[164] = {
	0,
	1,
	1,
	2,
	2,
	2,
	3,
	3,
	3,
	4,
	4,
	4,
	4,
	4,
	5,
	5,
	6,
	6,
	6,
	6,
	6,
	6,
	6,
	7,
	7,
	19,
	31,
	31,
	31,
	31,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32,
	38,
	42,
	42,
	50,
	50,
	50,
	50,
	51,
	63,
	65,
	76,
	76,
	76,
	76,
	76,
	78,
	80,
	82,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	84,
	85,
	85,
	85,
	85,
	85,
	85,
	85,
	87,
	87,
	88,
	88,
	88,
	88,
	91,
	91,
	92,
	92,
	93,
	93,
	93,
	93,
	93,
	93,
	93,
	93,
	93,
	93,
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
	96,
	96,
	96,
	96,
	96,
	97,
	99,
	99,
	99,
	100,
	100,
	102,
	102,
	102,
	102,
	102,
	102,
	102,
	102,
	106,
	106,
	106,
	106,
	106,
	106,
	106,
	106,
	106,
	107,
	107,
	107,
	107,
	107,
	108,
	108,
};
int keyframe_index[164] = {
	0,
	15,
	16,
	28,
	29,
	30,
	42,
	43,
	44,
	56,
	57,
	58,
	61,
	62,
	65,
	68,
	71,
	72,
	75,
	78,
	79,
	80,
	81,
	83,
	84,
	150,
	238,
	239,
	240,
	241,
	244,
	246,
	248,
	249,
	250,
	251,
	253,
	256,
	259,
	262,
	270,
	295,
	296,
	307,
	308,
	309,
	310,
	312,
	335,
	344,
	402,
	403,
	404,
	407,
	408,
	417,
	426,
	435,
	444,
	445,
	448,
	449,
	452,
	453,
	454,
	457,
	460,
	463,
	466,
	469,
	470,
	471,
	474,
	475,
	476,
	479,
	482,
	485,
	488,
	491,
	494,
	497,
	503,
	504,
	507,
	508,
	509,
	510,
	511,
	520,
	522,
	527,
	530,
	533,
	536,
	549,
	550,
	556,
	557,
	560,
	561,
	562,
	563,
	564,
	565,
	566,
	567,
	568,
	569,
	570,
	590,
	591,
	592,
	594,
	595,
	596,
	599,
	600,
	601,
	602,
	603,
	604,
	605,
	606,
	607,
	608,
	609,
	610,
	611,
	614,
	615,
	618,
	621,
	624,
	626,
	629,
	630,
	631,
	640,
	641,
	645,
	646,
	647,
	648,
	649,
	650,
	651,
	652,
	657,
	658,
	659,
	662,
	663,
	664,
	665,
	668,
	671,
	674,
	675,
	676,
	677,
	678,
	681,
	682,
};
Uniform uniforms[163] = {
	{2, 117 },
	{0, 109 },
	{2, 105 },
	{0, 108 },
	{0, 107 },
	{2, 113 },
	{0, 106 },
	{0, 111 },
	{2, 116 },
	{0, 118 },
	{0, 120 },
	{4, 123 },
	{0, 104 },
	{0, 125 },
	{2, 124 },
	{0, 103 },
	{0, 115 },
	{4, 122 },
	{2, 110 },
	{0, 121 },
	{0, 114 },
	{0, 119 },
	{0, 112 },
	{0, 69 },
	{2, 67 },
	{5, 68 },
	{0, 70 },
	{0, 65 },
	{0, 64 },
	{0, 141 },
	{1, 84 },
	{1, 87 },
	{0, 86 },
	{0, 85 },
	{0, 83 },
	{1, 88 },
	{4, 200 },
	{4, 197 },
	{4, 206 },
	{0, 210 },
	{0, 205 },
	{0, 202 },
	{0, 198 },
	{6, 204 },
	{0, 208 },
	{0, 209 },
	{0, 199 },
	{0, 207 },
	{0, 203 },
	{1, 201 },
	{0, 127 },
	{0, 129 },
	{2, 128 },
	{0, -1 },
	{4, 132 },
	{4, 137 },
	{4, 133 },
	{4, 131 },
	{0, 135 },
	{4, 150 },
	{0, 163 },
	{4, 165 },
	{0, 147 },
	{0, 152 },
	{4, 156 },
	{4, 148 },
	{4, -1 },
	{4, 155 },
	{2, 158 },
	{0, 153 },
	{0, 157 },
	{4, -1 },
	{0, 149 },
	{0, -1 },
	{4, 146 },
	{4, 144 },
	{4, 143 },
	{4, 145 },
	{4, 166 },
	{4, 154 },
	{4, 159 },
	{2, 162 },
	{0, 167 },
	{2, 161 },
	{0, 164 },
	{0, 160 },
	{0, 151 },
	{0, 189 },
	{0, 190 },
	{1, 188 },
	{0, 185 },
	{2, 191 },
	{4, 186 },
	{4, -1 },
	{0, 179 },
	{0, 184 },
	{0, 187 },
	{0, 183 },
	{0, 192 },
	{0, 181 },
	{0, 182 },
	{0, 180 },
	{0, 196 },
	{0, 193 },
	{0, 194 },
	{0, 195 },
	{0, -1 },
	{0, 175 },
	{0, 172 },
	{5, 169 },
	{0, 171 },
	{0, 178 },
	{1, 177 },
	{0, 170 },
	{0, 174 },
	{4, 168 },
	{0, 173 },
	{0, 176 },
	{0, 130 },
	{0, 126 },
	{0, 211 },
	{0, 212 },
	{0, 74 },
	{0, 76 },
	{0, 77 },
	{0, 73 },
	{0, 72 },
	{0, 71 },
	{0, 79 },
	{0, 78 },
	{4, 98 },
	{4, 99 },
	{4, 100 },
	{6, 75 },
	{6, 80 },
	{6, 97 },
	{6, 102 },
	{4, 101 },
	{6, 82 },
	{0, 81 },
	{0, 95 },
	{0, 94 },
	{0, 96 },
	{0, 89 },
	{0, 90 },
	{0, 91 },
	{0, 93 },
	{0, 92 },
	{0, 213 },
	{0, 216 },
	{2, 215 },
	{0, 214 },
	{0, 217 },
	{0, 220 },
	{2, 221 },
	{2, 219 },
	{0, 218 },
	{0, 136 },
	{0, 139 },
	{0, 138 },
	{0, 134 },
	{0, 140 },
	{0, 142 },
};
int keyframe_time[224] = {
	0,
	8500,
	16000,
	22000,
	0,
	6500,
	16500,
	0,
	6000,
	16000,
	0,
	13000,
	22000,
	0,
	22000,
	0,
	22000,
	0,
	0,
	6500,
	9500,
	17000,
	20000,
	0,
	14500,
	0,
	0,
	0,
	4000,
	0,
	21000,
	0,
	0,
	44000,
	0,
	0,
	0,
	4500,
	0,
	0,
	6500,
	9500,
	17000,
	20000,
	0,
	14500,
	0,
	0,
	0,
	4000,
	0,
	21000,
	0,
	0,
	44000,
	0,
	0,
	0,
	4500,
	0,
	0,
	68000,
	0,
	17750,
	18500,
	0,
	0,
	0,
	0,
	0,
	0,
	8000,
	10000,
	12000,
	16000,
	0,
	0,
	500,
	1000,
	1500,
	2000,
	2500,
	3000,
	3500,
	4000,
	4500,
	5000,
	5500,
	6000,
	6500,
	7000,
	7500,
	8000,
	0,
	4000,
	0,
	7250,
	0,
	0,
	0,
	0,
	0,
	875,
	10500,
	0,
	4000,
	0,
	0,
	2000,
	4000,
	16000,
	0,
	21000,
	0,
	4000,
	8000,
	0,
	8000,
	0,
	0,
	0,
	750,
	0,
	0,
	750,
	0,
	0,
	7250,
	11250,
	19250,
	23000,
	27500,
	0,
	750,
	0,
	0,
	2000,
	4000,
	6000,
	8000,
	10250,
	12000,
	0,
	4000,
	6000,
	8000,
	16000,
	0,
	10000,
	21000,
	0,
	0,
	0,
	3000,
	8250,
	0,
	812,
	1562,
	4000,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1000,
	2000,
	3000,
	0,
	1000,
	2000,
	3000,
	0,
	15500,
	25000,
	35000,
	0,
	1000,
	3500,
	6000,
	0,
	1000,
	3500,
	6000,
	0,
	1000,
	3500,
	6000,
	0,
	6500,
	9500,
	17000,
	20000,
	0,
	34500,
	3000,
	8000,
	11500,
	16500,
	0,
	0,
	0,
	0,
	0,
	0,
	17500,
	0,
	0,
	17500,
	0,
	0,
	0,
	0,
	0,
	13000,
	0,
	35625,
};
bool keyframe_ease_toward[224] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
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
	true,
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
	true,
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
	true,
	false,
	true,
	true,
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
};
bool keyframe_ease_away[224] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
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
	true,
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
};
int keyframe_time_index[164] = {
	0,
	4,
	4,
	7,
	7,
	7,
	10,
	10,
	10,
	13,
	13,
	13,
	13,
	13,
	15,
	15,
	17,
	17,
	17,
	17,
	17,
	17,
	17,
	18,
	18,
	39,
	60,
	60,
	60,
	60,
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
	69,
	93,
	93,
	103,
	103,
	103,
	103,
	104,
	126,
	134,
	162,
	162,
	162,
	162,
	162,
	164,
	166,
	168,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	170,
	171,
	171,
	171,
	171,
	171,
	171,
	171,
	179,
	179,
	183,
	183,
	183,
	183,
	195,
	195,
	200,
	200,
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
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	206,
	208,
	208,
	208,
	208,
	208,
	209,
	211,
	211,
	211,
	213,
	213,
	216,
	216,
	216,
	216,
	216,
	216,
	216,
	216,
	220,
	220,
	220,
	220,
	220,
	220,
	220,
	220,
	220,
	222,
	222,
	222,
	222,
	222,
	224,
	224,
};
#endif