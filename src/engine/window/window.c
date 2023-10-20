#include "window.h"

void window_size_callback(GLFWwindow* window, int width, int height)
{
	struct window* w = (struct window*)glfwGetWindowUserPointer(window);
	w->width = width;
	w->height = height;

	glViewport(0.0f, width, 0.0f, -height);
}

int window_create(struct window* window)
{
	// Init glfw and check for errors

	if (!glfwInit())
	{
		logger_log_string(ERROR, "GLFW init failed.\n");	
		return 0;
	}

	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	/* glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE); */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	/* glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); */
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window->glfw_window = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);


	if (!window->glfw_window)
	{
		logger_log_string(ERROR, "Window creation failed.\n");	
		return 0;
	}

	glfwMakeContextCurrent(window->glfw_window);

	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		logger_log_string(ERROR, "GLEW init failed: -> \n\n");
		printf("%s\n", glewGetErrorString(err));
		return 0;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSwapInterval(window->config & WINDOW_VSYNC_ON ? 1 : window->config & WINDOW_VSYNC_OFF ? 0 : 1);

	glfwSetWindowUserPointer(window->glfw_window, window);
	glfwSetKeyCallback(window->glfw_window, input_key_callback);
	glfwSetWindowSizeCallback(window->glfw_window, window_size_callback);
	glfwSetCursorPosCallback(window->glfw_window, input_cursor_pos_callback);

	logger_log_string(LOG, "Window created\n\n");
	printf("Title: [\"%s\"]\nDimensions: [%d, %d]\nColor: [%d, %d, %d, %d]\n\n", window->title, window->width, window->height, RGBA_UNPACK(window->color));

	return 1;
}

void window_clear(struct window* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(RGBA_NORMALIZED_FLOAT(window->color));
}

void window_end_frame(struct window* window)
{
	input_reset_key_pressed_array();
	window_swap_buffers(window);
	window_poll_events();

	if (input_get_key_down(GLFW_KEY_ESCAPE))
		window->should_close = 1;
}

void window_swap_buffers(struct window* window)
{ glfwSwapBuffers(window->glfw_window); }

void window_poll_events(void)
{ glfwPollEvents(); }

int window_should_close(struct window* window)
{
	window->should_close = glfwWindowShouldClose(window->glfw_window);
	return window->should_close;
}

void window_free_memory(struct window* window)
{
	glfwDestroyWindow(window->glfw_window);
    glfwTerminate();
}

double window_get_time(void)
{ return glfwGetTime(); }


