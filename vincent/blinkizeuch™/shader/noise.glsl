varying vec2 pos;
uniform float aspect;
uniform float time;

/* Given a position, this function generates a 3D co-ordinates based,
 * reconstructible static noise. */
//float noise(vec3 position)
//{
//	position.x += position.y * 57. + position.z * 21.;
//	return sin(cos(position.x) * position.x);
//
//	/* The following is an alternative for the previous line:
//	 * return fract(position.x * position.x * .0013) * 2. - 1.; */
//}

/* Given a position, this function generates a 3D co-ordinates based,
 * reconstructible linearly interpolated smooth noise.
 *
 * This function uses the noise() function above for its underlying
 * noise texture. */
//float smooth_noise(vec3 position)
//{
//	vec3 integer = floor(position);
//	vec3 fractional = position - integer;
//
//	return mix(mix(mix(noise(integer),
//			   noise(integer + vec3(1, 0, 0)),
//			   fractional.x),
//		       mix(noise(integer + vec3(0, 1, 0)),
//			   noise(integer + vec3(1, 1, 0)),
//			   fractional.x),
//		       fractional.y),
//		   mix(mix(noise(integer + vec3(0, 0, 1)),
//			   noise(integer + vec3(1, 0, 1)),
//			   fractional.x),
//		       mix(noise(integer + vec3(0, 1, 1)),
//			   noise(integer + 1.), fractional.x),
//		       fractional.y),
//		   fractional.z) * .5 + .5;
//}

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 fade(vec2 t) {
	return t*t*t*(t*(t*6.0-15.0)+10.0);
}

vec2 g(float v) {
	v *= 2.0 * 3.1415926;
	return vec2(cos(v), sin(v));
}

float classic_noise(vec2 co) {
	vec2 c = fract(co);
	vec2 C = floor(co);
	vec2 e = vec2(0.0, 1.0);

	vec4 n = vec4(
		dot(g(rand(C + e.xx)), c - e.xx),
		dot(g(rand(C + e.xy)), c - e.xy),
		dot(g(rand(C + e.yx)), c - e.yx),
		dot(g(rand(C + e.yy)), c - e.yy)
	);

	vec2 u = fade(c);

	return mix(
		mix(n[0], n[2], u.x),
		mix(n[1], n[3], u.x),
		u.y);
}


/* Given a position, this function constructs the oh-so-famous Perlin
 * noise. */
float perlin(vec2 position)
{
	return classic_noise(position * .06125) * .5 +
	       classic_noise(position * .125) * .25 +
	       classic_noise(position * .25) * .125;
}

float sdPlane(vec3 p, vec3 n) {
	return dot(p, n);
}

float sdBox(vec3 p, vec3 b) {
	vec3 d = abs(p) - b;
	return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

vec3 rX(vec3 p, float theta) {
	return mat3(
		1., 0., 0.,
		0., cos(theta), sin(theta),
		0., -sin(theta), cos(theta)
	) * p;
}

vec3 rY(vec3 p, float theta) {
	return mat3(
		cos(theta), 0., -sin(theta),
		0., 1., 0.,
		sin(theta), 0., cos(theta)
	) * p;
}

vec3 rZ(vec3 p, float theta) {
	return mat3(
		cos(theta), sin(theta), 0.,
		-sin(theta), cos(theta), 0.,
		0., 0., 1.
	) * p;
}

float f(vec3 p) {
	p.y += 5.0;
	p.z += time;
	return sdPlane(p, vec3(0.0, 1.0, 0.0)) - 5.0 * perlin(p.xz * 3.0);
}

void main(void) {
	vec3 ray_start = vec3(0.0, 0.0, 1.0 / tan(90.0/2.0 / 180.0 * 3.14));
	vec3 ray_dir = normalize(vec3(pos.x, pos.y/aspect, 0.0) - ray_start);

	vec3 p = ray_start;
	for(int i = 0; i < 500; i++) {
		float dist = f(p);
		if(abs(dist) < 0.0001) {
			break;
		}
		p += 0.4 * dist * ray_dir;
	}

	vec3 color = vec3(0.2, 1.0, 0.);
	color += 0.2 * pow(sin(time / 1000. * 3.), 2.);

	if(length(ray_start - p) < 50.0) {
		float e = 0.01;
		vec3 normal = normalize(vec3(
			f(p + vec3(e, 0, 0)) - f(p - vec3(e, 0, 0)),
			f(p + vec3(0, e, 0)) - f(p - vec3(0, e, 0)),
			f(p + vec3(0, 0, e)) - f(p - vec3(0, 0, e))
		));
		vec3 light_ray = vec3(0.4, 3., 4.) - p;
		light_ray = normalize(light_ray);
		float lambert = dot(normal, light_ray);
		color *= lambert;
		float k = 0.15;
		for(float i = 0.; i < 5.; i++) {
			color += vec3(i * k - f(p + normal * i * k)) / pow(2., i);
		}
	} else {
		color = vec3(0.0);
	}

	gl_FragColor = vec4(color, 1.0);
}

