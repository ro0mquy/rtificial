#include <string.h>
#include <stdlib.h>

#include <jansson.h>

#include "util.h"

#include "scene.h"

scene_t* scene_load(const char scene_path[], const char config_path[]) {
	// TODO "gut, dass wir hier <s>kein</s> nicht so viel error handling gemacht haben"
	json_error_t error;
	json_t* const root = json_load_file(config_path, 0, &error);
	if(root == NULL) {
		fprintf(stderr, "Error parsing json in %s line: %d column: %d!\n", config_path, error.line, error.column);
		fprintf(stderr, error.text, config_path);
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
				case BOOL:
					if (!json_is_boolean(defaults_object_json)) {
						fprintf(stderr, "tweakable #%d: default is no boolean\n", i);
						break;
					}
					defaults = malloc(sizeof(bool));
					bool* defaults_bool = (bool*) defaults;
					*defaults_bool = json_real_value(defaults_object_json);
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
				case DIR3:
					if (!json_is_array(defaults_object_json) || json_array_size(defaults_object_json) != 3) {
						fprintf(stderr, "tweakable #%d: default of direction is no array of length 3\n", i);
						break;
					}
					defaults = malloc(3 * sizeof(float));
					float* defaults_dir = (float*) defaults;
					for (int i = 0; i < 3; i++) {
						defaults_dir[i] = json_real_value(json_array_get(defaults_object_json, i));
					}
					break;
				case ROTATION:
					if (!json_is_array(defaults_object_json) || json_array_size(defaults_object_json) != 4) {
						fprintf(stderr, "tweakable #%d: default of rotation is no array of length 4\n", i);
						break;
					}
					defaults = malloc(4 * sizeof(float));
					float* defaults_rot = (float*) defaults;
					for (int i = 0; i < 4; i++) {
						defaults_rot[i] = json_real_value(json_array_get(defaults_object_json, i));
					}
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
		json_t* const path_json_object = json_object_get(texture_json, "path");

		if(json_is_string(path_json_object)) {
			const char* const path_json = json_string_value(path_json_object);
			const char* const uniform_json = json_string_value(json_object_get(texture_json, "uniform"));
			if(path_json == NULL || uniform_json == NULL) {
				fprintf(stderr, "texture #%d: path or uniform missing\n", i);
				continue;
			}

			texture_t* const texture = &textures[real_textures_size];
			const bool ok = texture_init(texture, scene_path, path_json, uniform_json);
			if(!ok) {
				fprintf(stderr, "Failed to load texture #%d\n", i);
				continue;
			}
		} else if(json_is_array(path_json_object) && json_array_size(path_json_object) == 6) {
			const char* const uniform_json = json_string_value(json_object_get(texture_json, "uniform"));
			if(uniform_json == NULL) {
				fprintf(stderr, "texture #%d: uniform missing\n", i);
				continue;
			}

			char* paths_texture[6];
			const size_t scene_path_length = strlen(scene_path);
			for(int i = 0; i < 6; i++) {
				const char* const path_json = json_string_value(json_array_get(path_json_object, i));
				const size_t path_json_length = strlen(path_json);
				char* const path_texture = malloc(scene_path_length + path_json_length + 1);
				strncpy(path_texture, scene_path, scene_path_length);
				strncpy(path_texture + scene_path_length, path_json, path_json_length + 1);
				paths_texture[i] = path_texture;
			}

			texture_t* const texture = &textures[real_textures_size];
			const bool ok = texture_init_cubemap(texture, paths_texture, uniform_json);
			for(int i = 0; i < 6; i++) {
				util_safe_free(paths_texture[i]);
			}
			if(!ok) {
				fprintf(stderr, "Failed to load texture #%d\n", i);
				continue;
			}
		} else {
			fprintf(stderr, "Not a valid path in texture #%d\n", i);
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

void scene_save(const scene_t* scene, char* path){
	json_t* tweakables_json_array = json_array();
	for(int i = 0; i < scene->num_tweakables; i++){
		tweakable_t tweakable = scene->tweakables[i];
		json_t* tweakable_json_object = json_object();

		json_object_set(tweakable_json_object, "name", json_string(tweakable.name));
		json_object_set(tweakable_json_object, "uniform", json_string(tweakable.uniform_name));

		switch(tweakable.type){
			case FLOAT:
				json_object_set(tweakable_json_object, "type", json_string("float"));
				json_object_set(
					tweakable_json_object,
					"default",
					json_real( *((float*) tweakable.value) )
				);
				break;
			case COLOR:
				json_object_set(tweakable_json_object, "type", json_string("color"));
				const float* const colors = tweakable.value;
				json_object_set(
					tweakable_json_object,
					"default",
					json_pack("[fff]", colors[0], colors[1], colors[2])
				);
				break;
			case BOOL:
				json_object_set(tweakable_json_object, "type", json_string("bool"));
				json_object_set(
					tweakable_json_object,
					"default",
					json_boolean( *((float*) tweakable.value) )
				);
				break;
			case ROTATION:
				json_object_set(tweakable_json_object, "type", json_string("rotation"));
				const float* const rot_values = tweakable.value;
				json_object_set(
					tweakable_json_object,
					"default",
					json_pack("[ffff]", rot_values[0], rot_values[1], rot_values[2], rot_values[3])
				);
				break;
			case DIR3:
				json_object_set(tweakable_json_object, "type", json_string("direction"));
				const float* const dir_values = tweakable.value;
				json_object_set(
					tweakable_json_object,
					"default",
					json_pack("[fff]", dir_values[0], dir_values[1], dir_values[2])
				);
				break;
		}

		if(strlen(tweakable.varparam) > 0){
			json_object_set(tweakable_json_object, "varparam", json_string(tweakable.varparam));
		}

		json_array_append(tweakables_json_array, tweakable_json_object);
	}

	json_t* textures_json_array = json_array();
	for(int i = 0; i < scene->num_textures; i++){
		json_t* texture_json_object = json_object();

		json_object_set(texture_json_object, "path", json_string(scene->textures[i].path));
		json_object_set(texture_json_object, "uniform", json_string(scene->textures[i].uniform_name));

		json_array_append(textures_json_array, texture_json_object);
	}

	json_t* scene_config_json = json_object();
	json_object_set(scene_config_json, "tweakables", tweakables_json_array);
	json_object_set(scene_config_json, "textures", textures_json_array);

	if(json_dump_file(scene_config_json, path, JSON_INDENT(4) | JSON_PRESERVE_ORDER)){
		fprintf(stderr, "error: config export: config file \"%s\" could not be written\n", path);
	}
	else{
		printf("config file \"%s\" written\n", path);
	}
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
