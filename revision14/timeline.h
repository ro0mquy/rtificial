#define TIMELINE_DEFAULT_SCALE .5

typedef struct {
	struct keyframe_list_t* keyframes;
	struct keyframe_list_t* controlPoints;
} timeline_t;

typedef struct {
	int time;
	camera_t camera;
} keyframe_t;

typedef struct keyframe_list_t {
	size_t length;
	size_t allocated;
	keyframe_t elements[];
} keyframe_list_t;

static keyframe_list_t* list_insert(keyframe_list_t* list, keyframe_t frame, size_t position);
static keyframe_list_t* list_clear(keyframe_list_t* list);
static keyframe_t* list_get(keyframe_list_t* list, size_t position);
static size_t list_find(keyframe_list_t* list, int time);
static keyframe_list_t* timeline_get_bezier_spline(keyframe_list_t* controlPoints, keyframe_list_t* knots, float scale);

camera_t timeline_get_camera(timeline_t* const timeline, int time) {
	// interpolate and stuff
	const size_t pos = list_find(timeline->keyframes, time);
	if (pos == 0) {
		// if cursor_position is at the very first keyframe, just return it
		return list_get(timeline->controlPoints, 0)->camera;
	}

	const size_t nodeIndex = 3 * (pos - 1);
        keyframe_t* const p0 = list_get(timeline->controlPoints, nodeIndex);
        keyframe_t* const p1 = list_get(timeline->controlPoints, nodeIndex + 1);
        keyframe_t* const p2 = list_get(timeline->controlPoints, nodeIndex + 2);
        keyframe_t* const p3 = list_get(timeline->controlPoints, nodeIndex + 3);

	const float t = (time - p0->time) / (p3->time - p0->time);
	return camera_cubic_bezier(&p0->camera, &p1->camera, &p2->camera, &p3->camera, t);
}

static keyframe_list_t* list_insert(keyframe_list_t* list, const keyframe_t frame, const size_t position) {
	if(list->length == list->allocated) {
		keyframe_list_t* const new_list = realloc(list, sizeof(keyframe_list_t) + 2 * list->length * sizeof(keyframe_t));
		new_list->allocated *= 2;
		list = new_list;
	}
	memmove(list->elements + position + 1, list->elements + position, (list->length - position) * sizeof(keyframe_t));
	list->elements[position] = frame;
	list->length++;
	return list;
}

static keyframe_list_t* list_clear(keyframe_list_t* const list) {
	list->length = 0;
	return list;
}

static keyframe_t* list_get(keyframe_list_t* list, size_t position) {
	return &list->elements[position];
}

static size_t list_find(keyframe_list_t* list, int time) {
	for(size_t i = 0; i < list->length; i++) {
		if(list_get(list, i)->time >= time) {
			return i;
		}
	}
	return list->length;
}

//helper functions for quaternion bezier curves
// cut q0 - q1 in two halfs
static quat bisect(quat q0, quat q1) {
	return quat_normalize(quat_add(q0, q1));
}

// mirror q0 at q1
static quat mirror(quat q0, quat q1) {
	// lol, shoemake actually does work
	quat product = quat_s_mult(2 * quat_dot(q0, q1), q1);
	return quat_sub(product, q0);

}

// taken from http://devmag.org.za/2011/06/23/bzier-path-algorithms/
// quaternion from http://embots.dfki.de/doc/seminar_ca/Kremer_Quaternions.pdf
// http://caig.cs.nctu.edu.tw/course/CA/Lecture/slerp.pdf
static keyframe_list_t* timeline_get_bezier_spline(keyframe_list_t* controlPoints, keyframe_list_t* knots, float scale) {
	controlPoints = list_clear(controlPoints);
	for (uint i = 0; i < knots->length; i++) {
		if (i == 0) { // is first
			vec3 p1 = knots->elements[i].camera.position;
			quat v1 = knots->elements[i].camera.rotation;
			vec3 p2 = knots->elements[i+1].camera.position;
			quat v2 = knots->elements[i+1].camera.rotation;

			vec3 tangent = vec3_sub(p2, p1); // (p2 - p1)
			vec3 q1 = vec3_add(p1, vec3_s_mult(scale, tangent)); // p1 + scale * tangent

			quat a1 = bisect(v1, v2);

			// add p1
			controlPoints = list_insert(controlPoints, knots->elements[i], controlPoints->length);
			// add q1 and a1
			controlPoints = list_insert(controlPoints, (keyframe_t) {
					.time = 0,
					.camera = (camera_t) { .position = q1, .rotation = a1 }
					}, controlPoints->length);
		} else if (i == knots->length - 1) { // last
			vec3 p0 = knots->elements[i-1].camera.position;
			quat v0 = knots->elements[i-1].camera.rotation;
			vec3 p1 = knots->elements[i].camera.position;
			quat v1 = knots->elements[i].camera.rotation;

			vec3 tangent = vec3_sub(p1, p0); // (p1 - p0)
			vec3 q0 = vec3_sub(p1, vec3_s_mult(scale, tangent)); // p1 - scale * tangent

			quat b1 = bisect(v1, v0);

			// add q0 and b1
			controlPoints = list_insert(controlPoints, (keyframe_t) {
					.time = 0,
					.camera = (camera_t) { .position = q0, .rotation = b1 }
					}, controlPoints->length);
			// add p1
			controlPoints = list_insert(controlPoints, knots->elements[i], controlPoints->length);
		} else {
			vec3 p0 = knots->elements[i-1].camera.position;
			quat v0 = knots->elements[i-1].camera.rotation;
			vec3 p1 = knots->elements[i].camera.position;
			quat v1 = knots->elements[i].camera.rotation;
			vec3 p2 = knots->elements[i+1].camera.position;
			quat v2 = knots->elements[i+1].camera.rotation;

			vec3 tangent = vec3_normalize(vec3_sub(p2, p0)); // (p2 - p0).normalized
			vec3 q0 = vec3_sub(p1, vec3_s_mult(scale * vec3_length(vec3_sub(p1, p0)), tangent)); // p1 - scale * tangent * (p1 - p0).length
			vec3 q1 = vec3_add(p1, vec3_s_mult(scale * vec3_length(vec3_sub(p2, p1)), tangent)); // p1 + scale * tangent * (p2 - p1).length

			quat a1 = bisect(mirror(v0, v1), v2);
			quat b1 = mirror(a1, v1);


			// add q0 and b1
			controlPoints = list_insert(controlPoints, (keyframe_t) {
					.time = 0,
					.camera = (camera_t) { .position = q0, .rotation = b1 }
					}, controlPoints->length);
			// add p1
			controlPoints = list_insert(controlPoints, knots->elements[i], controlPoints->length);
			// add q1 and a1
			controlPoints = list_insert(controlPoints, (keyframe_t) {
					.time = 0,
					.camera = (camera_t) { .position = q1, .rotation = a1 }
					}, controlPoints->length);
		}
	}

	//curveCount = (controlPoints->length - 1) / 3;
	return controlPoints;
}
