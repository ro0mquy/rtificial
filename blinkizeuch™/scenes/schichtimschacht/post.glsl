
#version 330

in vec2 texcoord;
out vec3 out_color;

uniform sampler2D tex;
uniform sampler2D tex_depth;

uniform mat4 previous_world_to_camera_matrix;
uniform mat4 inverse_world_to_camera_matrix;

vec3 fxaa() {
	// The parameters are hardcoded for now, but could be
	// made into uniforms to control fromt he program.
	float FXAA_SPAN_MAX = 8.0;
	float FXAA_REDUCE_MUL = 1.0 / 8.0;
	float FXAA_REDUCE_MIN = 1.0 / 128.0;

	vec2 offset = 1. / textureSize(tex, 0);

	vec3 rgbNW = textureOffset(tex, texcoord, ivec2(-1, -1)).rgb;
	vec3 rgbNE = textureOffset(tex, texcoord, ivec2(+1, -1)).rgb;
	vec3 rgbSW = textureOffset(tex, texcoord, ivec2(-1, +1)).rgb;
	vec3 rgbSE = textureOffset(tex, texcoord, ivec2(+1, +1)).rgb;
	vec3 rgbM  = texture2D(tex, texcoord).rgb;

	vec3 luma = vec3(.2126, .7152, .0722);
	float lumaNW = dot(rgbNW, luma);
	float lumaNE = dot(rgbNE, luma);
	float lumaSW = dot(rgbSW, luma);
	float lumaSE = dot(rgbSE, luma);
	float lumaM  = dot(rgbM , luma);

	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

	vec2 dir;
	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
	dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);

	float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

	dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
			max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) * offset;

	vec3 rgbA = (1.0/2.0) * (
			texture2D(tex, texcoord + dir * (1.0/3.0 - 0.5)).xyz +
			texture2D(tex, texcoord + dir * (2.0/3.0 - 0.5)).xyz);
	vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
			texture2D(tex, texcoord + dir * (0.0/3.0 - 0.5)).xyz +
			texture2D(tex, texcoord + dir * (3.0/3.0 - 0.5)).xyz);
	float lumaB = dot(rgbB, luma);

	if((lumaB < lumaMin) || (lumaB > lumaMax)){
		return rgbA;
	} else {
		return rgbB;
	}
}

void main() {
	vec3 color = texture(tex, texcoord).rgb;
	float depth = texture(tex_depth, texcoord);

	if(texcoord.x > 0.5)
	color = vec3(1-depth);

	//color = fxaa();

	out_color = color;
}
