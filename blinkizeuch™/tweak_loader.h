#ifndef TWEAK_LOADER_H
#define TWEAK_LOADER_H

#include "tweakable.h"
#include "texture.h"

int tweak_loader_load(const char filename[], tweakable_t** tweakables_out, texture_t** textures_out, int* texture_size_out);

#endif
