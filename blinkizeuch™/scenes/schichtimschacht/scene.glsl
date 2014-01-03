vec2 f(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

#define MAT_SCHACHT 1

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	vec3 light = vec3(0, -7, 0);
	int i;
	vec3 hit = march(view_position, dir, i);
	if(i < 100){
		vec3 normal = calc_normal(hit);
		final_color = 0.1 * ao(hit, normal, .15, 5) + vec3(1.0) * max(dot(normalize(light - hit), normal), 0);
	}
	out_color = vec4(final_color, 1); 
} 

vec2 f(vec3 p){
	// schacht im schicht
	float schacht = -box(p, vec3(4,4,100));
	schacht = max(schacht, box(p, vec3(4.1, 4.1, 100.2)));
	vec3 q = domrep(trans(p, 4, 0, 0), 8, 1, 0.4 + 0.2);
	q.y = p.y;
	float schlitz = box(trans(q, 0,-4,0), vec3(3.8,0.3,.2));
	q = domrep(p, 4, 1, 4.2 * foo1 * 20);
	q.y = p.y;
	schlitz = max(schlitz, box(trans(q, 0, -4, 0), vec3(4., 0.3, 4.2)));
	return vec2(max(schacht,-schlitz), MAT_SCHACHT);
}
