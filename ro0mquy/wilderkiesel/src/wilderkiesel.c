#include <pebble.h>
#include "libfixmath/fix16.h"

#define fb_w 144
#define fb_h 168

static Window *window;
static Layer *canvas_layer;
static AppTimer *redraw_timer;
static time_t startup_time;

static int8_t framebuffer[2][fb_w]; // buffer for two lines of pixels
static uint8_t bitmap_data_framebuffer[fb_h][20]; // buffer for bitmap data, width has to be % 4 == 0
static GBitmap bitmap_framebuffer;

static uint8_t fragment_draw(size_t x, size_t y, uint32_t ms) {
	uint16_t out_color = 0;

	/*
	size_t pos_y = (y + ms / 10) % fb_h;
	pos_y = abs(pos_y - fb_h / 2) * 2;

	out_color = pos_y * 255 / fb_h;
	*/

	fix16_t cx = fix16_div(fix16_from_int(10*x), fix16_from_int(fb_w));
	fix16_t cy = fix16_div(fix16_from_int(10*y), fix16_from_int(fb_h));
	fix16_t time = fix16_div(fix16_from_int(ms), fix16_from_int(10000));
	fix16_t v = 0;
	fix16_t fix16_two = F16(2);

	v = fix16_add(v, fix16_sin(fix16_add(cx, time)));
	v = fix16_add(v, fix16_sin(fix16_div(fix16_add(cy, time), fix16_two)));
	v = fix16_add(v, fix16_sin(fix16_div(fix16_add(fix16_add(cx, cy), time), fix16_two)));

	cx = fix16_add(cx, fix16_sin(fix16_div(time, fix16_from_int(3))));
	cy = fix16_add(cy, fix16_cos(fix16_div(time, fix16_two)));

	v = fix16_add(v, fix16_sin(fix16_add(fix16_sqrt(fix16_add(fix16_add(fix16_sq(cx), fix16_sq(cy)), fix16_one)), time)));
	v = fix16_div(v, fix16_two);

	fix16_t color = fix16_div(fix16_add(fix16_sin(fix16_mul(fix16_pi, v)), fix16_one), fix16_two);
	color = fix16_mul(color, fix16_from_int(256));
	out_color = fix16_to_int(color);

	/*
	float cx = (float) x / fb_w;
	float cy = (float) y / fb_h;
	float time = ms / 1000.;
	float v = 0.;
	v += sin(cx+time);
	v += sin((cy+time)/2.0);
	v += sin((cx+cy+time)/2.0);
	cx += sin(time/3.0);
	cy += cos(time/2.0);
	v += sin(sqrt(cx*cx+cy*cy+1.0)+time);
	v = v/2.0;
	out_color = sin(3.14159*v) * .5 + .5;
	 */

	return out_color;
}

static void render(uint32_t milliseconds) {
	memset(framebuffer, 0, 2 * fb_w);
	uint8_t active_line = 0;
	for (size_t y = 0; y < fb_h; y++) {
		for (size_t x = 0; x < fb_w; x++) {
			// get color of pixel
			// color from fragment_draw() is between 0 and 255
			// range for dithering is between -128 and 127
			int16_t pixel_color = framebuffer[active_line][x];
			pixel_color += fragment_draw(x, y, milliseconds) - 128;
			int16_t dithered_color = pixel_color / abs(pixel_color);
			int16_t dithering_error = pixel_color - dithered_color * 127;
			framebuffer[active_line][(x+1 + fb_w) % fb_w] += dithering_error * 7 / 16;
			framebuffer[active_line ^ 1][(x-1 + fb_w) % fb_w] += dithering_error * 3 / 16;
			framebuffer[active_line ^ 1][x] += dithering_error * 5 / 16;
			framebuffer[active_line ^ 1][(x+1) % fb_w] += dithering_error * 1 / 16;

			// write to bitmap
			size_t byte = x / 8;
			size_t bit = x % 8;
			int8_t mask = 1 << bit;
			int8_t bw_dithered_color = (dithered_color + 1) / 2;

			// modify bit of bitmap, yay bitoperations
			bitmap_data_framebuffer[y][byte] = (bitmap_data_framebuffer[y][byte] & ~mask) | (-bw_dithered_color & mask);
		}
		memset(framebuffer[active_line], 0, fb_w);
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
	redraw_timer = app_timer_register(16, redraw_task, NULL);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
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
	bitmap_framebuffer = (GBitmap) { .addr = bitmap_data_framebuffer, .bounds = GRect(0, 0, fb_w, fb_h), .row_size_bytes = 20 };
	time(&startup_time);
	redraw_timer = app_timer_register(16, redraw_task, NULL);
}

static void deinit(void) {
	window_destroy(window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
