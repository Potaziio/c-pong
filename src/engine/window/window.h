#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include "../../../include/GL/include/GL/glew.h"
#include "../../../include/GLFW/include/GLFW/glfw3.h"
#include "../utils/color.h"
#include "../utils/logger.h"

#include "../input/input.h"

#define WINDOW_VSYNC_ON 0
#define WINDOW_VSYNC_OFF 2

struct window
{	
	struct GLFWwindow* glfw_window;
	int width, height;
	char* title;
	struct rgba_color color;
	int should_close;
	unsigned char config;
};

int window_create(struct window* window);
void window_clear(struct window* window);
int window_should_close(struct window* window);
void window_swap_buffers(struct window* window);
void window_poll_events(void);
void window_free_memory(struct window* window);
void window_end_frame(struct window* window);
double window_get_time(void);

#endif
