#version 430

// inactive uniforms lead to crashes (!)
layout(location = 0) uniform vec2 res;
uniform float red;
uniform vec3 foo; // color
uniform vec3 camera_position;
uniform vec4 camera_rotation; // quat

layout(binding = 3) uniform samplerCube bla;

out vec4 out_color;

// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
vec3 quat_rotate(vec3 v, vec4 q) {
	vec3 t = 2 * cross(q.xyz, v);
	return v + q.w * t + cross(q.xyz, t);
	// *hex hex*
}

vec3 get_direction() {
	vec3 dir = normalize(vec3((gl_FragCoord.xy - .5 * res) / res.x , -0.02 / .03));
	return quat_rotate(dir, camera_rotation);

}
void main() {
	//out_color = mix(foo, vec3(red, gl_FragCoord.xy / res.xy), .1);
	out_color.rgb = textureLod(bla, get_direction(), 0.).rgb;
}
