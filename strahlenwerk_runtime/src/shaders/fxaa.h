#ifndef fxaa_H
#define fxaa_H
const char fxaa_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(in vec2 tc;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec2 texcoord = tc;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 25) uniform sampler2D tex; // vec3
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(float rgba2luma(vec4 rgba) {
)shader_source"
R"shader_source(	return dot(rgba.rgb, vec3(.2126, .7152, .0722));
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// NVIDIA FXAA 3.11 by TIMOTHY LOTTES
)shader_source"
R"shader_source(// TODO: Insure the texture sampler(s) used by FXAA are set to bilinear filtering.
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/*============================================================================
)shader_source"
R"shader_source(                        FXAA QUALITY - TUNING KNOBS
)shader_source"
R"shader_source(------------------------------------------------------------------------------
)shader_source"
R"shader_source(NOTE the other tuning knobs are now in the shader function inputs!
)shader_source"
R"shader_source(============================================================================*/
)shader_source"
R"shader_source(#ifndef FXAA_QUALITY__PRESET
)shader_source"
R"shader_source(    //
)shader_source"
R"shader_source(    // Choose the quality preset.
)shader_source"
R"shader_source(    // This needs to be compiled into the shader as it effects code.
)shader_source"
R"shader_source(    // Best option to include multiple presets is to 
)shader_source"
R"shader_source(    // in each shader define the preset, then include this file.
)shader_source"
R"shader_source(    // 
)shader_source"
R"shader_source(    // OPTIONS
)shader_source"
R"shader_source(    // -----------------------------------------------------------------------
)shader_source"
R"shader_source(    // 10 to 15 - default medium dither (10=fastest, 15=highest quality)
)shader_source"
R"shader_source(    // 20 to 29 - less dither, more expensive (20=fastest, 29=highest quality)
)shader_source"
R"shader_source(    // 39       - no dither, very expensive 
)shader_source"
R"shader_source(    //
)shader_source"
R"shader_source(    // NOTES
)shader_source"
R"shader_source(    // -----------------------------------------------------------------------
)shader_source"
R"shader_source(    // 12 = slightly faster then FXAA 3.9 and higher edge quality (default)
)shader_source"
R"shader_source(    // 13 = about same speed as FXAA 3.9 and better than 12
)shader_source"
R"shader_source(    // 23 = closest to FXAA 3.9 visually and performance wise
)shader_source"
R"shader_source(    //  _ = the lowest digit is directly related to performance
)shader_source"
R"shader_source(    // _  = the highest digit is directly related to style
)shader_source"
R"shader_source(    // 
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PRESET 13
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/*============================================================================
)shader_source"
R"shader_source(                     FXAA QUALITY - MEDIUM DITHER PRESETS
)shader_source"
R"shader_source(============================================================================*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 10)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 3
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 3.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 12.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 11)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 4
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 3.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 12.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 12)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 4.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 12.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 13)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 6
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 4.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P5 12.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 14)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 7
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P5 4.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P6 12.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 15)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 8
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P5 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P6 4.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P7 12.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/*============================================================================
)shader_source"
R"shader_source(                     FXAA QUALITY - LOW DITHER PRESETS
)shader_source"
R"shader_source(============================================================================*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 20)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 3
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 8.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 21)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 4
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 8.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 22)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 8.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 23)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 6
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P5 8.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 24)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 7
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P5 3.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P6 8.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 25)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 8
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P5 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P6 4.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P7 8.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 26)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 9
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P5 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P6 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P7 4.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P8 8.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 27)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 10
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P5 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P6 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P7 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P8 4.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P9 8.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 28)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 11
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P5 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P6 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P7 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P8 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P9 4.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P10 8.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 29)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 12
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P5 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P6 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P7 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P8 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P9 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P10 4.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P11 8.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/*============================================================================
)shader_source"
R"shader_source(                     FXAA QUALITY - EXTREME QUALITY
)shader_source"
R"shader_source(============================================================================*/
)shader_source"
R"shader_source(#if (FXAA_QUALITY__PRESET == 39)
)shader_source"
R"shader_source(    #define FXAA_QUALITY__PS 12
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P0 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P1 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P2 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P3 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P4 1.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P5 1.5
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P6 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P7 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P8 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P9 2.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P10 4.0
)shader_source"
R"shader_source(    #define FXAA_QUALITY__P11 8.0
)shader_source"
R"shader_source(#endif
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(/*============================================================================
)shader_source"
R"shader_source(                             FXAA3 QUALITY - PC
)shader_source"
R"shader_source(============================================================================*/
)shader_source"
R"shader_source(vec3 fxaa(
)shader_source"
R"shader_source(    //
)shader_source"
R"shader_source(    // Use noperspective interpolation here (turn off perspective interpolation).
)shader_source"
R"shader_source(    // {xy} = center of pixel
)shader_source"
R"shader_source(    //vec2 pos,
)shader_source"
R"shader_source(    //
)shader_source"
R"shader_source(    // Input color texture.
)shader_source"
R"shader_source(    // {rgb_} = color in linear or perceptual color space
)shader_source"
R"shader_source(    // if (FXAA_GREEN_AS_LUMA == 0)
)shader_source"
R"shader_source(    //     {___a} = luma in perceptual color space (not linear)
)shader_source"
R"shader_source(    //sampler2D tex,
)shader_source"
R"shader_source(    //
)shader_source"
R"shader_source(    // Only used on FXAA Quality.
)shader_source"
R"shader_source(    // This must be from a constant/uniform.
)shader_source"
R"shader_source(    // {x_} = 1.0/screenWidthInPixels
)shader_source"
R"shader_source(    // {_y} = 1.0/screenHeightInPixels
)shader_source"
R"shader_source(    //vec2 fxaaQualityRcpFrame,
)shader_source"
R"shader_source(    //
)shader_source"
R"shader_source(    // Only used on FXAA Quality.
)shader_source"
R"shader_source(    // This used to be the FXAA_QUALITY__SUBPIX define.
)shader_source"
R"shader_source(    // It is here now to allow easier tuning.
)shader_source"
R"shader_source(    // Choose the amount of sub-pixel aliasing removal.
)shader_source"
R"shader_source(    // This can effect sharpness.
)shader_source"
R"shader_source(    //   1.00 - upper limit (softer)
)shader_source"
R"shader_source(    //   0.75 - default amount of filtering
)shader_source"
R"shader_source(    //   0.50 - lower limit (sharper, less sub-pixel aliasing removal)
)shader_source"
R"shader_source(    //   0.25 - almost off
)shader_source"
R"shader_source(    //   0.00 - completely off
)shader_source"
R"shader_source(    float fxaaQualitySubpix,
)shader_source"
R"shader_source(    //
)shader_source"
R"shader_source(    // Only used on FXAA Quality.
)shader_source"
R"shader_source(    // This used to be the FXAA_QUALITY__EDGE_THRESHOLD define.
)shader_source"
R"shader_source(    // It is here now to allow easier tuning.
)shader_source"
R"shader_source(    // The minimum amount of local contrast required to apply algorithm.
)shader_source"
R"shader_source(    //   0.333 - too little (faster)
)shader_source"
R"shader_source(    //   0.250 - low quality
)shader_source"
R"shader_source(    //   0.166 - default
)shader_source"
R"shader_source(    //   0.125 - high quality 
)shader_source"
R"shader_source(    //   0.063 - overkill (slower)
)shader_source"
R"shader_source(    float fxaaQualityEdgeThreshold,
)shader_source"
R"shader_source(    //
)shader_source"
R"shader_source(    // Only used on FXAA Quality.
)shader_source"
R"shader_source(    // This used to be the FXAA_QUALITY__EDGE_THRESHOLD_MIN define.
)shader_source"
R"shader_source(    // It is here now to allow easier tuning.
)shader_source"
R"shader_source(    // Trims the algorithm from processing darks.
)shader_source"
R"shader_source(    //   0.0833 - upper limit (default, the start of visible unfiltered edges)
)shader_source"
R"shader_source(    //   0.0625 - high quality (faster)
)shader_source"
R"shader_source(    //   0.0312 - visible limit (slower)
)shader_source"
R"shader_source(    float fxaaQualityEdgeThresholdMin
)shader_source"
R"shader_source() {
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    vec2 fxaaQualityRcpFrame = 1. / textureSize(tex, 0);
)shader_source"
R"shader_source(    //vec2 posM = pos;
)shader_source"
R"shader_source(    vec2 posM = texcoord;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(    vec4 rgbyM = texture(tex, posM);
)shader_source"
R"shader_source(    float lumaM = rgba2luma(rgbyM);
)shader_source"
R"shader_source(    float lumaS = rgba2luma(textureOffset(tex, posM, ivec2( 0, 1)));
)shader_source"
R"shader_source(    float lumaE = rgba2luma(textureOffset(tex, posM, ivec2( 1, 0)));
)shader_source"
R"shader_source(    float lumaN = rgba2luma(textureOffset(tex, posM, ivec2( 0,-1)));
)shader_source"
R"shader_source(    float lumaW = rgba2luma(textureOffset(tex, posM, ivec2(-1, 0)));
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    float maxSM = max(lumaS, lumaM);
)shader_source"
R"shader_source(    float minSM = min(lumaS, lumaM);
)shader_source"
R"shader_source(    float maxESM = max(lumaE, maxSM);
)shader_source"
R"shader_source(    float minESM = min(lumaE, minSM);
)shader_source"
R"shader_source(    float maxWN = max(lumaN, lumaW);
)shader_source"
R"shader_source(    float minWN = min(lumaN, lumaW);
)shader_source"
R"shader_source(    float rangeMax = max(maxWN, maxESM);
)shader_source"
R"shader_source(    float rangeMin = min(minWN, minESM);
)shader_source"
R"shader_source(    float rangeMaxScaled = rangeMax * fxaaQualityEdgeThreshold;
)shader_source"
R"shader_source(    float range = rangeMax - rangeMin;
)shader_source"
R"shader_source(    float rangeMaxClamped = max(fxaaQualityEdgeThresholdMin, rangeMaxScaled);
)shader_source"
R"shader_source(    bool earlyExit = range < rangeMaxClamped;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    if(earlyExit) {
)shader_source"
R"shader_source(        return rgbyM.rgb;
)shader_source"
R"shader_source(    }
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    float lumaNW = rgba2luma(textureOffset(tex, posM, ivec2(-1,-1)));
)shader_source"
R"shader_source(    float lumaSE = rgba2luma(textureOffset(tex, posM, ivec2( 1, 1)));
)shader_source"
R"shader_source(    float lumaNE = rgba2luma(textureOffset(tex, posM, ivec2( 1,-1)));
)shader_source"
R"shader_source(    float lumaSW = rgba2luma(textureOffset(tex, posM, ivec2(-1, 1)));
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    float lumaNS = lumaN + lumaS;
)shader_source"
R"shader_source(    float lumaWE = lumaW + lumaE;
)shader_source"
R"shader_source(    float subpixRcpRange = 1.0/range;
)shader_source"
R"shader_source(    float subpixNSWE = lumaNS + lumaWE;
)shader_source"
R"shader_source(    float edgeHorz1 = (-2.0 * lumaM) + lumaNS;
)shader_source"
R"shader_source(    float edgeVert1 = (-2.0 * lumaM) + lumaWE;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    float lumaNESE = lumaNE + lumaSE;
)shader_source"
R"shader_source(    float lumaNWNE = lumaNW + lumaNE;
)shader_source"
R"shader_source(    float edgeHorz2 = (-2.0 * lumaE) + lumaNESE;
)shader_source"
R"shader_source(    float edgeVert2 = (-2.0 * lumaN) + lumaNWNE;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    float lumaNWSW = lumaNW + lumaSW;
)shader_source"
R"shader_source(    float lumaSWSE = lumaSW + lumaSE;
)shader_source"
R"shader_source(    float edgeHorz4 = (abs(edgeHorz1) * 2.0) + abs(edgeHorz2);
)shader_source"
R"shader_source(    float edgeVert4 = (abs(edgeVert1) * 2.0) + abs(edgeVert2);
)shader_source"
R"shader_source(    float edgeHorz3 = (-2.0 * lumaW) + lumaNWSW;
)shader_source"
R"shader_source(    float edgeVert3 = (-2.0 * lumaS) + lumaSWSE;
)shader_source"
R"shader_source(    float edgeHorz = abs(edgeHorz3) + edgeHorz4;
)shader_source"
R"shader_source(    float edgeVert = abs(edgeVert3) + edgeVert4;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    float subpixNWSWNESE = lumaNWSW + lumaNESE;
)shader_source"
R"shader_source(    float lengthSign = fxaaQualityRcpFrame.x;
)shader_source"
R"shader_source(    bool horzSpan = edgeHorz >= edgeVert;
)shader_source"
R"shader_source(    float subpixA = subpixNSWE * 2.0 + subpixNWSWNESE;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    if(!horzSpan) lumaN = lumaW;
)shader_source"
R"shader_source(    if(!horzSpan) lumaS = lumaE;
)shader_source"
R"shader_source(    if(horzSpan) lengthSign = fxaaQualityRcpFrame.y;
)shader_source"
R"shader_source(    float subpixB = (subpixA * (1.0/12.0)) - lumaM;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    float gradientN = lumaN - lumaM;
)shader_source"
R"shader_source(    float gradientS = lumaS - lumaM;
)shader_source"
R"shader_source(    float lumaNN = lumaN + lumaM;
)shader_source"
R"shader_source(    float lumaSS = lumaS + lumaM;
)shader_source"
R"shader_source(    bool pairN = abs(gradientN) >= abs(gradientS);
)shader_source"
R"shader_source(    float gradient = max(abs(gradientN), abs(gradientS));
)shader_source"
R"shader_source(    if(pairN) lengthSign = -lengthSign;
)shader_source"
R"shader_source(    float subpixC = clamp(abs(subpixB) * subpixRcpRange, 0., 1.);
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    vec2 posB = posM;
)shader_source"
R"shader_source(    vec2 offNP;
)shader_source"
R"shader_source(    offNP.x = (!horzSpan) ? 0.0 : fxaaQualityRcpFrame.x;
)shader_source"
R"shader_source(    offNP.y = ( horzSpan) ? 0.0 : fxaaQualityRcpFrame.y;
)shader_source"
R"shader_source(    if(!horzSpan) posB.x += lengthSign * 0.5;
)shader_source"
R"shader_source(    if( horzSpan) posB.y += lengthSign * 0.5;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    vec2 posN = posB - offNP * FXAA_QUALITY__P0;
)shader_source"
R"shader_source(    vec2 posP = posB + offNP * FXAA_QUALITY__P0;
)shader_source"
R"shader_source(    float subpixD = ((-2.0)*subpixC) + 3.0;
)shader_source"
R"shader_source(    float lumaEndN = rgba2luma(texture(tex, posN));
)shader_source"
R"shader_source(    float subpixE = subpixC * subpixC;
)shader_source"
R"shader_source(    float lumaEndP = rgba2luma(texture(tex, posP));
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    if(!pairN) lumaNN = lumaSS;
)shader_source"
R"shader_source(    float gradientScaled = gradient * 1.0/4.0;
)shader_source"
R"shader_source(    float lumaMM = lumaM - lumaNN * 0.5;
)shader_source"
R"shader_source(    float subpixF = subpixD * subpixE;
)shader_source"
R"shader_source(    bool lumaMLTZero = lumaMM < 0.0;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    lumaEndN -= lumaNN * 0.5;
)shader_source"
R"shader_source(    lumaEndP -= lumaNN * 0.5;
)shader_source"
R"shader_source(    bool doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(    bool doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(    if(!doneN) posN -= offNP * FXAA_QUALITY__P1;
)shader_source"
R"shader_source(    bool doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(    if(!doneP) posP += offNP * FXAA_QUALITY__P1;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    if(doneNP) {
)shader_source"
R"shader_source(        if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));
)shader_source"
R"shader_source(        if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));
)shader_source"
R"shader_source(        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
)shader_source"
R"shader_source(        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
)shader_source"
R"shader_source(        doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(        doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(        if(!doneN) posN -= offNP * FXAA_QUALITY__P2;
)shader_source"
R"shader_source(        doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(        if(!doneP) posP += offNP * FXAA_QUALITY__P2;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(        #if (FXAA_QUALITY__PS > 3)
)shader_source"
R"shader_source(        if(doneNP) {
)shader_source"
R"shader_source(            if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));
)shader_source"
R"shader_source(            if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));
)shader_source"
R"shader_source(            if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
)shader_source"
R"shader_source(            if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
)shader_source"
R"shader_source(            doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(            doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(            if(!doneN) posN -= offNP * FXAA_QUALITY__P3;
)shader_source"
R"shader_source(            doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(            if(!doneP) posP += offNP * FXAA_QUALITY__P3;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(            #if (FXAA_QUALITY__PS > 4)
)shader_source"
R"shader_source(            if(doneNP) {
)shader_source"
R"shader_source(                if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));
)shader_source"
R"shader_source(                if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));
)shader_source"
R"shader_source(                if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
)shader_source"
R"shader_source(                if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
)shader_source"
R"shader_source(                doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(                doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(                if(!doneN) posN -= offNP * FXAA_QUALITY__P4;
)shader_source"
R"shader_source(                doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(                if(!doneP) posP += offNP * FXAA_QUALITY__P4;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(                #if (FXAA_QUALITY__PS > 5)
)shader_source"
R"shader_source(                if(doneNP) {
)shader_source"
R"shader_source(                    if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));
)shader_source"
R"shader_source(                    if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));
)shader_source"
R"shader_source(                    if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
)shader_source"
R"shader_source(                    if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
)shader_source"
R"shader_source(                    doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(                    doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(                    if(!doneN) posN -= offNP * FXAA_QUALITY__P5;
)shader_source"
R"shader_source(                    doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(                    if(!doneP) posP += offNP * FXAA_QUALITY__P5;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(                    #if (FXAA_QUALITY__PS > 6)
)shader_source"
R"shader_source(                    if(doneNP) {
)shader_source"
R"shader_source(                        if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));
)shader_source"
R"shader_source(                        if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));
)shader_source"
R"shader_source(                        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
)shader_source"
R"shader_source(                        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
)shader_source"
R"shader_source(                        doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(                        doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(                        if(!doneN) posN -= offNP * FXAA_QUALITY__P6;
)shader_source"
R"shader_source(                        doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(                        if(!doneP) posP += offNP * FXAA_QUALITY__P6;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(                        #if (FXAA_QUALITY__PS > 7)
)shader_source"
R"shader_source(                        if(doneNP) {
)shader_source"
R"shader_source(                            if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));
)shader_source"
R"shader_source(                            if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));
)shader_source"
R"shader_source(                            if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
)shader_source"
R"shader_source(                            if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
)shader_source"
R"shader_source(                            doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(                            doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(                            if(!doneN) posN -= offNP * FXAA_QUALITY__P7;
)shader_source"
R"shader_source(                            doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(                            if(!doneP) posP += offNP * FXAA_QUALITY__P7;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    #if (FXAA_QUALITY__PS > 8)
)shader_source"
R"shader_source(    if(doneNP) {
)shader_source"
R"shader_source(        if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));
)shader_source"
R"shader_source(        if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));
)shader_source"
R"shader_source(        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
)shader_source"
R"shader_source(        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
)shader_source"
R"shader_source(        doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(        doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(        if(!doneN) posN -= offNP * FXAA_QUALITY__P8;
)shader_source"
R"shader_source(        doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(        if(!doneP) posP += offNP * FXAA_QUALITY__P8;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(        #if (FXAA_QUALITY__PS > 9)
)shader_source"
R"shader_source(        if(doneNP) {
)shader_source"
R"shader_source(            if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));
)shader_source"
R"shader_source(            if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));
)shader_source"
R"shader_source(            if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
)shader_source"
R"shader_source(            if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
)shader_source"
R"shader_source(            doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(            doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(            if(!doneN) posN -= offNP * FXAA_QUALITY__P9;
)shader_source"
R"shader_source(            doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(            if(!doneP) posP += offNP * FXAA_QUALITY__P9;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(            #if (FXAA_QUALITY__PS > 10)
)shader_source"
R"shader_source(            if(doneNP) {
)shader_source"
R"shader_source(                if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));
)shader_source"
R"shader_source(                if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));
)shader_source"
R"shader_source(                if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
)shader_source"
R"shader_source(                if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
)shader_source"
R"shader_source(                doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(                doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(                if(!doneN) posN -= offNP * FXAA_QUALITY__P10;
)shader_source"
R"shader_source(                doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(                if(!doneP) posP += offNP * FXAA_QUALITY__P10;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(                #if (FXAA_QUALITY__PS > 11)
)shader_source"
R"shader_source(                if(doneNP) {
)shader_source"
R"shader_source(                    if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));
)shader_source"
R"shader_source(                    if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));
)shader_source"
R"shader_source(                    if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
)shader_source"
R"shader_source(                    if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
)shader_source"
R"shader_source(                    doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(                    doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(                    if(!doneN) posN -= offNP * FXAA_QUALITY__P11;
)shader_source"
R"shader_source(                    doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(                    if(!doneP) posP += offNP * FXAA_QUALITY__P11;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(                    #if (FXAA_QUALITY__PS > 12)
)shader_source"
R"shader_source(                    if(doneNP) {
)shader_source"
R"shader_source(                        if(!doneN) lumaEndN = rgba2luma(texture(tex, posN.xy));
)shader_source"
R"shader_source(                        if(!doneP) lumaEndP = rgba2luma(texture(tex, posP.xy));
)shader_source"
R"shader_source(                        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
)shader_source"
R"shader_source(                        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
)shader_source"
R"shader_source(                        doneN = abs(lumaEndN) >= gradientScaled;
)shader_source"
R"shader_source(                        doneP = abs(lumaEndP) >= gradientScaled;
)shader_source"
R"shader_source(                        if(!doneN) posN -= offNP * FXAA_QUALITY__P12;
)shader_source"
R"shader_source(                        doneNP = (!doneN) || (!doneP);
)shader_source"
R"shader_source(                        if(!doneP) posP += offNP * FXAA_QUALITY__P12;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(                    }
)shader_source"
R"shader_source(                    #endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(                }
)shader_source"
R"shader_source(                #endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(            }
)shader_source"
R"shader_source(            #endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(        }
)shader_source"
R"shader_source(        #endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    }
)shader_source"
R"shader_source(    #endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(                        }
)shader_source"
R"shader_source(                        #endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(                    }
)shader_source"
R"shader_source(                    #endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(                }
)shader_source"
R"shader_source(                #endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(            }
)shader_source"
R"shader_source(            #endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(        }
)shader_source"
R"shader_source(        #endif
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    }
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    float dstN = posM.x - posN.x;
)shader_source"
R"shader_source(    float dstP = posP.x - posM.x;
)shader_source"
R"shader_source(    if(!horzSpan) dstN = posM.y - posN.y;
)shader_source"
R"shader_source(    if(!horzSpan) dstP = posP.y - posM.y;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    bool goodSpanN = (lumaEndN < 0.0) != lumaMLTZero;
)shader_source"
R"shader_source(    float spanLength = (dstP + dstN);
)shader_source"
R"shader_source(    bool goodSpanP = (lumaEndP < 0.0) != lumaMLTZero;
)shader_source"
R"shader_source(    float spanLengthRcp = 1.0/spanLength;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    bool directionN = dstN < dstP;
)shader_source"
R"shader_source(    float dst = min(dstN, dstP);
)shader_source"
R"shader_source(    bool goodSpan = directionN ? goodSpanN : goodSpanP;
)shader_source"
R"shader_source(    float subpixG = subpixF * subpixF;
)shader_source"
R"shader_source(    float pixelOffset = (dst * (-spanLengthRcp)) + 0.5;
)shader_source"
R"shader_source(    float subpixH = subpixG * fxaaQualitySubpix;
)shader_source"
R"shader_source(/*--------------------------------------------------------------------------*/
)shader_source"
R"shader_source(    float pixelOffsetGood = goodSpan ? pixelOffset : 0.0;
)shader_source"
R"shader_source(    float pixelOffsetSubpix = max(pixelOffsetGood, subpixH);
)shader_source"
R"shader_source(    if(!horzSpan) posM.x += pixelOffsetSubpix * lengthSign;
)shader_source"
R"shader_source(    if( horzSpan) posM.y += pixelOffsetSubpix * lengthSign;
)shader_source"
R"shader_source(    return texture(tex, posM).rgb;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	out_color = fxaa(.75, .125, .0625);
)shader_source"
R"shader_source(	//out_color = texture(tex, tc).rgb;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
