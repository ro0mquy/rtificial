#ifndef UNIFORM_H
#define UNIFORM_H

#include <string>

#include "UniformType.h"

struct Uniform {
	Uniform(int id, std::string name, UniformType type);

	int id;
	std::string name;
	UniformType type;
};

#endif
