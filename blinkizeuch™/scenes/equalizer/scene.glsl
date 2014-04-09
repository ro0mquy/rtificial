vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

void main(void){
	vec2 pos = gl_FragCoord.xy / res;
	float env = envelopes[int(pos.x * 32)];
	float note = float(notes[int(pos.x * 32)]) / 128;
	if(pos.y > .5) {
		out_color = vec3(1. - step(env, 2. * (pos.y - .5)));
	} else {
		out_color = vec3(1. - step(note, 2. * pos.y));
	}
}
