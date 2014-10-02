#ifndef UNIFORM_H
#define UNIFORM_H

#define UNIFORM_TYPE_FLOAT 0
#define UNIFORM_TYPE_VEC2 1
#define UNIFORM_TYPE_VEC3 2
#define UNIFORM_TYPE_VEC4 3
#define UNIFORM_TYPE_COLOR 4
#define UNIFORM_TYPE_BOOL 5

struct Uniform {
	int type;
	int location;
};

#endif
