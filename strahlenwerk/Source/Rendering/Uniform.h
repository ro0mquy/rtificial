#ifndef UNIFORM_H
#define UNIFORM_H

#include "UniformType.h"

struct Uniform {
	Uniform(int id, UniformType type);

	int id;
	UniformType type;
};

#endif
