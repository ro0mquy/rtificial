#ifndef SHADER_SOURCES_H
#define SHADER_SOURCES_H

static const char vertex_source[] = "\
#version 330\n\
in vec2 coord2d;\
\
void main(void) {\
	gl_Position = vec4(coord2d, 0.0, 1.0);\
}\
";

static const char post_vertex_source[] = "\
#version 330\n\
in vec2 coord2d;\
out vec2 texcoord;\
void main() {\
	texcoord = (coord2d + 1.) / 2.;\
	gl_Position = vec4(coord2d, 0.0, 1.0);\
}\
";

static const char post_default_fragment_source[] = "\
#version 330\n\
in vec2 texcoord;\
out vec4 out_color;\
uniform sampler2D tex;\
void main() {\
	vec3 color = texture(tex, texcoord).rgb;\
	color = pow(color, vec3(1 / 2.2));\
	out_color.rgb = color;\
	out_color.a = dot(color, vec3(.2126, .7152, .0722));\
}\
";

// only pass bright pixels and scale image down
static const char bloom_fragment_source0[] = "\
#version 330\n\
\
in vec2 texcoord;\
out vec3 out_color;\
\
uniform sampler2D tex;\
\
void main() {\
	out_color = texture(tex, texcoord).rgb;\
	float luma = dot(out_color, vec3(.2126, .7152, .0722));\
	float treshold = .0;\
	if (luma < treshold) {\
		out_color = vec3(0);\
	}\
}\
";

static const char bloom_fragment_source1[] = "\
#version 330\n\
\
in vec2 texcoord;\
out vec3 out_color;\
\
uniform sampler2D tex;\
\
void main() {\
	out_color = texture(tex, texcoord).rgb;\
}\
";

static const char bloom_fragment_source2[] = "\
#version 330\n\
\
in vec2 texcoord;\
out vec3 out_color;\
\
uniform sampler2D tex;\
uniform sampler2D blurtex;\
\
void main() {\
	out_color = texture(blurtex, texcoord).rgb;\
}\
";
static const char* bloom_fragment_sources[3] = {
	bloom_fragment_source0,
	bloom_fragment_source1,
	bloom_fragment_source2,
};

static const char fxaa_fragment_source[] = "\
#version 330\n\
\n\
in vec2 texcoord;\n\
out vec3 out_color;\n\
\n\
uniform sampler2D tex;\n\
\n\
float rgba2luma(vec4 rgba) {\n\
	return rgba.a;\n\
}\n\
\n\
// NVIDIA FXAA 3.11 by TIMOTHY LOTTES\n\
// TODO: Insure the texture sampler(s) used by FXAA are set to bilinear filtering.\n\
\n\
/*============================================================================\n\
                        FXAA QUALITY - TUNING KNOBS\n\
------------------------------------------------------------------------------\n\
NOTE the other tuning knobs are now in the shader function inputs!\n\
============================================================================*/\n\
#ifndef FXAA_QUALITY__PRESET\n\
    //\n\
    // Choose the quality preset.\n\
    // This needs to be compiled into the shader as it effects code.\n\
    // Best option to include multiple presets is to \n\
    // in each shader define the preset, then include this file.\n\
    // \n\
    // OPTIONS\n\
    // -----------------------------------------------------------------------\n\
    // 10 to 15 - default medium dither (10=fastest, 15=highest quality)\n\
    // 20 to 29 - less dither, more expensive (20=fastest, 29=highest quality)\n\
    // 39       - no dither, very expensive \n\
    //\n\
    // NOTES\n\
    // -----------------------------------------------------------------------\n\
    // 12 = slightly faster then FXAA 3.9 and higher edge quality (default)\n\
    // 13 = about same speed as FXAA 3.9 and better than 12\n\
    // 23 = closest to FXAA 3.9 visually and performance wise\n\
    //  _ = the lowest digit is directly related to performance\n\
    // _  = the highest digit is directly related to style\n\
    // \n\
    #define FXAA_QUALITY__PRESET 12\n\
#endif\n\
\n\
/*============================================================================\n\
                     FXAA QUALITY - MEDIUM DITHER PRESETS\n\
============================================================================*/\n\
#if (FXAA_QUALITY__PRESET == 10)\n\
    #define FXAA_QUALITY__PS 3\n\
    #define FXAA_QUALITY__P0 1.5\n\
    #define FXAA_QUALITY__P1 3.0\n\
    #define FXAA_QUALITY__P2 12.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 11)\n\
    #define FXAA_QUALITY__PS 4\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 3.0\n\
    #define FXAA_QUALITY__P3 12.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 12)\n\
    #define FXAA_QUALITY__PS 5\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 4.0\n\
    #define FXAA_QUALITY__P4 12.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 13)\n\
    #define FXAA_QUALITY__PS 6\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 2.0\n\
    #define FXAA_QUALITY__P4 4.0\n\
    #define FXAA_QUALITY__P5 12.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 14)\n\
    #define FXAA_QUALITY__PS 7\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 2.0\n\
    #define FXAA_QUALITY__P4 2.0\n\
    #define FXAA_QUALITY__P5 4.0\n\
    #define FXAA_QUALITY__P6 12.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 15)\n\
    #define FXAA_QUALITY__PS 8\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 2.0\n\
    #define FXAA_QUALITY__P4 2.0\n\
    #define FXAA_QUALITY__P5 2.0\n\
    #define FXAA_QUALITY__P6 4.0\n\
    #define FXAA_QUALITY__P7 12.0\n\
#endif\n\
\n\
/*============================================================================\n\
                     FXAA QUALITY - LOW DITHER PRESETS\n\
============================================================================*/\n\
#if (FXAA_QUALITY__PRESET == 20)\n\
    #define FXAA_QUALITY__PS 3\n\
    #define FXAA_QUALITY__P0 1.5\n\
    #define FXAA_QUALITY__P1 2.0\n\
    #define FXAA_QUALITY__P2 8.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 21)\n\
    #define FXAA_QUALITY__PS 4\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 8.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 22)\n\
    #define FXAA_QUALITY__PS 5\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 2.0\n\
    #define FXAA_QUALITY__P4 8.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 23)\n\
    #define FXAA_QUALITY__PS 6\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 2.0\n\
    #define FXAA_QUALITY__P4 2.0\n\
    #define FXAA_QUALITY__P5 8.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 24)\n\
    #define FXAA_QUALITY__PS 7\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 2.0\n\
    #define FXAA_QUALITY__P4 2.0\n\
    #define FXAA_QUALITY__P5 3.0\n\
    #define FXAA_QUALITY__P6 8.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 25)\n\
    #define FXAA_QUALITY__PS 8\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 2.0\n\
    #define FXAA_QUALITY__P4 2.0\n\
    #define FXAA_QUALITY__P5 2.0\n\
    #define FXAA_QUALITY__P6 4.0\n\
    #define FXAA_QUALITY__P7 8.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 26)\n\
    #define FXAA_QUALITY__PS 9\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 2.0\n\
    #define FXAA_QUALITY__P4 2.0\n\
    #define FXAA_QUALITY__P5 2.0\n\
    #define FXAA_QUALITY__P6 2.0\n\
    #define FXAA_QUALITY__P7 4.0\n\
    #define FXAA_QUALITY__P8 8.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 27)\n\
    #define FXAA_QUALITY__PS 10\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 2.0\n\
    #define FXAA_QUALITY__P4 2.0\n\
    #define FXAA_QUALITY__P5 2.0\n\
    #define FXAA_QUALITY__P6 2.0\n\
    #define FXAA_QUALITY__P7 2.0\n\
    #define FXAA_QUALITY__P8 4.0\n\
    #define FXAA_QUALITY__P9 8.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 28)\n\
    #define FXAA_QUALITY__PS 11\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 2.0\n\
    #define FXAA_QUALITY__P4 2.0\n\
    #define FXAA_QUALITY__P5 2.0\n\
    #define FXAA_QUALITY__P6 2.0\n\
    #define FXAA_QUALITY__P7 2.0\n\
    #define FXAA_QUALITY__P8 2.0\n\
    #define FXAA_QUALITY__P9 4.0\n\
    #define FXAA_QUALITY__P10 8.0\n\
#endif\n\
/*--------------------------------------------------------------------------*/\n\
#if (FXAA_QUALITY__PRESET == 29)\n\
    #define FXAA_QUALITY__PS 12\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.5\n\
    #define FXAA_QUALITY__P2 2.0\n\
    #define FXAA_QUALITY__P3 2.0\n\
    #define FXAA_QUALITY__P4 2.0\n\
    #define FXAA_QUALITY__P5 2.0\n\
    #define FXAA_QUALITY__P6 2.0\n\
    #define FXAA_QUALITY__P7 2.0\n\
    #define FXAA_QUALITY__P8 2.0\n\
    #define FXAA_QUALITY__P9 2.0\n\
    #define FXAA_QUALITY__P10 4.0\n\
    #define FXAA_QUALITY__P11 8.0\n\
#endif\n\
\n\
/*============================================================================\n\
                     FXAA QUALITY - EXTREME QUALITY\n\
============================================================================*/\n\
#if (FXAA_QUALITY__PRESET == 39)\n\
    #define FXAA_QUALITY__PS 12\n\
    #define FXAA_QUALITY__P0 1.0\n\
    #define FXAA_QUALITY__P1 1.0\n\
    #define FXAA_QUALITY__P2 1.0\n\
    #define FXAA_QUALITY__P3 1.0\n\
    #define FXAA_QUALITY__P4 1.0\n\
    #define FXAA_QUALITY__P5 1.5\n\
    #define FXAA_QUALITY__P6 2.0\n\
    #define FXAA_QUALITY__P7 2.0\n\
    #define FXAA_QUALITY__P8 2.0\n\
    #define FXAA_QUALITY__P9 2.0\n\
    #define FXAA_QUALITY__P10 4.0\n\
    #define FXAA_QUALITY__P11 8.0\n\
#endif\n\
\n\
\n\
\n\
/*============================================================================\n\
                             FXAA3 QUALITY - PC\n\
============================================================================*/\n\
vec3 fxaa(\n\
    //\n\
    // Use noperspective interpolation here (turn off perspective interpolation).\n\
    // {xy} = center of pixel\n\
    //vec2 pos,\n\
    //\n\
    // Input color texture.\n\
    // {rgb_} = color in linear or perceptual color space\n\
    // if (FXAA_GREEN_AS_LUMA == 0)\n\
    //     {___a} = luma in perceptual color space (not linear)\n\
    //sampler2D tex,\n\
    //\n\
    // Only used on FXAA Quality.\n\
    // This must be from a constant/uniform.\n\
    // {x_} = 1.0/screenWidthInPixels\n\
    // {_y} = 1.0/screenHeightInPixels\n\
    //vec2 fxaaQualityRcpFrame,\n\
    //\n\
    // Only used on FXAA Quality.\n\
    // This used to be the FXAA_QUALITY__SUBPIX define.\n\
    // It is here now to allow easier tuning.\n\
    // Choose the amount of sub-pixel aliasing removal.\n\
    // This can effect sharpness.\n\
    //   1.00 - upper limit (softer)\n\
    //   0.75 - default amount of filtering\n\
    //   0.50 - lower limit (sharper, less sub-pixel aliasing removal)\n\
    //   0.25 - almost off\n\
    //   0.00 - completely off\n\
    float fxaaQualitySubpix,\n\
    //\n\
    // Only used on FXAA Quality.\n\
    // This used to be the FXAA_QUALITY__EDGE_THRESHOLD define.\n\
    // It is here now to allow easier tuning.\n\
    // The minimum amount of local contrast required to apply algorithm.\n\
    //   0.333 - too little (faster)\n\
    //   0.250 - low quality\n\
    //   0.166 - default\n\
    //   0.125 - high quality \n\
    //   0.063 - overkill (slower)\n\
    float fxaaQualityEdgeThreshold,\n\
    //\n\
    // Only used on FXAA Quality.\n\
    // This used to be the FXAA_QUALITY__EDGE_THRESHOLD_MIN define.\n\
    // It is here now to allow easier tuning.\n\
    // Trims the algorithm from processing darks.\n\
    //   0.0833 - upper limit (default, the start of visible unfiltered edges)\n\
    //   0.0625 - high quality (faster)\n\
    //   0.0312 - visible limit (slower)\n\
    float fxaaQualityEdgeThresholdMin\n\
) {\n\
/*--------------------------------------------------------------------------*/\n\
    vec2 fxaaQualityRcpFrame = 1. / textureSize(tex, 0);\n\
    //vec2 posM = pos;\n\
    vec2 posM = texcoord;\n\
\n\
    vec4 rgbyM = texture(tex, posM);\n\
    float lumaM = rgba2luma(rgbyM);\n\
    float lumaS = rgba2luma(textureOffset(tex, posM, ivec2( 0, 1)));\n\
    float lumaE = rgba2luma(textureOffset(tex, posM, ivec2( 1, 0)));\n\
    float lumaN = rgba2luma(textureOffset(tex, posM, ivec2( 0,-1)));\n\
    float lumaW = rgba2luma(textureOffset(tex, posM, ivec2(-1, 0)));\n\
/*--------------------------------------------------------------------------*/\n\
    float maxSM = max(lumaS, lumaM);\n\
    float minSM = min(lumaS, lumaM);\n\
    float maxESM = max(lumaE, maxSM);\n\
    float minESM = min(lumaE, minSM);\n\
    float maxWN = max(lumaN, lumaW);\n\
    float minWN = min(lumaN, lumaW);\n\
    float rangeMax = max(maxWN, maxESM);\n\
    float rangeMin = min(minWN, minESM);\n\
    float rangeMaxScaled = rangeMax * fxaaQualityEdgeThreshold;\n\
    float range = rangeMax - rangeMin;\n\
    float rangeMaxClamped = max(fxaaQualityEdgeThresholdMin, rangeMaxScaled);\n\
    bool earlyExit = range < rangeMaxClamped;\n\
/*--------------------------------------------------------------------------*/\n\
    if(earlyExit) {\n\
        return rgbyM.rgb;\n\
    }\n\
/*--------------------------------------------------------------------------*/\n\
    float lumaNW = rgba2luma(textureOffset(tex, posM, ivec2(-1,-1)));\n\
    float lumaSE = rgba2luma(textureOffset(tex, posM, ivec2( 1, 1)));\n\
    float lumaNE = rgba2luma(textureOffset(tex, posM, ivec2( 1,-1)));\n\
    float lumaSW = rgba2luma(textureOffset(tex, posM, ivec2(-1, 1)));\n\
/*--------------------------------------------------------------------------*/\n\
    float lumaNS = lumaN + lumaS;\n\
    float lumaWE = lumaW + lumaE;\n\
    float subpixRcpRange = 1.0/range;\n\
    float subpixNSWE = lumaNS + lumaWE;\n\
    float edgeHorz1 = (-2.0 * lumaM) + lumaNS;\n\
    float edgeVert1 = (-2.0 * lumaM) + lumaWE;\n\
/*--------------------------------------------------------------------------*/\n\
    float lumaNESE = lumaNE + lumaSE;\n\
    float lumaNWNE = lumaNW + lumaNE;\n\
    float edgeHorz2 = (-2.0 * lumaE) + lumaNESE;\n\
    float edgeVert2 = (-2.0 * lumaN) + lumaNWNE;\n\
/*--------------------------------------------------------------------------*/\n\
    float lumaNWSW = lumaNW + lumaSW;\n\
    float lumaSWSE = lumaSW + lumaSE;\n\
    float edgeHorz4 = (abs(edgeHorz1) * 2.0) + abs(edgeHorz2);\n\
    float edgeVert4 = (abs(edgeVert1) * 2.0) + abs(edgeVert2);\n\
    float edgeHorz3 = (-2.0 * lumaW) + lumaNWSW;\n\
    float edgeVert3 = (-2.0 * lumaS) + lumaSWSE;\n\
    float edgeHorz = abs(edgeHorz3) + edgeHorz4;\n\
    float edgeVert = abs(edgeVert3) + edgeVert4;\n\
/*--------------------------------------------------------------------------*/\n\
    float subpixNWSWNESE = lumaNWSW + lumaNESE;\n\
    float lengthSign = fxaaQualityRcpFrame.x;\n\
    bool horzSpan = edgeHorz >= edgeVert;\n\
    float subpixA = subpixNSWE * 2.0 + subpixNWSWNESE;\n\
/*--------------------------------------------------------------------------*/\n\
    if(!horzSpan) lumaN = lumaW;\n\
    if(!horzSpan) lumaS = lumaE;\n\
    if(horzSpan) lengthSign = fxaaQualityRcpFrame.y;\n\
    float subpixB = (subpixA * (1.0/12.0)) - lumaM;\n\
/*--------------------------------------------------------------------------*/\n\
    float gradientN = lumaN - lumaM;\n\
    float gradientS = lumaS - lumaM;\n\
    float lumaNN = lumaN + lumaM;\n\
    float lumaSS = lumaS + lumaM;\n\
    bool pairN = abs(gradientN) >= abs(gradientS);\n\
    float gradient = max(abs(gradientN), abs(gradientS));\n\
    if(pairN) lengthSign = -lengthSign;\n\
    float subpixC = clamp(abs(subpixB) * subpixRcpRange, 0., 1.);\n\
/*--------------------------------------------------------------------------*/\n\
    vec2 posB = posM;\n\
    vec2 offNP;\n\
    offNP.x = (!horzSpan) ? 0.0 : fxaaQualityRcpFrame.x;\n\
    offNP.y = ( horzSpan) ? 0.0 : fxaaQualityRcpFrame.y;\n\
    if(!horzSpan) posB.x += lengthSign * 0.5;\n\
    if( horzSpan) posB.y += lengthSign * 0.5;\n\
/*--------------------------------------------------------------------------*/\n\
    vec2 posN = posB - offNP * FXAA_QUALITY__P0;\n\
    vec2 posP = posB + offNP * FXAA_QUALITY__P0;\n\
    float subpixD = ((-2.0)*subpixC) + 3.0;\n\
    float lumaEndN = rgba2luma(texture(tex, posN));\n\
    float subpixE = subpixC * subpixC;\n\
    float lumaEndP = rgba2luma(texture(tex, posP));\n\
/*--------------------------------------------------------------------------*/\n\
    if(!pairN) lumaNN = lumaSS;\n\
    float gradientScaled = gradient * 1.0/4.0;\n\
    float lumaMM = lumaM - lumaNN * 0.5;\n\
    float subpixF = subpixD * subpixE;\n\
    bool lumaMLTZero = lumaMM < 0.0;\n\
/*--------------------------------------------------------------------------*/\n\
    lumaEndN -= lumaNN * 0.5;\n\
    lumaEndP -= lumaNN * 0.5;\n\
    bool doneN = abs(lumaEndN) >= gradientScaled;\n\
    bool doneP = abs(lumaEndP) >= gradientScaled;\n\
    if(!doneN) posN -= offNP * FXAA_QUALITY__P1;\n\
    bool doneNP = (!doneN) || (!doneP);\n\
    if(!doneP) posP += offNP * FXAA_QUALITY__P1;\n\
/*--------------------------------------------------------------------------*/\n\
    if(doneNP) {\n\
        if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));\n\
        if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));\n\
        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;\n\
        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;\n\
        doneN = abs(lumaEndN) >= gradientScaled;\n\
        doneP = abs(lumaEndP) >= gradientScaled;\n\
        if(!doneN) posN -= offNP * FXAA_QUALITY__P2;\n\
        doneNP = (!doneN) || (!doneP);\n\
        if(!doneP) posP += offNP * FXAA_QUALITY__P2;\n\
/*--------------------------------------------------------------------------*/\n\
        #if (FXAA_QUALITY__PS > 3)\n\
        if(doneNP) {\n\
            if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));\n\
            if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));\n\
            if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;\n\
            if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;\n\
            doneN = abs(lumaEndN) >= gradientScaled;\n\
            doneP = abs(lumaEndP) >= gradientScaled;\n\
            if(!doneN) posN -= offNP * FXAA_QUALITY__P3;\n\
            doneNP = (!doneN) || (!doneP);\n\
            if(!doneP) posP += offNP * FXAA_QUALITY__P3;\n\
/*--------------------------------------------------------------------------*/\n\
            #if (FXAA_QUALITY__PS > 4)\n\
            if(doneNP) {\n\
                if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));\n\
                if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));\n\
                if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;\n\
                if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;\n\
                doneN = abs(lumaEndN) >= gradientScaled;\n\
                doneP = abs(lumaEndP) >= gradientScaled;\n\
                if(!doneN) posN -= offNP * FXAA_QUALITY__P4;\n\
                doneNP = (!doneN) || (!doneP);\n\
                if(!doneP) posP += offNP * FXAA_QUALITY__P4;\n\
/*--------------------------------------------------------------------------*/\n\
                #if (FXAA_QUALITY__PS > 5)\n\
                if(doneNP) {\n\
                    if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));\n\
                    if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));\n\
                    if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;\n\
                    if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;\n\
                    doneN = abs(lumaEndN) >= gradientScaled;\n\
                    doneP = abs(lumaEndP) >= gradientScaled;\n\
                    if(!doneN) posN -= offNP * FXAA_QUALITY__P5;\n\
                    doneNP = (!doneN) || (!doneP);\n\
                    if(!doneP) posP += offNP * FXAA_QUALITY__P5;\n\
/*--------------------------------------------------------------------------*/\n\
                    #if (FXAA_QUALITY__PS > 6)\n\
                    if(doneNP) {\n\
                        if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));\n\
                        if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));\n\
                        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;\n\
                        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;\n\
                        doneN = abs(lumaEndN) >= gradientScaled;\n\
                        doneP = abs(lumaEndP) >= gradientScaled;\n\
                        if(!doneN) posN -= offNP * FXAA_QUALITY__P6;\n\
                        doneNP = (!doneN) || (!doneP);\n\
                        if(!doneP) posP += offNP * FXAA_QUALITY__P6;\n\
/*--------------------------------------------------------------------------*/\n\
                        #if (FXAA_QUALITY__PS > 7)\n\
                        if(doneNP) {\n\
                            if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));\n\
                            if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));\n\
                            if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;\n\
                            if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;\n\
                            doneN = abs(lumaEndN) >= gradientScaled;\n\
                            doneP = abs(lumaEndP) >= gradientScaled;\n\
                            if(!doneN) posN -= offNP * FXAA_QUALITY__P7;\n\
                            doneNP = (!doneN) || (!doneP);\n\
                            if(!doneP) posP += offNP * FXAA_QUALITY__P7;\n\
/*--------------------------------------------------------------------------*/\n\
    #if (FXAA_QUALITY__PS > 8)\n\
    if(doneNP) {\n\
        if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));\n\
        if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));\n\
        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;\n\
        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;\n\
        doneN = abs(lumaEndN) >= gradientScaled;\n\
        doneP = abs(lumaEndP) >= gradientScaled;\n\
        if(!doneN) posN -= offNP * FXAA_QUALITY__P8;\n\
        doneNP = (!doneN) || (!doneP);\n\
        if(!doneP) posP += offNP * FXAA_QUALITY__P8;\n\
/*--------------------------------------------------------------------------*/\n\
        #if (FXAA_QUALITY__PS > 9)\n\
        if(doneNP) {\n\
            if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));\n\
            if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));\n\
            if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;\n\
            if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;\n\
            doneN = abs(lumaEndN) >= gradientScaled;\n\
            doneP = abs(lumaEndP) >= gradientScaled;\n\
            if(!doneN) posN -= offNP * FXAA_QUALITY__P9;\n\
            doneNP = (!doneN) || (!doneP);\n\
            if(!doneP) posP += offNP * FXAA_QUALITY__P9;\n\
/*--------------------------------------------------------------------------*/\n\
            #if (FXAA_QUALITY__PS > 10)\n\
            if(doneNP) {\n\
                if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));\n\
                if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));\n\
                if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;\n\
                if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;\n\
                doneN = abs(lumaEndN) >= gradientScaled;\n\
                doneP = abs(lumaEndP) >= gradientScaled;\n\
                if(!doneN) posN -= offNP * FXAA_QUALITY__P10;\n\
                doneNP = (!doneN) || (!doneP);\n\
                if(!doneP) posP += offNP * FXAA_QUALITY__P10;\n\
/*--------------------------------------------------------------------------*/\n\
                #if (FXAA_QUALITY__PS > 11)\n\
                if(doneNP) {\n\
                    if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));\n\
                    if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));\n\
                    if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;\n\
                    if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;\n\
                    doneN = abs(lumaEndN) >= gradientScaled;\n\
                    doneP = abs(lumaEndP) >= gradientScaled;\n\
                    if(!doneN) posN -= offNP * FXAA_QUALITY__P11;\n\
                    doneNP = (!doneN) || (!doneP);\n\
                    if(!doneP) posP += offNP * FXAA_QUALITY__P11;\n\
/*--------------------------------------------------------------------------*/\n\
                    #if (FXAA_QUALITY__PS > 12)\n\
                    if(doneNP) {\n\
                        if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));\n\
                        if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));\n\
                        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;\n\
                        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;\n\
                        doneN = abs(lumaEndN) >= gradientScaled;\n\
                        doneP = abs(lumaEndP) >= gradientScaled;\n\
                        if(!doneN) posN -= offNP * FXAA_QUALITY__P12;\n\
                        doneNP = (!doneN) || (!doneP);\n\
                        if(!doneP) posP += offNP * FXAA_QUALITY__P12;\n\
/*--------------------------------------------------------------------------*/\n\
                    }\n\
                    #endif\n\
/*--------------------------------------------------------------------------*/\n\
                }\n\
                #endif\n\
/*--------------------------------------------------------------------------*/\n\
            }\n\
            #endif\n\
/*--------------------------------------------------------------------------*/\n\
        }\n\
        #endif\n\
/*--------------------------------------------------------------------------*/\n\
    }\n\
    #endif\n\
/*--------------------------------------------------------------------------*/\n\
                        }\n\
                        #endif\n\
/*--------------------------------------------------------------------------*/\n\
                    }\n\
                    #endif\n\
/*--------------------------------------------------------------------------*/\n\
                }\n\
                #endif\n\
/*--------------------------------------------------------------------------*/\n\
            }\n\
            #endif\n\
/*--------------------------------------------------------------------------*/\n\
        }\n\
        #endif\n\
/*--------------------------------------------------------------------------*/\n\
    }\n\
/*--------------------------------------------------------------------------*/\n\
    float dstN = posM.x - posN.x;\n\
    float dstP = posP.x - posM.x;\n\
    if(!horzSpan) dstN = posM.y - posN.y;\n\
    if(!horzSpan) dstP = posP.y - posM.y;\n\
/*--------------------------------------------------------------------------*/\n\
    bool goodSpanN = (lumaEndN < 0.0) != lumaMLTZero;\n\
    float spanLength = (dstP + dstN);\n\
    bool goodSpanP = (lumaEndP < 0.0) != lumaMLTZero;\n\
    float spanLengthRcp = 1.0/spanLength;\n\
/*--------------------------------------------------------------------------*/\n\
    bool directionN = dstN < dstP;\n\
    float dst = min(dstN, dstP);\n\
    bool goodSpan = directionN ? goodSpanN : goodSpanP;\n\
    float subpixG = subpixF * subpixF;\n\
    float pixelOffset = (dst * (-spanLengthRcp)) + 0.5;\n\
    float subpixH = subpixG * fxaaQualitySubpix;\n\
/*--------------------------------------------------------------------------*/\n\
    float pixelOffsetGood = goodSpan ? pixelOffset : 0.0;\n\
    float pixelOffsetSubpix = max(pixelOffsetGood, subpixH);\n\
    if(!horzSpan) posM.x += pixelOffsetSubpix * lengthSign;\n\
    if( horzSpan) posM.y += pixelOffsetSubpix * lengthSign;\n\
    return texture(tex, posM).rgb;\n\
}\n\
\n\
void main() {\n\
	out_color = fxaa(.75, .25, .0625);\n\
}\n\
";


#endif
