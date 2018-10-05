#version 130
uniform int m;
out vec4 o;
float t = m/float(44100);
vec2 res = vec2(1280,720);
void main(){
	vec2 pos = gl_FragCoord.xy/res * 2. - 1.;
	pos.x *= res.x/res.y;

	vec4 color = vec4(1.);
	if (length(pos) < 0.5) {
		color = vec4(1.,0.,0.,1.);
	}
	o = color;
}