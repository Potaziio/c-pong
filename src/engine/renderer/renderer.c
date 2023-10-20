#include "renderer.h"
#include "shapes/renderer_shapes.h"

extern struct shader* _renderer_default_rect_shader;

int renderer_init(void)
{
	// Allocate memory, load and compile our default shader
	_renderer_default_rect_shader = (struct shader*)malloc(sizeof(struct shader));

	if (_renderer_default_rect_shader == NULL) 
	{
		logger_log_string(ERROR, "Could not allocate memory for '_renderer_default_rect_shader'\n");
		return 0;
	}

	int res = shader_load_and_compile(_renderer_default_rect_shader, "../src/engine/assets/shaders/default_vertex.glsl", "../src/engine/assets/shaders/default_fragment.glsl");

	if (!res) 
	{
		logger_log_string(ERROR, "Could not compile '_renderer_default_rect_shader'\n");
		return 0;
	}

	return 1;
}

void renderer_free(void)
{
	shader_free_memory(_renderer_default_rect_shader);
	free(_renderer_default_rect_shader);
}

void renderer_init_rectangle(struct rect* rect)
{
	if (rect->initialized) return;

	// If we havent specified a configuration the default is to originate it at the center
	if (!rect->config) rect->config = RECT_ORIGIN_CENTER;

	if (rect->config & RECT_ORIGIN_CENTER)
	{
		rect->vertices[0].position = (struct vector3){-0.5f, -0.5f, 0.0f};
		rect->vertices[1].position = (struct vector3){0.5f, -0.5f, 0.0f};
		rect->vertices[2].position = (struct vector3){-0.5f, 0.5f, 0.0f};
		rect->vertices[3].position = (struct vector3){0.5f, 0.5f, 0.0f};
	} else if (rect->config & RECT_ORIGIN_TOP_LEFT) 
 	{
		rect->vertices[0].position = (struct vector3){0.0f, 0.0f, 0.0f};
		rect->vertices[1].position = (struct vector3){1.0f, 0.0f, 0.0f};
		rect->vertices[2].position = (struct vector3){0.0f, 1.0f, 0.0f};
		rect->vertices[3].position = (struct vector3){1.0f, 1.0f, 0.0f};
	}

	for (int i = 0; i < 4; i++)
	{
		rect->vertices[i].color = (struct rgba_color){RGBA_NORMALIZED(rect->color)};
	}
	
	rect->indices[0] = 0;
	rect->indices[1] = 1; 
	rect->indices[2] = 2;
	rect->indices[3] = 2;
	rect->indices[4] = 3;
	rect->indices[5] = 1;
	
	glGenVertexArrays(1, &rect->vao);
	glBindVertexArray(rect->vao);
	glGenBuffers(1, &rect->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rect->vbo);
	glGenBuffers(1, &rect->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect->ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(struct rect_vertex) * 4, rect->vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, rect->indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct rect_vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_BYTE, GL_FALSE, sizeof(struct rect_vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	rect->initialized = 1;
}

void renderer_draw_rectangle(struct rect* rect, struct camera* camera)
{
	if (rect->config & RECT_DRAW_MODE_WIREFRAME)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm_mat4_identity(rect->transform);
	glm_translate(rect->transform, (vec3){rect->position.x, rect->position.y, rect->position.z});
	glm_rotate(rect->transform, glm_rad(rect->rotation_angle), (vec3){rect->rotation.x, rect->rotation.y, rect->rotation.z});
	glm_scale(rect->transform, (vec3){rect->scale.x, rect->scale.y, rect->scale.z});

	if (!_renderer_default_rect_shader->being_used) shader_use(_renderer_default_rect_shader);

	shader_send_mat4(_renderer_default_rect_shader, rect->transform, "transform");
	camera_send_matrices_to_shader(camera, _renderer_default_rect_shader, "projection", "view");

	glBindVertexArray(rect->vao);
	glBindBuffer(GL_ARRAY_BUFFER, rect->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect->ebo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* shader_detach(_renderer_default_rect_shader); */

	if (rect->config & RECT_DRAW_MODE_WIREFRAME)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void renderer_free_rectangle(struct rect* rect)
{
	glDeleteBuffers(1, &rect->vbo);
	glDeleteBuffers(1, &rect->ebo);
	glDeleteVertexArrays(1, &rect->vao);
}

