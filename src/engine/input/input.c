#include "input.h"

void input_key_callback(GLFWwindow* window, int key, int scancode, int action , int mods)
{
	if (action == GLFW_PRESS)
	{
		input_keys_pressed[key] = 1;
		input_keys_held[key] = 1;
	}
	else if (action == GLFW_RELEASE)
	{
		input_keys_pressed[key] = 0;
		input_keys_held[key] = 0;
	}
}

void input_reset_key_pressed_array(void)
{
	for (int i = 0; i < KEY_NUMBER; i++)
		input_keys_pressed[i] = 0;	
}

int input_get_key_down(int key)
{ return input_keys_pressed[key]; }
int input_get_key(int key)
{ return input_keys_held[key]; }

void input_hide_cursor(int lever, GLFWwindow* window)
{
	if (lever)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else 
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void input_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{ mouse_pos = (struct vector2){xpos, ypos}; }

struct vector2 input_get_mouse_pos(void)
{ return mouse_pos; }

