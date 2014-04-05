vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

uniform samplerCube environment;
uniform mat3 rotation;
uniform vec3 light;

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	if(i < 100 && f(hit)[1] == 1.) {
		vec3 normal = calc_normal(hit);
		final_color = mix(texture(environment, reflect(dir, normal)).rgb, vec3(lambert(light, normal)), foo1);
		//final_color = texture(environment, normal).rgb;
	} else {
		final_color = texture(environment, dir).rgb;
	}
	out_color = final_color;
}

vec2 f(vec3 p){
	return vec2(torus(rotation * p, vec2(4., 1)),1);
}
