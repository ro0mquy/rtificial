#include "fragment.glsl"
#line 3

uniform float overlay_offset_x;
uniform float overlay_offset_y;
uniform float overlay_scale;
uniform float overlay_brightness;

layout(binding = 4) uniform sampler2D tex_rt_loves_all;

void main() {
	ivec2 texture_size = textureSize(tex_rt_loves_all, 0);
	vec2 tex_coords = gl_FragCoord.xy / res.xy;
	tex_coords.y = 1.-tex_coords.y;
	tex_coords.y *= texture_size.x / texture_size.y;
	tex_coords.x -= overlay_offset_x;
	tex_coords.y += overlay_offset_y * (texture_size.x / texture_size.y);
	tex_coords /= overlay_scale;
	vec4 tex_color = texture(tex_rt_loves_all, tex_coords);
	out_color = mix(vec3(0.), tex_color.rgb * overlay_brightness, tex_color.a);
	out_color *= exp2(camera_exposure_rt_float);
}
