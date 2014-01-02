#include <string.h>
#include <stdlib.h>

#include <jansson.h>

#include "util.h"

#include "scene.h"

scene_t* scene_load(const char filename[]) {
	// TODO "gut, dass wir hier <s>kein</s> nicht so viel error handling gemacht haben"
	json_error_t error;
	json_t* const root = json_load_file(filename, 0, &error);
	if(root == NULL) {
		fprintf(stderr, "Error parsing json in %s line: %d column: %d!\n", filename, error.line, error.column);
		fprintf(stderr, error.text, filename);
		fprintf(stderr, "\n");
		return NULL;
	}

	// load tweakables from json
	json_t* const tweakables_json = json_object_get(root, "tweakables");
	const int size = json_array_size(tweakables_json);
	tweakable_t* const tweakables = malloc(sizeof(tweakable_t) * size);
	int real_tweakables_size = 0;
	for(int i = 0; i < size; i++) {
		json_t* const tweakable_json = json_array_get(tweakables_json, i);
		const char* const name_json = json_string_value(json_object_get(tweakable_json, "name"));
		const char* const uniform_name_json = json_string_value(json_object_get(tweakable_json, "uniform"));
		const char* const type_json = json_string_value(json_object_get(tweakable_json, "type"));
		// for varparam syntax see: http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:varparamsyntax
		const char* varparam_json = json_string_value(json_object_get(tweakable_json, "varparam"));

		if(name_json == NULL || uniform_name_json == NULL || type_json == NULL) {
			fprintf(stderr, "tweakable #%d: name, uniform or type missing\n", i);
			continue;
		}
		if (varparam_json == NULL) {
			varparam_json = "";
		}

		tweakable_type_t type;
		if(!tweakable_get_type(type_json, &type)) {
			fprintf(stderr, "tweakable #%d: Invalid type: %s\n", i, type_json);
			continue;
		}

		void* defaults = NULL;
		json_t* defaults_object_json = json_object_get(tweakable_json, "default");
		if (defaults_object_json != NULL) {
			switch (type) {
				case COLOR:
					if (!json_is_array(defaults_object_json) || json_array_size(defaults_object_json) != 3) {
						fprintf(stderr, "tweakable #%d: default of color is no array of length 3\n", i);
						break;
					}
					defaults = malloc(3 * sizeof(float));
					float* defaults_color = (float*) defaults;
					for (int i = 0; i < 3; i++) {
						defaults_color[i] = json_real_value(json_array_get(defaults_object_json, i));
					}
					break;
				case FLOAT:
					if (!json_is_number(defaults_object_json)) {
						fprintf(stderr, "tweakable #%d: default is no number\n", i);
						break;
					}
					defaults = malloc(sizeof(float));
					float* defaults_float = (float*) defaults;
					*defaults_float = json_real_value(defaults_object_json);
					break;
				default:
					break;
			}
		}

		tweakable_t* const tweakable = &tweakables[real_tweakables_size];
		if(!tweakable_init(tweakable, name_json, uniform_name_json, type, varparam_json, defaults)) {
			fprintf(stderr, "Failed to init tweakable #%d\n", i);
			continue;
		}
		real_tweakables_size++;
		util_safe_free(defaults);
	}

	// load textures
	json_t* const textures_json = json_object_get(root, "textures");
	const int textures_size = json_array_size(textures_json);
	texture_t* const textures = malloc(sizeof(texture_t) * textures_size);
	int real_textures_size = 0;
	for(int i = 0; i < textures_size; i++) {
		json_t* const texture_json = json_array_get(textures_json, i);
		const char* const path_json = json_string_value(json_object_get(texture_json, "path"));
		const char* const uniform_json = json_string_value(json_object_get(texture_json, "uniform"));
		if(path_json == NULL || uniform_json == NULL) {
			fprintf(stderr, "texture #%d: path or uniform missing\n", i);
			continue;
		}
		texture_t* const texture = &textures[real_textures_size];
		if(!texture_init(texture, path_json, uniform_json)) {
			fprintf(stderr, "Failed to load texture #%d\n", i);
			continue;
		}
		real_textures_size++;
	}

	json_decref(root);

	scene_t* const scene = malloc(sizeof(scene_t));
	if(scene == NULL) return NULL;
	scene->tweakables = tweakables;
	scene->textures = textures;
	scene->num_tweakables = real_tweakables_size;
	scene->num_textures = real_textures_size;
	return scene;
}

void scene_load_uniforms(scene_t* const scene, const GLuint program) {
	for(int i = 0; i < scene->num_tweakables; i++) {
		tweakable_load_uniform(&scene->tweakables[i], program);
	}
	for(int i = 0; i < scene->num_textures; i++) {
		texture_load_uniform(&scene->textures[i], program);
	}
}

void scene_bind(const scene_t* const scene) {
	for(int i = 0; i < scene->num_tweakables; i++) {
		tweakable_update_uniform(&scene->tweakables[i]);
	}
	for(int i = 0; i < scene->num_textures; i++) {
		texture_bind(&scene->textures[i], i);
	}
}

void scene_add_to_tweakbar(const scene_t* const scene, TwBar* const tweak_bar) {
	for(int i = 0; i < scene->num_tweakables; i++) {
		tweakable_add_to_bar(&scene->tweakables[i], tweak_bar);
	}

}

void scene_destroy(scene_t* const scene) {
	for(int i = 0; i < scene->num_tweakables; i++) {
		tweakable_destroy(&scene->tweakables[i]);
	}
	for(int i = 0; i < scene->num_textures; i++) {
		texture_destroy(&scene->textures[i]);
	}
	util_safe_free(scene->tweakables);
	util_safe_free(scene->textures);
}
