#include "Shader.h"
#include "Framebuffer.h"
#include "shaders/bokeh_pass1.h"
#include "shaders/bloom_small_vert.h"
#include "shaders/bloom_medium_vert.h"
#include "shaders/bloom_big_vert.h"
#include "shaders/bokeh_pass2.h"
#include "shaders/bloom_small_horiz.h"
#include "shaders/bloom_medium_horiz.h"
#include "shaders/bloom_big_horiz.h"
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
	{2, 0},
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
	{3, 0},
	{4, 0},
};
Input bloom_small_horiz_inputs[1] = {
	{5, 0},
};
Input bloom_medium_horiz_inputs[1] = {
	{6, 0},
};
Input bloom_big_horiz_inputs[1] = {
	{7, 0},
};
Input tonemap_inputs[6] = {
	{1, 11},
	{11, 0},
	{10, 0},
	{9, 0},
	{8, 0},
	{0, 0},
};
Input fxaa_inputs[1] = {
	{12, 0},
};
Input final_inputs[1] = {
	{13, 0},
};
Shader postproc[11] = {
	Shader(bokeh_pass1_source, 2, bokeh_pass1_inputs),
	Shader(bloom_small_vert_source, 1, bloom_small_vert_inputs),
	Shader(bloom_medium_vert_source, 1, bloom_medium_vert_inputs),
	Shader(bloom_big_vert_source, 1, bloom_big_vert_inputs),
	Shader(bokeh_pass2_source, 2, bokeh_pass2_inputs),
	Shader(bloom_small_horiz_source, 1, bloom_small_horiz_inputs),
	Shader(bloom_medium_horiz_source, 1, bloom_medium_horiz_inputs),
	Shader(bloom_big_horiz_source, 1, bloom_big_horiz_inputs),
	Shader(tonemap_source, 6, tonemap_inputs),
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
Output input_outputs[3] = {
	{3, 0, 4},
	{1, 1, 11},
	{1, 2, 0},
};
Output bokeh_pass1_outputs[2] = {
	{4, 3, 0},
	{4, 4, 0},
};
Output bloom_small_vert_outputs[1] = {
	{3, 5, 0},
};
Output bloom_medium_vert_outputs[1] = {
	{3, 6, 0},
};
Output bloom_big_vert_outputs[1] = {
	{3, 7, 0},
};
Output bokeh_pass2_outputs[1] = {
	{4, 8, 0},
};
Output bloom_small_horiz_outputs[1] = {
	{3, 9, 0},
};
Output bloom_medium_horiz_outputs[1] = {
	{3, 10, 0},
};
Output bloom_big_horiz_outputs[1] = {
	{3, 11, 0},
};
Output tonemap_outputs[1] = {
	{3, 12, 0},
};
Output fxaa_outputs[1] = {
	{3, 13, 0},
};
Framebuffer fbos[11] = {
	Framebuffer(0, 3, input_outputs),
	Framebuffer(0, 2, bokeh_pass1_outputs),
	Framebuffer(2, 1, bloom_small_vert_outputs),
	Framebuffer(3, 1, bloom_medium_vert_outputs),
	Framebuffer(4, 1, bloom_big_vert_outputs),
	Framebuffer(0, 1, bokeh_pass2_outputs),
	Framebuffer(2, 1, bloom_small_horiz_outputs),
	Framebuffer(3, 1, bloom_medium_horiz_outputs),
	Framebuffer(4, 1, bloom_big_horiz_outputs),
	Framebuffer(0, 1, tonemap_outputs),
	Framebuffer(0, 1, fxaa_outputs),
};
