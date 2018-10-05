#version 130
uniform sampler2D o;
out vec4 i;
void main(){
	i = vec4(0);
	vec2 uv = gl_FragCoord.xy/vec2(1280,720);
	i = textureLod(o, uv, 0.);
}