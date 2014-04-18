#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/select.h>

int main() {

	Display * display;
	int screen;
	Window root, window;

	display = XOpenDisplay (NULL);
	if (!display) {
		puts("Could not open display.");
		return 1;
	}
	screen = DefaultScreen(display);
	root = RootWindow(display, screen);

	window = DefaultRootWindow(display);
	if (window < 0) {
		puts("Unable to find default root window.");
		return 1;
	}
		
	// Add StructureNotifyMask to send us events involving resizing of the window, etc.
	XSelectInput (display, window, StructureNotifyMask);
	XMapWindow (display, window);
	XWindowAttributes xwAttr;
	Status ret = XGetWindowAttributes( display, window, &xwAttr );
	int width = xwAttr.width;
    int height = xwAttr.height;
	
	int count = 0;

	int xfd = ConnectionNumber(display);
	int first = 1;
	while (1){
		XEvent e;
		XNextEvent (display, &e);

		// Respond to ConfigureNotify, the type of event sent by the
		// server if the window is resized.
		if (e.type == ConfigureNotify){
			XConfigureEvent xce = e.xconfigure;

			/* This event type is generated for a variety of
			   happenings, so check whether the window has been
			   resized. */
			if (xce.width != width || xce.height != height) {
				return 0;
			}

		}
	}
}

