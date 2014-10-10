#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

void DrawAQuad() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1., 1., -1., 1., 1., 20.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

	glBegin(GL_QUADS);
	glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
	glColor3f(0., 1., 0.); glVertex3f( .75, -.75, 0.);
	glColor3f(0., 0., 1.); glVertex3f( .75,  .75, 0.);
	glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);
	glEnd();
}

int main() {
	// get local X display
	Display* x_display;
	x_display = XOpenDisplay(NULL);
	if(NULL == x_display){
		puts("error: cannot connect to X server");
		exit(1);
	}

	// get root window
	Window x_root_window;
	x_root_window = DefaultRootWindow(x_display);

	// set the visual attributes required for the OpenGL context
	GLint gl_attributes[] = {
		GLX_RGBA,
		GLX_DEPTH_SIZE,
		24,
		GLX_DOUBLEBUFFER,
		None
	};
	XVisualInfo* x_visual_info;
	x_visual_info = glXChooseVisual(x_display, 0, gl_attributes);
	if(NULL == x_visual_info){
		puts("error: no visual found");
	} else {
		 printf("visual %p selected\n", (void*) x_visual_info->visualid);
	}

	// create color map for the window
	Colormap x_color_map;
	x_color_map = XCreateColormap(x_display, x_root_window, x_visual_info->visual, AllocNone);

	// initialize XSetWindowAttributes
	XSetWindowAttributes x_set_window_attributes;
	x_set_window_attributes.colormap = x_color_map;
	x_set_window_attributes.event_mask = ExposureMask | KeyPressMask;

	// create window
	Window x_window;
	x_window = XCreateWindow(
			/* display       */ x_display,
			/* parent window */ x_root_window,
			/* initial x     */ 0,
			/* initial y     */ 0,
			/* width         */ 600,
			/* height        */ 600,
			/* border        */ 0,
			/* depth         */ x_visual_info->depth,
			/* window type   */ InputOutput,
			/* visual info   */ x_visual_info->visual,
			/* win-attrs set */ CWColormap | CWEventMask,
			/* XSetWinAttr   */ &x_set_window_attributes
			);

	// make the window appear
	XMapWindow(x_display, x_window);

	XStoreName(x_display, x_window, "Xlib/OpenGL Sample Application");

	// create OpenGL context
	GLXContext gl_context;
	gl_context = glXCreateContext(x_display, x_visual_info, NULL, GL_TRUE);
	glXMakeCurrent(x_display, x_window, gl_context);
	glEnable(GL_DEPTH_TEST);


	// main loop
	XEvent x_event;
	while (1) {
		XNextEvent(x_display, &x_event);
		if(Expose == x_event.type) {
			XWindowAttributes x_window_attributes;
			XGetWindowAttributes(x_display, x_window, &x_window_attributes);
			glViewport(0, 0, x_window_attributes.width, x_window_attributes.height);
			DrawAQuad();
			glXSwapBuffers(x_display, x_window);
		} else if (KeyPress == x_event.type) {
			glXMakeCurrent(x_display, None, NULL);
			glXDestroyContext(x_display, gl_context);
			XDestroyWindow(x_display, x_window);
			XCloseDisplay(x_display);
			exit(0);
		}
	}
}
