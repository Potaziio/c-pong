#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../../include/stb_image/stb_image.h"
#include "../../../include/stb_image/stb_image_write.h"
#include "../../../include/stb_image/stb_image_resize2.h"

#define GLFW_INCLUDE_NONE
#include "../../../include/GLFW/include/GLFW/glfw3.h"
#include "../../../include/GL/include/GL/glew.h"

#include "../utils/logger.h"

struct texture
{
	int width, height, channels;
	unsigned char* data;
	unsigned int id;
	const char* file_path;
};

void texture_load(struct texture* texture, const char* path);
unsigned int texture_get_id(struct texture texture);
void texture_bind(struct texture texture);
void texture_unbind(void);
const char* texture_get_path(struct texture texture);
unsigned char* texture_get_data(struct texture texture);
int texture_get_width(struct texture texture);
int texture_get_height(struct texture texture);
void texture_free(struct texture* texture);
int texture_is_good(struct texture* texture);

#endif
