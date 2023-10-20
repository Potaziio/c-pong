#ifndef PHYSICS_SHAPE_H
#define PHYSICS_SHAPE_H

#include "../../vector/vector2.h"
#include "../../vector/vector3.h"

// For collision stuff
struct aabb
{
	int active;

	struct vector3 position;
	struct vector3 scale;

	// We dont care about rotatation just yet, not implemented
	struct vector3 rotation;
	float angle;
};



#endif
