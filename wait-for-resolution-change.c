#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>

// BE CAREFUL: No proper care has been put into this application. It's meant as
// a small utility to be run on machines I care nothing about the data inside
// them. Use it at your own risk.

typedef struct {
	int delay;
	int persist;
	char* script;
} config_t;

void parse_config(int argc, char** argv, config_t *config);
Display * prepare_display(int *width, int *height);
void wait_for_resolution_change(Display* display, int* width, int* height);

int main(int argc, char** argv) {
	int width, height;
	Display *display = prepare_display(&width, &height);

	config_t config;
	parse_config(argc, argv, &config);

	if (!config.persist) {
		wait_for_resolution_change(display, &width, &height);
	}
	else {
		time_t last_time = time(NULL);
		while(1) {
			wait_for_resolution_change(display, &width, &height);
			time_t new_time = time(NULL);
			if (new_time > last_time + config.delay) {
				last_time = new_time;
				int pid = fork();
				if (pid == -1) {
					fprintf(stderr, "Unable to fork process.\n");
					exit(5);
				}
				else if (pid == 0) {
					int status = execl(config.script, config.script);
					if (status == -1) {
						fprintf(stderr, "Unable to execute script.\n");
						exit(6);
					}
				}
			}
		}
	}

}

void parse_config(int argc, char** argv, config_t *config) {
	config->delay = 0;
	config->persist = 0;
	config->script = NULL;
	if (argc > 1) {
		if (argc != 3) {
			fprintf(stderr, "Wrong number of arguments. "
					"Expected 2 (delay and script), got %d.\n",
					argc - 1);
			exit(4);
		}
		config->persist = 1;
		config->delay = atoi(argv[1]);
		config->script = argv[2];
	}
}

void wait_for_resolution_change(Display* display, int* width, int* height) {
	while (1){
		XEvent e;
		XNextEvent (display, &e);
		if (e.type == ConfigureNotify){
			XConfigureEvent xce = e.xconfigure;
			if (xce.width != *width || xce.height != *height) {
/*				fprintf(stderr, "Current size: %dx%d; new size: %dx%d.\n",
						*width, *height, xce.width, xce.height); */
				*width = xce.width;
				*height = xce.height;
				return;
			}

		}
	}
}


Display * prepare_display(int *width, int *height) {
	Display *display = XOpenDisplay (NULL);
	if (!display) {
		fprintf(stderr, "Could not open display.\n");
		exit(1);
	}

	Window root = DefaultRootWindow(display);
	if (root < 0) {
		fprintf(stderr, "Unable to find default root window.\n");
		exit(2);
	}
		
	XSelectInput (display, root, StructureNotifyMask);
	XMapWindow (display, root);
	
	XWindowAttributes xwAttr;
	Status ret = XGetWindowAttributes( display, root, &xwAttr );
	if (ret == 0) {
		fprintf(stderr, "Unable to query current screen resolution.\n");
		exit(3);
	}

	*width = xwAttr.width;
    *height = xwAttr.height;
	return display;
}

