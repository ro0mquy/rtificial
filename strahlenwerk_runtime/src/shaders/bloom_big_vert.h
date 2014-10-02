#ifndef bloom_big_vert_H
#define bloom_big_vert_H
const char bloom_big_vert_source[] =R"shader_source(#version 430)shader_source"
R"shader_source()shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;)shader_source"
R"shader_source()shader_source"
R"shader_source(layout(location = 0) in vec2 tc;)shader_source"
R"shader_source()shader_source"
R"shader_source(#line 1)shader_source"
R"shader_source()shader_source"
R"shader_source(vec3 gaussian(sampler2D tex, vec2 coords, vec2 dir, vec2 pixelsize, float scale) {)shader_source"
R"shader_source(	scale *= (4. * scale) / (1920. * pixelsize.x);)shader_source"
R"shader_source(	const float N = 7 * scale;)shader_source"
R"shader_source(	const float PI = acos(-1.);)shader_source"
R"shader_source(	const vec3 sigma = 2.65 * scale * vec3(1.34, 1.2, 1.);)shader_source"
R"shader_source(	vec3 coefficient = 1. / sqrt(2. * PI) * sigma;)shader_source"
R"shader_source(	vec3 g1 = exp(-.5 / (sigma * sigma));)shader_source"
R"shader_source(	vec3 g2 = g1 * g1;)shader_source"
R"shader_source()shader_source"
R"shader_source(	float coefficientSum = coefficient;)shader_source"
R"shader_source(	vec3 avg = textureLod(tex, coords, 0.).rgb * coefficient;)shader_source"
R"shader_source()shader_source"
R"shader_source(	for(int i = 1; i <= N; i++) {)shader_source"
R"shader_source(		coefficient *= g1;)shader_source"
R"shader_source(		g1 *= g2;)shader_source"
R"shader_source()shader_source"
R"shader_source(		avg += textureLod(tex, coords - i * dir * pixelsize, 0.).rgb * coefficient;)shader_source"
R"shader_source(		avg += textureLod(tex, coords + i * dir * pixelsize, 0.).rgb * coefficient;)shader_source"
R"shader_source()shader_source"
R"shader_source(		coefficientSum += 2. * coefficient;)shader_source"
R"shader_source(	})shader_source"
R"shader_source()shader_source"
R"shader_source(	return avg / coefficientSum;)shader_source"
R"shader_source(})shader_source"
R"shader_source()shader_source"
R"shader_source()shader_source"
R"shader_source(layout(binding = 0) uniform sampler2D color; // vec3 level(4))shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;)shader_source"
R"shader_source()shader_source"
R"shader_source(// level(4))shader_source"
R"shader_source()shader_source"
R"shader_source(void main() {)shader_source"
R"shader_source(	out_color = gaussian(color, tc, vec2(0., 1.), 1./res, 4.);)shader_source"
R"shader_source(})shader_source"
;
#endif
