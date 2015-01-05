#version 400

layout(lines_adjacency) in;
layout(triangle_strip, max_vertices = 4) out;

in vec2 tex_position[];
out vec2 position;
out float intensity;

void main() {
	vec2 a = gl_in[0].gl_Position.xy;
	vec2 b = gl_in[1].gl_Position.xy;
	vec2 c = gl_in[2].gl_Position.xy;
	vec2 d = gl_in[3].gl_Position.xy;

	float area = 0.;
	area += .5 * length(cross(vec3(b - a, 0.), vec3(c - a, 0.)));
	area += .5 * length(cross(vec3(b - d, 0.), vec3(c - d, 0.)));
	intensity = 4./ (area + .01);
	intensity *= .05;
	intensity *= 30.; // debug
	gl_Position = gl_in[0].gl_Position;
	position = tex_position[0];
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	position = tex_position[1];
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	position = tex_position[2];
	EmitVertex();
	gl_Position = gl_in[3].gl_Position;
	position = tex_position[3];
	EmitVertex();
	EndPrimitive();
}
