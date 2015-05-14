#line 2 "quat_rotate"

// TODO figure out how to get rid of this special include file

// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
void pQuatRotate(inout vec3 v, vec4 q) {
	vec3 t = 2 * cross(q.xyz, v);
	v += q.w * t + cross(q.xyz, t);
	// *hex hex*
}
