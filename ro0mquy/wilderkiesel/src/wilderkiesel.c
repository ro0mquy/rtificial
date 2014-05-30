#include <pebble.h>
#include <fix16.h>
#include <fixvector3d.h>

#define res_x 144
#define res_y 168

static const uint16_t time_between_frames = 16;

static Window *window;
static Layer *canvas_layer;
static AppTimer *redraw_timer;
static time_t startup_time;

static int8_t framebuffer[2][res_x]; // buffer for two lines of pixels
static uint8_t bitmap_data_framebuffer[res_y][20]; // buffer for bitmap data, width has to be % 4 == 0
static GBitmap bitmap_framebuffer;

static uint8_t center_z = 2;

static const fix16_t fix16_two = F16(2);
static const fix16_t f16_res_x = F16(res_x);
static const fix16_t f16_res_y = F16(res_y);
static const fix16_t f16_res_x_2 = F16(res_x / 2);
static const fix16_t f16_res_y_2 = F16(res_y / 2);

static const v3d camera_pos = { .x = 0, .y = 0, .z = 0 };

static uint8_t fragment_draw(size_t x, size_t y, uint32_t ms) {
	uint16_t out_color = 0;
	fix16_t f16_x = fix16_from_int(x);
	fix16_t f16_y = fix16_from_int(y);

	v3d center = { .x = fix16_from_int(0), .y = fix16_from_int(0), .z = fix16_from_int(-center_z) };
	fix16_t radius = fix16_from_int(1);

	v3d direction = {
		.x = fix16_div(fix16_sub(f16_x, f16_res_x_2), f16_res_y),
		.y = fix16_div(fix16_sub(f16_y, f16_res_y_2), f16_res_y),
		.z = fix16_from_int(-1)
	};
	v3d_normalize(&direction, &direction);

	v3d relativ;
	v3d_sub(&relativ, &camera_pos, &center);
	// in case that camera is always at (0, 0, 0)
	// intersection point formula must be changed
	//v3d relativ = center;

	fix16_t discr1 = fix16_sq(v3d_dot(&relativ, &direction));
	fix16_t discr2 = fix16_sub(v3d_dot(&relativ, &relativ), fix16_sq(radius));
	fix16_t discriminant = fix16_sub(discr1, discr2);

	if (discriminant < 0) {
		// no solution, no hit
		out_color = 0;
	} else {
		// we hit something
		fix16_t plus_b = v3d_dot(&relativ, &direction);
		discriminant = fix16_sqrt(discriminant);

		if (plus_b < discriminant) {
			// -b + sqrt(b^2 - 4ac) is bigger than 0

			// now get smallest distance and hit point
			fix16_t hit_dist = fix16_add(-plus_b, discriminant);
			v3d hit_pos;
			v3d_mul_s(&hit_pos, &direction, hit_dist);
			v3d_add(&hit_pos, &camera_pos, &hit_pos);

			// calculate normal
			v3d normal;
			v3d_sub(&normal, &hit_pos, &center);
			v3d_normalize(&normal, &normal);

			// lambertian lighting
			v3d to_light = { fix16_from_float(.7071), 0, -fix16_from_float(.7071) };
			fix16_t lambert = v3d_dot(&normal, &to_light);
			lambert = fix16_clamp(lambert, 0, fix16_one);
			out_color = fix16_to_int(fix16_mul(lambert, F16(255)));
		} else {
			// all hitpoints are behind the camera
			out_color = 0;
		}
	}

	return out_color;
}

static void render(uint32_t milliseconds) {
	memset(framebuffer, 0, 2 * res_x);
	uint8_t active_line = 0;
	for (size_t y = 0; y < res_y; y++) {
		for (size_t x = 0; x < res_x; x++) {
			// get color of pixel
			// color from fragment_draw() is between 0 and 255
			// range for dithering is between -128 and 127
			int16_t pixel_color = framebuffer[active_line][x];
			pixel_color += fragment_draw(x, y, milliseconds) - 128;
			int16_t dithered_color = pixel_color / abs(pixel_color);
			int16_t dithering_error = pixel_color - dithered_color * 127;
			framebuffer[active_line][(x+1 + res_x) % res_x] += dithering_error * 7 / 16;
			framebuffer[active_line ^ 1][(x-1 + res_x) % res_x] += dithering_error * 3 / 16;
			framebuffer[active_line ^ 1][x] += dithering_error * 5 / 16;
			framebuffer[active_line ^ 1][(x+1) % res_x] += dithering_error * 1 / 16;

			// write to bitmap
			size_t byte = x / 8;
			size_t bit = x % 8;
			int8_t mask = 1 << bit;
			int8_t bw_dithered_color = (dithered_color + 1) / 2;

			// modify bit of bitmap, yay bitoperations
			bitmap_data_framebuffer[y][byte] = (bitmap_data_framebuffer[y][byte] & ~mask) | (-bw_dithered_color & mask);
		}
		memset(framebuffer[active_line], 0, res_x);
		active_line ^= 1;
	}
}

static void canvas_draw(Layer* layer, GContext* context) {
	GRect bounds = layer_get_bounds(layer);
	time_t seconds;
	uint16_t mseconds;
	time_ms(&seconds, &mseconds);
	seconds -= startup_time;
	uint32_t milliseconds = seconds * 1000 + mseconds;

	render(milliseconds);
	graphics_draw_bitmap_in_rect(context, &bitmap_framebuffer, bounds);
}

static void redraw_task(void *data) {
	layer_mark_dirty(canvas_layer);
	redraw_timer = app_timer_register(time_between_frames, redraw_task, NULL);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	center_z++;
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	canvas_layer = layer_create(bounds);
	layer_set_update_proc(canvas_layer, canvas_draw);
	layer_add_child(window_layer, canvas_layer);
}

static void window_unload(Window *window) {
	layer_destroy(canvas_layer);
}

static void init(void) {
	window = window_create();
	window_set_click_config_provider(window, click_config_provider);
	window_set_window_handlers(window, (WindowHandlers) {
			.load = window_load,
			.unload = window_unload,
			});
	window_set_fullscreen(window, true);
	const bool animated = true;
	window_stack_push(window, animated);
	bitmap_framebuffer = (GBitmap) { .addr = bitmap_data_framebuffer, .bounds = GRect(0, 0, res_x, res_y), .row_size_bytes = 20 };
	time(&startup_time);
	redraw_timer = app_timer_register(time_between_frames, redraw_task, NULL);
}

static void deinit(void) {
	window_destroy(window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
