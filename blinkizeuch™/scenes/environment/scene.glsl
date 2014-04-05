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
uniform float specular_lod;

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	if(i < 100 && f(hit)[1] == 1.) {
		vec3 normal = calc_normal(hit);
		vec3 reflect_dir = reflect(dir, normal);
		float lod = max(specular_lod, textureQueryLod(environment, reflect_dir).y);
		final_color = textureLod(environment, reflect_dir, lod).rgb + cook_torrance(light, normal, -dir, .45, 450.);
	} else {
		final_color = texture(environment, dir).rgb;
	}
	out_color = final_color;
}

vec2 f(vec3 p){
	return vec2(torus(rotation * p, vec2(4., 1)),1);
}
