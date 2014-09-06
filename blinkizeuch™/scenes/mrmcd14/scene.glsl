vec2 f(vec3);
vec4 df(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	if(i < 100){
		vec3 normal = calc_normal(hit);
		//vec3 normal = normalize(df(hit).yzw);
		//float lamb = .95 * lambert(vec3(1., 2., 1.), normal) + .05;
		Material material = Material(vec3(0., .3, 1.), foo1, foo2);
		SphereLight light = SphereLight(vec3(10.), vec3(1.), 2., 200.);
		SphereLight zweiteslight = SphereLight(vec3(2., -1., -8.), vec3(1.), .5, 200.);
		final_color = apply_light(hit, normal, -dir, material, light);
		//final_color += apply_light(hit, normal, -dir, material, zweiteslight);
		vec3 ambient_color = mix(color_foo1, color_foo2, normal.z * .5 + .5);
		final_color = mix(final_color, ambient_color, .01);
	}
	out_color.rgb = final_color;
}

vec2 f(vec3 p){
	//float fbox = roundbox(p, vec3(2.), .01);
	//float fbox = df(p).x;
	//float box1 = roundbox(p, vec3(2.), .1);
	//float box2 = roundbox(rX(radians(30.)) * trans(p, 3., 3., 3.), vec3(2.), .1);
	//float fbox = mix(box1, box2, sin(time * 25.) * .03 + .5);
	//float fbox = torus(rZ(radians(30.) * time) * rX(radians(20.) * time) * p, vec2(2., .5));
	float fbox = sphere(p, 2.);
	return vec2(fbox, 1);
}

vec4 dabs(vec4 d) {
	float derabs = (d.x < 0) ? -1. : 1.;
	return vec4(abs(d.x), derabs * d.y, derabs * d.z, derabs * d.w);
}

vec4 dsqrt(vec4 d) {
	float dersqrt = 1. / (2. * sqrt(d.x));
	d.x = sqrt(d.x);
	d.yzw *= dersqrt;
	return d;
}

vec4 dmult(vec4 d1, vec4 d2) {
	return vec4(
			d1.x * d2.x,
			//d1.x * d2.yzw + d1.yzw * d2.x,
			d1.x * d2.y + d1.y * d2.x,
			d1.x * d2.z + d1.z * d2.x,
			d1.x * d2.w + d1.w * d2.x
			);
}

vec4 dlength(vec4 x, vec4 y, vec4 z) {
	return dsqrt(dmult(x, x) + dmult(y, y) + dmult(z, z));
}

vec4 dmax(vec4 d1, vec4 d2) {
	return (d1.x >= d2.x) ? d1 : d2;
}

vec4 dmin(vec4 d1, vec4 d2) {
	return (d1.x <= d2.x) ? d1 : d2;
}

vec4 dbox(vec4 x, vec4 y, vec4 z, vec3 bounds) {
	vec4 xabs = dabs(x);
	vec4 yabs = dabs(y);
	vec4 zabs = dabs(z);

	// - b
	xabs.x -= bounds.x;
	yabs.x -= bounds.y;
	zabs.x -= bounds.z;

	return dmax(xabs, dmax(yabs, zabs));
}

vec4 dslowbox(vec4 x, vec4 y, vec4 z, vec3 bounds) {
	vec4 xabs = dabs(x);
	vec4 yabs = dabs(y);
	vec4 zabs = dabs(z);

	// - b
	xabs.x -= bounds.x;
	yabs.x -= bounds.y;
	zabs.x -= bounds.z;

	vec4 minmax = dmin(dmax(xabs, dmax(yabs, zabs)), vec4(0.));
	vec4 lengthmax = dlength(dmax(xabs, vec4(0.)), dmax(yabs, vec4(0.)), dmax(zabs, vec4(0.)));
	return minmax + lengthmax;
}

vec4 df(vec3 p) {
	vec4 x = vec4(p.x, 1., 0., 0.);
	vec4 y = vec4(p.y, 0., 1., 0.);
	vec4 z = vec4(p.z, 0., 0., 1.);

	return dslowbox(x, y, z, vec3(2.)) - vec4(.1, 0., 0., 0.);
}
