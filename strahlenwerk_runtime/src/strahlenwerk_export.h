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
#include "shaders/glumanda.h"
#include "shaders/glumanda_environment.h"
#include "shaders/karpador.h"
#include "shaders/karpador_environment.h"
#include "shaders/kleinstein.h"
#include "shaders/kleinstein_environment.h"
#include "shaders/relaxo.h"
#include "shaders/relaxo_environment.h"
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
Shader scenes[4] = {
	Shader(glumanda_source, 0, nullptr),
	Shader(karpador_source, 0, nullptr),
	Shader(kleinstein_source, 0, nullptr),
	Shader(relaxo_source, 0, nullptr),
};
AmbientLight environments[4] = {
	AmbientLight(glumanda_environment_source),
	AmbientLight(karpador_environment_source),
	AmbientLight(kleinstein_environment_source),
	AmbientLight(relaxo_environment_source),
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
Scene scenes_data[7] = {
	{0, 87500, 0},
	{87500, 166500, 3},
	{166500, 260500, 1},
	{260500, 332000, 3},
	{332000, 461250, 2},
	{460000, 509000, 3},
	{509000, 558000, 2},
};
float keyframe_data[1489] = {
	-8.000000,
	-8.000000,
	-9.150000,
	-15.421000,
	-8.000000,
	-8.000000,
	-10.317000,
	-8.000000,
	-8.000000,
	-10.220000,
	-8.000000,
	-8.000000,
	-14.000000,
	-9.551000,
	-7.881000,
	-8.000000,
	-100.000000,
	-100.000000,
	-100.000000,
	-8.000000,
	-8.000000,
	-9.000000,
	-12.000000,
	-100.000000,
	-8.000000,
	-1000.000000,
	-8.000000,
	-1000.000000,
	-1000.000000,
	-10.000000,
	-1000.000000,
	-1000.000000,
	-10.000000,
	-1000.000000,
	-1000.000000,
	-8.000000,
	-1000.000000,
	-1000.000000,
	-10.000000,
	-9.190000,
	-50.000000,
	-1000.000000,
	-1000.000000,
	-1000.000000,
	-1000.000000,
	-8.000000,
	-8.000000,
	-1000.000000,
	-10.000000,
	-9.000000,
	-1000.000000,
	-11.000000,
	-1000.000000,
	-1000.000000,
	-1000.000000,
	-1000.000000,
	-1000.000000,
	-8.000000,
	-1000.000000,
	-1000.000000,
	-8.000000,
	-1000.000000,
	-13.000000,
	-8.000000,
	-100.000000,
	-100.000000,
	0.022000,
	0.018000,
	0.022000,
	0.028000,
	0.028000,
	0.055000,
	0.055000,
	0.022000,
	0.022000,
	0.017000,
	0.017000,
	0.018000,
	0.018000,
	0.100000,
	0.100000,
	0.050000,
	0.050000,
	0.016000,
	0.016000,
	0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000,
	1.305245, 1.978355, -11.944466,
	11.300220, 9.738379, -14.460633,
	30.191198, 27.419125, -3.576252,
	36.448128, 8.260777, 24.157364,
	35.827831, 1.350101, 35.833851,
	35.800907, 1.350101, 35.806797,
	35.827831, 1.350101, 35.833851,
	26.523096, 0.888802, 26.481415,
	23.465176, 0.888802, 23.407818,
	20.753733, 0.888802, 20.682470,
	15.081762, 0.142932, 14.981429,
	0.217011, 0.911540, 0.111148,
	0.000000, 0.000000, 0.000000,
	0.217011, 0.911540, 0.111148,
	8.654902, 9.397054, 0.157018,
	14.699642, 12.250875, 0.114508,
	14.699642, 12.250875, 0.114508,
	10.254007, 6.482441, -4.590658,
	-2.769438, 1.906032, -12.422028,
	13.568065, 9.695831, -13.272884,
	2.500119, 5.294064, 21.539137,
	-5.621164, 5.474891, 12.470597,
	-20.614248, 0.765397, -20.712463,
	-18.559361, 1.059392, -18.657797,
	-46.693573, 2.150512, -46.788864,
	-60.874039, 15.387908, -60.905758,
	3.016877, 0.109760, 2.953009,
	2.465696, 0.109760, 2.416971,
	-1.983744, 1.140772, 7.722066,
	2.198637, 1.140772, 7.748411,
	0.290832, 1.140772, 19.169617,
	0.123714, 1.683411, 8.154359,
	-0.287187, 1.619588, 17.341526,
	-0.185659, 1.619588, 26.840858,
	-0.182283, 1.590141, 26.352842,
	0.402124, 15.654578, 10.589061,
	0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000,
	90.234131, 0.684455, -13.114203,
	85.134666, 0.684455, -3.459900,
	82.235085, 0.684455, -1.132030,
	63.761677, 0.533719, -0.736013,
	45.199787, 0.530909, 1.280457,
	16.617224, 7.417778, 0.878711,
	-5.586633, 32.155418, -0.413691,
	-4.132099, 21.392540, -0.546460,
	-27.295429, 15.220130, -17.539955,
	-37.257710, 15.220130, -17.049755,
	-39.686520, 14.746255, -17.585211,
	-42.981560, 15.220130, -34.773422,
	-23.903355, 50.091381, -64.178711,
	-13.196825, 27.654997, -35.432484,
	48.743961, 2.701604, -12.931068,
	51.661629, 1.089494, 5.152371,
	2.479893, 11.994681, 1.152166,
	-36.986134, 13.741493, 0.959067,
	74.576675, -6.762972, -21.493393,
	77.752686, 4.343738, -20.848793,
	52.122871, 8.759866, -44.802353,
	68.800430, 3.180978, -40.137142,
	55.268211, 4.017811, -48.025761,
	55.268211, 4.017811, -48.025761,
	55.268211, 4.017811, -48.025761,
	46.795769, 4.407757, -53.934837,
	58.728558, -0.557718, -50.102051,
	57.123974, 0.870937, -50.645004,
	54.360748, 0.870937, -51.321320,
	49.957985, 0.870937, -52.020481,
	34.417664, 0.870937, -61.358395,
	8.771114, 0.870937, -76.708603,
	-9.758824, 0.870937, -87.358429,
	0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000,
	-25.306871, -1.235555, -54.160950,
	60.292850, 3.299488, -52.178619,
	13.095589, 2.147914, -81.543007,
	3.159900, 1.363209, -77.845093,
	-6.282967, 1.531379, -71.796982,
	-10.236229, 0.625814, -71.291687,
	-14.200358, 1.016179, -72.752853,
	-44.265373, 1.106239, -53.460537,
	-60.906010, 0.400804, -34.780258,
	0.000000, 0.000000, 0.000000,
	-46.230827, 2.283110, -48.387623,
	0.000000, 0.000000, 0.000000,
	-67.546349, 2.283110, -10.431232,
	-0.073236, -0.119990, -1.408562,
	0.805620, -0.119990, -40.407593,
	0.000000, 25.315304, -0.581125,
	0.586347, 12.386934, -12.214477,
	-23.684830, 4.492705, -1.145454,
	-15.799189, 4.492705, 18.599277,
	15.246177, 1.414325, 22.128258,
	22.097406, 5.872145, 8.762771,
	11.361899, 3.647986, -24.407475,
	-16.036749, 2.239836, -18.663345,
	-14.572295, 10.017987, -9.321136,
	-16.802784, 9.980742, -11.151800,
	-26.663837, 1.245988, -1.191073,
	-22.433962, 1.060226, 10.856906,
	-13.019633, 1.060226, -1.274388,
	-16.431744, 1.060226, -17.902641,
	0.000000, 0.000000, 0.000000,
	12.644032, -4.877594, 9.327963,
	19.838980, 5.187898, 5.740065,
	-2.348039, 5.615546, 17.496889,
	-22.963043, 6.497660, 0.581250,
	-11.218121, 5.141048, -17.010553,
	16.822069, 5.141048, -5.096961,
	22.023632, 6.387924, 8.883282,
	25.901632, 6.387924, -15.460366,
	27.400606, -1.603850, -16.522230,
	-23.899893, -0.681706, -17.202068,
	6.353562, 1.503709, -11.785218,
	18.915588, 1.503709, -15.485925,
	0.000000, 0.000000, 0.000000,
	-11.612030, -1.186265, 20.694405,
	6.394844, -2.003315, 1.797041,
	10.350377, -5.413310, -14.670207,
	13.117639, -4.948184, -12.357553,
	5.225583, -1.324930, -16.234470,
	2.073873, -2.277206, -8.507305,
	-2.360751, -1.083034, -1.099878,
	-0.647453, 1.747276, -10.652020,
	26.595943, -3.624450, -21.493473,
	16.265188, 0.731979, -18.238840,
	5.868662, -5.596756, 16.125820,
	5.489725, -5.596756, 16.014881,
	-22.364208, 1.603794, 13.766211,
	-22.266285, 1.770299, 13.955221,
	0.000000, 0.139545, 0.000000,
	0.000000, 2.328305, 0.000000,
	-7.407756, -4.684971, 10.848302,
	-6.761472, -4.684971, 12.255128,
	-17.979746, -5.069039, -8.161098,
	-5.963357, 7.290941, -4.262176,
	0.052654, -2.390893, 0.018157,
	0.052654, -2.390893, 0.018157,
	-6.114767, 4.468062, -3.347566,
	-17.212440, -1.400614, -10.540871,
	-17.212440, -1.400614, -10.540871,
	32.044849, -0.318647, -16.001024,
	27.651533, -0.318647, -19.754480,
	-25.606209, -4.825075, -6.341953,
	-24.852087, -4.825075, -5.510608,
	-24.852087, -4.825075, -5.510608,
	-24.022234, -4.825075, -4.595771,
	-24.022234, -4.825075, -4.595771,
	-23.881096, -4.825075, -4.440178,
	-23.881096, -4.825075, -4.440178,
	-23.715034, -4.825075, -4.257110,
	-23.715034, -4.825075, -4.257110,
	-23.715034, -4.825075, -4.257110,
	-12.256635, -4.853430, 4.069864,
	-5.710466, -4.879760, 7.289046,
	1.156009, -4.879760, 12.837969,
	-4.725987, -4.879760, 16.015518,
	0.000000, 0.000000, 0.000000,
	176.489349, 3.509836, 33.043198,
	76.200401, 2.454874, 20.719803,
	70.255142, 2.392334, 19.989258,
	69.467209, 2.384046, 19.892437,
	32.098091, 3.410171, -15.781819,
	32.464172, 3.430311, 59.598240,
	32.464172, 3.430311, 59.598240,
	5.392800, 3.445886, 48.887310,
	32.771675, 0.215106, 39.669849,
	59.355785, 0.545166, -8.230579,
	59.355785, 0.545166, -8.230579,
	27.974531, 1.519826, -3.476889,
	-4.296309, 9.385345, -0.744112,
	-12.740602, 18.216621, -0.576628,
	7.711141, 31.357098, 0.379639,
	159.383987, 15.305342, -15.645590,
	10.265858, 15.305342, -20.289528,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.214575, -0.624101, 0.183834, 0.728464,
	0.139792, -0.778251, 0.186589, 0.583068,
	0.004332, -0.971981, -0.143206, 0.186350,
	0.094306, -0.904920, -0.312444, -0.273136,
	-0.041430, -0.381119, 0.017099, -0.923438,
	-0.040221, -0.381141, 0.016600, -0.923491,
	-0.041430, -0.381119, 0.017099, -0.923438,
	-0.041430, -0.381119, 0.017099, -0.923438,
	-0.041430, -0.381119, 0.017099, -0.923438,
	-0.204590, -0.372041, 0.084438, -0.901442,
	-0.316677, -0.358416, 0.130698, -0.868429,
	0.489141, 0.508986, -0.491128, 0.510360,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.489141, 0.508986, -0.491128, 0.510360,
	-0.367885, 0.605213, 0.366047, 0.603644,
	-0.367885, 0.605213, 0.366047, 0.603644,
	-0.367885, 0.605213, 0.366047, 0.603644,
	-0.124928, 0.715169, 0.129799, 0.675336,
	-0.075122, 0.919752, -0.324403, -0.207787,
	-0.296683, 0.608920, -0.260071, -0.688156,
	-0.041597, 0.045079, -0.031031, -0.997635,
	-0.033954, -0.120717, -0.006269, -0.992086,
	-0.055326, -0.381555, 0.011492, -0.922618,
	-0.055326, -0.381555, 0.011492, -0.922618,
	-0.055326, -0.381555, 0.011492, -0.922618,
	-0.397300, -0.349396, 0.153749, -0.834529,
	0.008041, 0.920996, 0.019037, -0.389025,
	0.008041, 0.920996, 0.019037, -0.389025,
	0.500792, 0.241821, -0.148082, 0.817803,
	0.496341, -0.265164, 0.162377, 0.810535,
	0.302900, 0.013752, -0.004371, 0.952913,
	0.174182, 0.005837, -0.002144, 0.984694,
	0.327685, 0.005202, -0.002960, 0.944768,
	0.136722, 0.005680, -0.001886, 0.990591,
	0.134252, 0.005577, -0.001852, 0.990930,
	-0.241999, 0.082683, 0.019972, 0.966541,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.018361, 0.979895, -0.107825, 0.166857,
	0.066059, 0.910274, -0.159867, 0.376137,
	0.082927, 0.910667, -0.213668, 0.343737,
	0.074356, 0.816546, -0.191585, 0.539463,
	-0.281292, 0.667825, -0.491565, 0.482958,
	-0.705687, 0.164685, -0.679610, -0.114081,
	0.149699, -0.579535, -0.190968, -0.777985,
	0.123868, -0.479536, -0.158016, -0.854244,
	-0.234005, -0.710448, -0.322524, -0.580072,
	0.112755, -0.178712, -0.424124, -0.880606,
	0.074717, -0.067488, -0.391890, -0.914486,
	-0.381434, -0.250290, -0.309010, -0.834492,
	0.038778, -0.998488, 0.021363, 0.032595,
	0.029129, -0.750031, 0.016047, 0.660566,
	0.499886, -0.847893, 0.029101, 0.174196,
	0.317979, -0.634799, 0.606687, -0.357562,
	-0.031959, -0.691332, 0.022638, -0.721476,
	-0.002474, -0.701102, -0.005647, -0.713036,
	-0.050884, 0.331366, 0.017900, 0.941959,
	-0.079170, 0.330679, 0.027851, 0.940005,
	0.056455, -0.385225, -0.023619, -0.920792,
	-0.046962, 0.491214, 0.026535, 0.869368,
	0.048431, -0.475718, -0.026120, -0.877875,
	0.048431, -0.475718, -0.026120, -0.877875,
	0.048431, -0.475718, -0.026120, -0.877875,
	-0.021373, 0.640163, 0.017821, 0.767735,
	0.108022, 0.577699, -0.077487, 0.805352,
	-0.022207, 0.582655, 0.015930, 0.812261,
	-0.022207, 0.582655, 0.015930, 0.812261,
	-0.023661, 0.500260, 0.013677, 0.865445,
	0.038922, 0.499941, -0.022498, 0.864893,
	0.003509, 0.500443, -0.002029, 0.865761,
	0.004915, 0.500439, -0.002841, 0.865754,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.058851, 0.792324, -0.077419, 0.602300,
	-0.039032, 0.562058, 0.026567, 0.825750,
	0.010244, 0.821100, -0.013032, 0.570545,
	0.038372, 0.868500, -0.066051, 0.489769,
	-0.043541, 0.774966, 0.055278, 0.628075,
	0.056881, 0.423266, -0.025556, 0.903858,
	-0.007123, 0.894137, 0.016415, 0.447438,
	0.283435, 0.841927, 0.114195, 0.444730,
	0.063075, 0.935496, -0.145814, 0.315608,
	0.000000, 0.000000, 0.000000, 1.000000,
	-0.044513, 0.851712, 0.075268, 0.516665,
	0.000000, 0.000000, 0.000000, 1.000000,
	-0.044513, 0.851712, 0.075268, 0.516665,
	0.012048, -0.011265, 0.000136, 0.999864,
	0.012048, -0.011265, 0.000136, 0.999864,
	-0.721550, 0.000000, 0.000000, 0.692362,
	0.021350, 0.816089, -0.030210, 0.576741,
	-0.087035, 0.709097, -0.088923, -0.694046,
	-0.124085, 0.400223, -0.054794, -0.906324,
	-0.157209, -0.275950, 0.045804, -0.947121,
	-0.032363, -0.544450, 0.021029, -0.837904,
	-0.038259, -0.961740, 0.191573, -0.192070,
	0.072843, -0.904569, 0.171924, 0.383261,
	-0.030050, -0.895525, -0.063034, 0.439498,
	0.128857, -0.704506, 0.131284, 0.685443,
	0.077590, -0.240147, 0.022361, 0.967371,
	0.067373, 0.580035, -0.044511, 0.810579,
	0.073047, 0.422724, -0.030883, 0.902781,
	0.000000, 0.000000, 0.000000, 1.000000,
	0.205281, 0.432240, -0.112425, 0.870856,
	-0.212166, 0.581160, 0.082072, 0.781347,
	-0.261988, -0.081978, -0.060641, 0.959670,
	-0.222615, -0.633727, -0.025691, 0.740390,
	0.068170, -0.937714, -0.256929, 0.223684,
	0.203126, -0.756555, -0.223460, -0.580028,
	0.223871, -0.447604, -0.088411, -0.861231,
	-0.088139, 0.860922, 0.152123, 0.477395,
	-0.039386, 0.874825, 0.072172, 0.477411,
	0.173395, -0.824811, -0.090736, 0.530461,
	-0.149965, 0.015502, -0.331807, 0.931222,
	0.212756, -0.920665, -0.072338, -0.319182,
	0.000000, 0.000000, 0.000000, 1.000000,
	-0.046327, -0.556682, -0.031115, 0.828849,
	-0.025244, -0.890452, -0.049771, 0.451642,
	0.026188, 0.757822, -0.030474, 0.651224,
	0.026188, 0.757822, -0.030474, 0.651224,
	0.000484, 0.998365, -0.008546, 0.056534,
	-0.017435, 0.978265, 0.085905, 0.187922,
	-0.131633, -0.488521, -0.074834, 0.859314,
	-0.114506, -0.603408, -0.088103, 0.784236,
	-0.002653, -0.962041, 0.008801, -0.272752,
	-0.024608, -0.969381, -0.108205, 0.219058,
	-0.562117, 0.420157, 0.401212, 0.588660,
	-0.562117, 0.420157, 0.401212, 0.588660,
	-0.022354, -0.620953, -0.017720, 0.783329,
	-0.022354, -0.620953, -0.017720, 0.783329,
	-0.656877, -0.253771, -0.250986, 0.664168,
	-0.656877, -0.253771, -0.250986, 0.664168,
	0.311362, -0.501430, 0.199608, 0.782162,
	0.359439, -0.218905, 0.087141, 0.902935,
	-0.011628, 0.840089, -0.018022, -0.542023,
	-0.165451, 0.712495, 0.657930, 0.179173,
	-0.106245, 0.506464, 0.063056, 0.853363,
	-0.106245, 0.506464, 0.063056, 0.853363,
	-0.169104, 0.482092, 0.095424, 0.854332,
	-0.169104, 0.482092, 0.095424, 0.854332,
	-0.169104, 0.482092, 0.095424, 0.854332,
	-0.078914, -0.928784, 0.077909, 0.353639,
	-0.080046, -0.928688, 0.078340, 0.353543,
	-0.041701, 0.800592, 0.056099, 0.595118,
	-0.036670, 0.849258, 0.059509, 0.523330,
	0.113749, -0.925164, 0.003530, 0.362102,
	0.113749, -0.925164, 0.003530, 0.362102,
	-0.128960, -0.923167, 0.096895, 0.348915,
	-0.128960, -0.923167, 0.096895, 0.348915,
	0.120700, -0.924283, 0.000808, 0.362118,
	0.120700, -0.924283, 0.000808, 0.362118,
	-0.110269, -0.925586, 0.089821, 0.350803,
	-0.110269, -0.925586, 0.089821, 0.350803,
	-0.050513, -0.947790, 0.075898, 0.305584,
	-0.002071, -0.997725, 0.036497, -0.056628,
	-0.027681, -0.652709, 0.023876, -0.756725,
	-0.035964, -0.179106, 0.006552, -0.983150,
	0.000000, 0.000000, 0.000000, 1.000000,
	-0.007750, 0.325174, 0.002665, 0.945620,
	-0.007777, 0.315526, 0.002586, 0.948883,
	-0.046744, 0.110714, 0.005213, 0.992739,
	-0.038718, 0.063464, 0.002486, 0.997230,
	-0.038718, 0.063464, 0.002486, 0.997230,
	-0.027319, -0.543616, -0.017681, 0.838704,
	-0.090818, -0.551011, 0.060485, -0.827337,
	-0.090818, -0.551011, 0.060485, -0.827337,
	-0.090818, -0.551011, 0.060485, -0.827337,
	-0.031247, -0.713372, 0.031873, -0.699368,
	-0.039796, -0.712929, 0.040593, -0.698933,
	0.051960, -0.712114, -0.053001, -0.698134,
	0.131535, -0.700848, -0.133958, -0.688167,
	0.045254, -0.820610, -0.065622, -0.565909,
	0.045254, -0.820610, -0.065622, -0.565909,
	0.000000,
	0.000000,
	0.000000,
	0.003000,
	0.003000,
	0.429000,
	0.429000,
	0.429000,
	0.005000,
	0.005000,
	0.015000,
	0.015000,
	0.047000,
	0.000000,
	0.032000,
	0.000000,
	0.000000,
	0.010000,
	0.010000,
	0.010000,
	0.010000,
	0.010000,
	0.020000,
	0.000000,
	0.235000,
	2.181000,
	0.533000,
	0.018000,
	1.000000,
	0.000000,
	20.750000,
	0.000000,
	0.300000,
	128.300003,
	0.000000,
	0.000000,
	24.000000,
	200.000000,
	500.000000,
	500.000000,
	500.000000,
	500.000000,
	500.000000,
	500.000000,
	0.493616, 0.493616, 0.493616,
	0.498039, 0.498039, 0.498039,
	0.498039, 0.498039, 0.498039,
	0.000000,
	0.000000,
	6.997000,
	0.000000,
	0.000000,
	1.681000,
	1.611000,
	0.000000,
	0.000000,
	0.000000,
	2.946000,
	2.946000,
	0.000000,
	3.828000,
	0.000000,
	0.000000,
	0.000000,
	0.000000,
	0.000000,
	0.000000,
	0.000000,
	1.498000,
	0.000000,
	2.837000,
	0.000000,
	0.000000,
	4.717000,
	0.000000,
	0.000000,
	-5.000000,
	0.000000,
	0.000000,
	20.915001,
	1.455000,
	0.970000,
	1.970000,
	0.281429,
	1.654000,
	34.959000,
	0.000000,
	34.806999,
	0.000000,
	6.993000,
	0.000000,
	0.000000,
	0.000000,
	-0.060000,
	-0.005546,
	-0.084000,
	-0.050051,
	-0.026392,
	0.488000,
	-0.005301,
	-0.142000,
	0.000000,
	0.000000,
	-0.032000,
	0.000000,
	0.120000,
	0.000000,
	-0.044000,
	0.000000,
	0.000000,
	0.000000,
	-0.435000,
	-0.217500,
	0.000000,
	0.000000,
	0.000000,
	-0.137000,
	-0.123000,
	0.000000,
	-0.117000,
	-0.001000,
	0.000000,
	-0.106000,
	0.117000,
	-0.130000,
	0.000000,
	0.000000,
	-0.144000,
	0.000000,
	0.000000,
	0.000000,
	-1.265000,
	0.000000,
	-0.481000,
	0.000000,
	0.000000,
	0.088000,
	-0.053000,
	0.000000,
	0.000000,
	6.949000,
	0.000000,
	0.000000,
	-0.184000,
	-0.058105,
	-0.069000,
	-0.020032,
	0.000000,
	0.000000,
	0.000000,
	0.000000,
	0.100000,
	0.000000,
	0.000000,
	0.000000,
	-0.052000,
	0.000000,
	0.000000,
	0.000000,
	-0.616000,
	0.000000,
	0.159000,
	0.159000,
	0.159000,
	0.159000,
	0.159000,
	0.159000,
	0.159000,
	0.159000,
	-1.764000,
	0.159000,
};
Sequence sequence_data[138] = {
	{83000, 88250, 1, 4},
	{256000, 262000, 1, 3},
	{123000, 131000, 1, 3},
	{331250, 334500, 1, 5},
	{387375, 387625, 1, 2},
	{423000, 423625, 0, 2},
	{454000, 461250, 1, 5},
	{14062, 14468, 0, 2},
	{16625, 17453, 0, 9},
	{19906, 20906, 1, 10},
	{21437, 22562, 0, 12},
	{22750, 22937, 0, 2},
	{527250, 531000, 1, 4},
	{164000, 166500, 1, 2},
	{332000, 454000, 0, 4},
	{0, 71250, 0, 2},
	{72000, 83000, 0, 2},
	{233500, 234500, 0, 2},
	{246500, 260500, 0, 2},
	{87500, 127000, 0, 2},
	{127000, 166500, 0, 2},
	{300500, 332000, 0, 2},
	{332375, 387500, 2, 12},
	{387500, 391000, 0, 2},
	{391000, 401500, 1, 3},
	{401500, 423000, 2, 10},
	{423000, 427500, 1, 2},
	{427500, 460000, 1, 9},
	{460000, 509000, 2, 19},
	{0, 19000, 1, 6},
	{19000, 31125, 1, 10},
	{31125, 43000, 1, 2},
	{43000, 72000, 2, 9},
	{72000, 83000, 1, 3},
	{83000, 87500, 1, 2},
	{509000, 555000, 2, 15},
	{166500, 199000, 1, 13},
	{198812, 215812, 1, 2},
	{215750, 219750, 1, 2},
	{219750, 224375, 1, 2},
	{224375, 227625, 1, 2},
	{227625, 230500, 1, 2},
	{230500, 231500, 1, 2},
	{231500, 232500, 1, 2},
	{232500, 233500, 1, 2},
	{233500, 234500, 1, 2},
	{234500, 238000, 2, 3},
	{238000, 242500, 2, 4},
	{242500, 246500, 1, 2},
	{246500, 247500, 1, 2},
	{247500, 248500, 1, 2},
	{248500, 249000, 1, 2},
	{249000, 249750, 1, 2},
	{249750, 260500, 1, 6},
	{87375, 127000, 2, 5},
	{127000, 146813, 1, 2},
	{146750, 166438, 1, 2},
	{260500, 278500, 1, 2},
	{278500, 300500, 2, 5},
	{300500, 332000, 1, 2},
	{332375, 387500, 2, 12},
	{387500, 391000, 0, 2},
	{391000, 401500, 1, 3},
	{401500, 423000, 2, 10},
	{423000, 427500, 1, 2},
	{427500, 460000, 1, 9},
	{460000, 509000, 2, 19},
	{0, 19000, 1, 6},
	{19000, 31125, 1, 10},
	{31125, 43625, 1, 2},
	{43000, 72000, 2, 9},
	{72000, 83000, 1, 3},
	{83000, 87500, 1, 2},
	{509000, 555000, 2, 14},
	{166500, 199000, 1, 13},
	{198812, 215812, 1, 2},
	{215750, 219750, 1, 2},
	{219750, 224375, 1, 2},
	{224375, 227625, 1, 2},
	{227625, 230500, 1, 2},
	{231500, 232500, 1, 2},
	{230500, 231500, 1, 2},
	{232500, 233500, 1, 2},
	{233500, 234500, 1, 2},
	{234500, 238000, 2, 3},
	{238000, 242500, 2, 4},
	{246500, 247500, 1, 2},
	{242500, 246500, 1, 2},
	{247500, 248500, 1, 2},
	{248500, 249000, 1, 2},
	{249000, 249750, 1, 2},
	{249750, 260500, 1, 6},
	{87375, 127000, 1, 3},
	{127000, 146813, 1, 2},
	{146750, 166438, 1, 2},
	{260500, 278500, 1, 2},
	{278500, 300500, 2, 5},
	{300500, 332000, 1, 2},
	{332500, 387500, 0, 7},
	{413500, 423000, 0, 2},
	{423000, 438875, 0, 2},
	{6000, 83000, 2, 5},
	{230500, 234500, 0, 2},
	{240875, 242125, 0, 2},
	{242500, 246500, 1, 2},
	{158500, 166000, 1, 5},
	{250, 87500, 1, 2},
	{332000, 460000, 1, 2},
	{166500, 262500, 1, 2},
	{460000, 473000, 0, 2},
	{87500, 166500, 0, 2},
	{260500, 332000, 0, 2},
	{332000, 460000, 0, 2},
	{385250, 387750, 1, 3},
	{440000, 451000, 2, 4},
	{458000, 462500, 1, 2},
	{14375, 23000, 0, 2},
	{43000, 47500, 1, 6},
	{73500, 80000, 1, 7},
	{194000, 198500, 1, 3},
	{233000, 236500, 1, 3},
	{509000, 529000, 1, 10},
	{161000, 165000, 2, 3},
	{401250, 425500, 2, 10},
	{44000, 54000, 2, 7},
	{67250, 75750, 1, 6},
	{175000, 187500, 1, 6},
	{199500, 223000, 1, 5},
	{248000, 261000, 1, 4},
	{484000, 493000, 1, 5},
	{103500, 128000, 1, 4},
	{312000, 316000, 1, 3},
	{327000, 381000, 1, 6},
	{33000, 41500, 1, 5},
	{58500, 62000, 1, 4},
	{136000, 150500, 1, 3},
	{439500, 453500, 1, 6},
	{485000, 491500, 1, 3},
};
int sequence_index[15] = {
	0,
	14,
	22,
	60,
	98,
	106,
	107,
	108,
	109,
	112,
	113,
	123,
	133,
	136,
	138,
};
int keyframe_index[15] = {
	0,
	66,
	85,
	616,
	1312,
	1340,
	1343,
	1346,
	1349,
	1356,
	1365,
	1409,
	1466,
	1479,
	1489,
};
Uniform uniforms[14] = {
	{0, 75 },
	{0, 68 },
	{2, 66 },
	{5, 67 },
	{0, 64 },
	{0, 77 },
	{0, 79 },
	{0, 78 },
	{0, 76 },
	{4, 73 },
	{0, 69 },
	{0, 70 },
	{0, 71 },
	{0, 72 },
};
int keyframe_time[593] = {
	0,
	1000,
	4250,
	5250,
	0,
	4000,
	6000,
	0,
	4000,
	8000,
	0,
	625,
	2250,
	2500,
	3250,
	31,
	218,
	0,
	625,
	0,
	250,
	5750,
	6750,
	7250,
	0,
	406,
	0,
	171,
	203,
	234,
	328,
	359,
	421,
	500,
	828,
	0,
	312,
	375,
	437,
	500,
	594,
	687,
	781,
	969,
	1000,
	0,
	63,
	203,
	250,
	297,
	375,
	453,
	625,
	813,
	1000,
	1063,
	1125,
	0,
	187,
	0,
	125,
	3500,
	3750,
	0,
	2500,
	0,
	119625,
	120500,
	122000,
	0,
	71250,
	-22000,
	11000,
	0,
	1000,
	0,
	14000,
	0,
	39500,
	0,
	39500,
	0,
	31500,
	0,
	125,
	4762,
	13625,
	27875,
	33125,
	33140,
	33156,
	45121,
	47537,
	49627,
	55125,
	0,
	3500,
	0,
	5000,
	10500,
	0,
	2687,
	6500,
	12000,
	14250,
	15250,
	17750,
	19375,
	20500,
	21500,
	0,
	4500,
	0,
	4500,
	7000,
	8625,
	10875,
	21000,
	25750,
	29500,
	32500,
	0,
	1000,
	6000,
	8000,
	12910,
	16000,
	18000,
	20000,
	21000,
	23000,
	25000,
	26000,
	27000,
	30000,
	34992,
	40000,
	42000,
	46000,
	49000,
	0,
	3375,
	14500,
	17000,
	18968,
	19000,
	0,
	3750,
	3875,
	5625,
	6875,
	8250,
	8937,
	11125,
	11375,
	12125,
	0,
	11875,
	0,
	7000,
	10750,
	16000,
	20250,
	22500,
	25750,
	28750,
	29000,
	-5500,
	0,
	11000,
	0,
	4500,
	0,
	2250,
	4000,
	5500,
	6250,
	7250,
	8250,
	9250,
	10250,
	10250,
	11250,
	12250,
	13250,
	14250,
	46000,
	0,
	1000,
	2000,
	3000,
	4000,
	5000,
	6000,
	7000,
	8000,
	9000,
	16000,
	16500,
	32500,
	0,
	17000,
	0,
	4000,
	0,
	4625,
	0,
	3250,
	0,
	2875,
	0,
	1000,
	0,
	1000,
	0,
	1000,
	0,
	1000,
	0,
	1625,
	3500,
	0,
	2875,
	4125,
	4500,
	0,
	4000,
	0,
	1000,
	0,
	1000,
	0,
	500,
	0,
	750,
	0,
	500,
	2125,
	3375,
	5875,
	10750,
	0,
	125,
	30125,
	36625,
	39625,
	0,
	19813,
	0,
	19688,
	0,
	18000,
	0,
	8500,
	15096,
	17750,
	22000,
	0,
	31500,
	0,
	125,
	4762,
	13625,
	27875,
	33125,
	33140,
	33156,
	45121,
	47537,
	49627,
	55125,
	0,
	3500,
	0,
	5000,
	10500,
	0,
	2687,
	6500,
	12000,
	14250,
	15250,
	17750,
	19375,
	20500,
	21500,
	0,
	4500,
	0,
	4500,
	7000,
	8625,
	10875,
	21000,
	25750,
	29500,
	32500,
	0,
	1000,
	6000,
	8000,
	12910,
	16000,
	18000,
	20000,
	21000,
	23000,
	25000,
	26000,
	27000,
	30000,
	34992,
	40000,
	42000,
	46000,
	49000,
	0,
	3375,
	14500,
	17000,
	18968,
	19000,
	0,
	3750,
	3875,
	5625,
	6875,
	8250,
	8937,
	11125,
	11375,
	12125,
	0,
	12500,
	0,
	7000,
	10750,
	16000,
	20250,
	22500,
	25750,
	28750,
	29000,
	-5500,
	0,
	11000,
	0,
	4500,
	0,
	2250,
	4000,
	5500,
	6250,
	7250,
	8250,
	9250,
	10250,
	11250,
	12250,
	13250,
	14250,
	46000,
	0,
	1000,
	2000,
	3000,
	4000,
	5000,
	6000,
	7000,
	8000,
	9000,
	16000,
	16500,
	32500,
	0,
	17000,
	0,
	4000,
	0,
	4625,
	0,
	3250,
	0,
	2875,
	0,
	1000,
	0,
	1000,
	0,
	1000,
	0,
	1000,
	0,
	1625,
	3500,
	0,
	2875,
	4125,
	4500,
	0,
	1000,
	0,
	4000,
	0,
	1000,
	0,
	500,
	0,
	750,
	0,
	500,
	2125,
	3375,
	5875,
	10750,
	0,
	125,
	39625,
	0,
	19813,
	0,
	19688,
	0,
	18000,
	0,
	8500,
	15096,
	17750,
	22000,
	0,
	31500,
	0,
	13500,
	33000,
	48500,
	49000,
	51000,
	55000,
	0,
	9500,
	0,
	15875,
	-6000,
	0,
	13000,
	16875,
	77000,
	0,
	4000,
	0,
	1250,
	0,
	4000,
	0,
	3500,
	5500,
	7500,
	8125,
	0,
	87250,
	0,
	128000,
	0,
	96000,
	0,
	13000,
	0,
	79000,
	0,
	71500,
	0,
	128000,
	0,
	1250,
	2500,
	0,
	2000,
	4500,
	11000,
	0,
	4500,
	0,
	8625,
	0,
	1000,
	2000,
	3000,
	4250,
	4500,
	0,
	1500,
	2500,
	3500,
	5000,
	5500,
	6500,
	0,
	4000,
	4500,
	0,
	1500,
	3500,
	0,
	1000,
	3500,
	7000,
	10500,
	13500,
	17000,
	18000,
	20000,
	21500,
	0,
	1500,
	4000,
	0,
	1750,
	9750,
	12250,
	14500,
	15750,
	16750,
	16750,
	17500,
	24250,
	0,
	1500,
	3500,
	5000,
	6000,
	8000,
	10000,
	0,
	1000,
	2500,
	4000,
	5500,
	8500,
	0,
	2500,
	7500,
	8500,
	11000,
	12500,
	0,
	3000,
	7000,
	10500,
	23500,
	0,
	1000,
	3000,
	13000,
	0,
	2500,
	4500,
	5500,
	9000,
	0,
	6500,
	17500,
	24500,
	0,
	3000,
	4000,
	0,
	5000,
	18000,
	31000,
	53000,
	54000,
	0,
	1000,
	4500,
	7500,
	8500,
	0,
	1500,
	2500,
	3500,
	0,
	2000,
	14500,
	0,
	3000,
	5500,
	8500,
	11000,
	14000,
	0,
	1500,
	6500,
};
int keyframe_time_index[15] = {
	0,
	65,
	83,
	259,
	432,
	459,
	461,
	463,
	465,
	471,
	473,
	516,
	572,
	584,
	593,
};
#endif
