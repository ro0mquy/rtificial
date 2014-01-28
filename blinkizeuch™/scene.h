#ifndef SCENE_H
#define SCENE_H

#include <AntTweakBar.h>

#include "tweakable.h"
#include "texture.h"

typedef struct {
	tweakable_t* tweakables;
	int num_tweakables;
	texture_t* textures;
	int num_textures;
} scene_t;

scene_t* scene_load(const char filename[]);

void scene_save(const scene_t* scene, char* path);

void scene_load_uniforms(scene_t* scene, GLuint program);

void scene_bind(const scene_t* scene);

void scene_add_to_tweakbar(const scene_t* scene, TwBar* tweak_bar);

void scene_destroy(scene_t* scene);

#endif
