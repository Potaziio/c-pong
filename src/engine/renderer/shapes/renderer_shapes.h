#ifndef RENDERER_SHAPES_H
#define RENDERER_SHAPES_H

#include "../../utils/color.h"
#include "../../math/vector/vector2.h"
#include "../../math/vector/vector3.h"

#include "../../../../include/cglm/include/cglm/mat4.h"

#define RECT_ORIGIN_CENTER 1
#define RECT_ORIGIN_TOP_LEFT 2
#define RECT_DRAW_MODE_WIREFRAME 4
#define RECT_DRAW_MODE_FILL 8

struct rect_vertex
{
    struct vector3 position;
    struct rgba_color color;
};

struct rect 
{
    unsigned int vao, vbo, ebo;

    mat4 transform;
    struct vector3 position;
    struct vector3 scale;
    struct vector3 rotation;
    float rotation_angle;

    struct rgba_color color;
    unsigned char config;

    struct rect_vertex vertices[4];
    unsigned int indices[6];

    unsigned char initialized;
};

#endif

