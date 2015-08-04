#version 430
#line 3 "fragment"

// main output color
out vec3 out_color;
out float out_depth;

// common uniforms
layout(location = 0) uniform vec2 res;
uniform float time;
