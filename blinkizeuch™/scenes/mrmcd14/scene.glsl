vec2 f(vec3);
vec4 df(vec3);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

vec3 c[19] = vec3[](
	vec3(1., 0., 0.),
	vec3(0., 1., 0.),
	vec3(0., 0., 1.),
	vec3(.577, .577, .577),
	vec3(-.577, .577, .577),
	vec3(.577, -.577, .577),
	vec3(.577, .577, -.577),
	vec3(0., .357, .934),
	vec3(0., -.357, .934),
	vec3(.934, 0., .357),
	vec3(-.934, 0., .357),
	vec3(.357, .934, 0.),
	vec3(-.357, .934, 0.),
	vec3(0., .851, .526),
	vec3(0., -.851, .526),
	vec3(.526, 0., .851),
	vec3(-.526, 0., .851),
	vec3(.851, .526, 0.),
	vec3(-.851, .526, 0.)
	);


void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);
	int i;
	vec3 hit = march(view_position, dir, i);
	//if(i < 100){
		vec3 normal = calc_normal(hit);
		//vec3 normal = normalize(df(hit).yzw);
		//float lamb = .95 * lambert(vec3(1., 2., 1.), normal) + .05;
		float roughness = foo1 + smooth_noise(10. * hit + 5. * sin(hit.x * 20.)) * .4;
		Material material = Material(vec3(0., .3, 1.), roughness, foo2);
		SphereLight light = SphereLight(vec3(10.), color_foo1, 2., 200.);
		SphereLight zweiteslight = SphereLight(vec3(2., -2., 8.), color_foo2, .5, 200.);
		final_color = apply_light(hit, normal, -dir, material, light);
		final_color += apply_light(hit, normal, -dir, material, zweiteslight);
		vec3 ambient_color = mix(color_foo1, color_foo2, normal.z * .5 + .5);
		final_color = mix(final_color, ambient_color, .01);
	//}
	out_color.rgb = final_color + 1./512. * vec3( // so schön weerboß
			classic_noise(gl_FragCoord.xy),
			classic_noise(gl_FragCoord.xy + 30.),
			classic_noise(gl_FragCoord.xy + 70.)
			);
}

// This will generate the distance function for some kind of spikeball.
// It's a bit magic - it's based on the paper "Generalized Distance Functions"
// - don't ask - play.
float spikeball(vec3 p){
	float l = length(p);
	p = normalize(p);
	float b = 0.;
	for (int i = 3; i < 19; i++)
		b = max(abs(dot(p, c[i])), b);
	b = 1. - acos(b - .01)/(acos(-1.) * .5);
	b = smoothstep(.78, 1., b);
	return l - 2.2 * pow(1.5, b);
}

vec2 f(vec3 p){
	//float fbox = roundbox(p, vec3(2.), .01);
	//float fbox = df(p).x;
	//float box1 = roundbox(p, vec3(2.), .1);
	//float box2 = roundbox(rX(radians(30.)) * trans(p, 3., 3., 3.), vec3(2.), .1);
	//float fbox = mix(box1, box2, sin(time * 25.) * .03 + .5);
	//float fbox = torus(rZ(radians(30.) * time) * rX(radians(20.) * time) * p, vec2(2., .5));
	//float fbox = sphere(p, 2.);
	//float fbox = scale(spikeball, trans(p, 0., 0., -5.), .5);
	float fbox = sphere(trans(p, 0., 0., -5.), 2.);
	fbox = min(fbox, -sphere(p, 50.));
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
