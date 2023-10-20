#ifndef RENDERER_H
#define RENDERER_H

#include "../utils/color.h"
#include "../utils/logger.h"
#include "../math/vector/vector2.h"
#include "../shader/shader.h"
#include "../camera/camera.h"
#include "shapes/renderer_shapes.h"

#define GLFW_INCLUDE_NONE
#include "../../../include/GLFW/include/GLFW/glfw3.h"
#include "../../../include/GL/include/GL/glew.h"

#include "../../../include/cglm/include/cglm/cglm.h"
#include "../../../include/cglm/include/cglm/mat4.h"
#include "../../../include/cglm/include/cglm/vec3.h"

struct shader* _renderer_default_rect_shader; 

// Compiles shaders for drawing
int renderer_init(void);
void renderer_free(void);

// Initializes, draws and frees a default rectangle 
// This uses the basic shader 
void renderer_init_rectangle(struct rect* rect);
void renderer_draw_rectangle(struct rect* rect, struct camera* camera);
void renderer_free_rectangle(struct rect* rect);

#endif

