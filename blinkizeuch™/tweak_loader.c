#include <string.h>

#include <jansson.h>

#include "tweak_loader.h"

int tweak_loader_load(const char filename[], tweakable_t** tweakables_out) {
	// TODO "gut, dass wir hier kein error handling gemacht haben"
	json_error_t error;
	json_t* const root = json_load_file(filename, 0, &error);
	if(root != NULL) {
		json_t* const tweakables_json = json_object_get(root, "tweakables");
		const int size = json_array_size(tweakables_json);
		tweakable_t* const tweakables = malloc(sizeof(tweakable_t) * size);
		for(int i = 0; i < size; i++) {
			tweakable_t* const tweakable = &tweakables[i];
			json_t* const tweakable_json = json_array_get(tweakables_json, i);
			const char* const name_json = json_string_value(json_object_get(tweakable_json, "name"));
			const int name_length = strlen(name_json);
			tweakable->name = malloc(name_length + 1);
			tweakable->name[name_length] = 0;
			strncpy(tweakable->name, name_json, name_length);
			const char* const uniform_name_json = json_string_value(json_object_get(tweakable_json, "uniform"));
			const int uniform_name_length = strlen(uniform_name_json);
			tweakable->uniform_name = malloc(uniform_name_length + 1);
			tweakable->uniform_name[uniform_name_length] = 0;
			strncpy(tweakable->uniform_name, uniform_name_json, uniform_name_length);
			const char* const type_json = json_string_value(json_object_get(tweakable_json, "type"));
			if(strcmp("color", type_json)) {
				tweakable->type = COLOR;
			} else if(strcmp("float", type_json)) {
				tweakable->type = FLOAT;
			}
		}
		json_decref(root);
		*tweakables_out = tweakables;
		return size;
	} else {
		fprintf(stderr, "Error parsing json in %s line: %d column: %d!\n", filename, error.line, error.column);
		fprintf(stderr, error.text, filename);
		return 0;
	}
}
