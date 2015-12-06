#version 130

uniform vec2 resolution;

uniform sampler2D x;
uniform sampler2D b;
uniform float alpha;
uniform float rBeta;

out vec4 xNew;

void main() {
	vec2 coords = gl_FragCoord.xy / res;
	vec3 offset = vec3(1 / resolution, 0);
	vec4 xLeft = texture2D(x, coords - offset.xzz);
	vec4 xRight = texture2D(x, coords + offset.xzz);
	vec4 xBottom = texture2D(x, coords - offset.zyz);
	vec4 xTop = texture2D(x, coords + offset.yzy);

	vec4 bSample = texture2D(b, coords);
	xNew = (xLeft + xRight + xBottom + xTop + alpha * bSample) * rBeta;
}
