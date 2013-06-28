attribute vec2 coord2d;
varying vec2 pos;

void main(void) {
	pos = coord2d;
	gl_Position = vec4(coord2d, 0.0, 1.0);
}
