#include <string.h>
#include <stdlib.h>

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
		int real_size = 0;
		for(int i = 0; i < size; i++) {
			json_t* const tweakable_json = json_array_get(tweakables_json, i);
			const char* const name_json = json_string_value(json_object_get(tweakable_json, "name"));
			const char* const uniform_name_json = json_string_value(json_object_get(tweakable_json, "uniform"));
			const char* const type_json = json_string_value(json_object_get(tweakable_json, "type"));
			if(name_json == NULL || uniform_name_json == NULL || type_json == NULL) {
				fprintf(stderr, "name, uniform_name or type missing\n");
				continue;
			}
			tweakable_type_t type;
			if(!tweakable_get_type(type_json, &type)) {
				fprintf(stderr, "Invalid type: %s\n", type_json);
				continue;
			}
			tweakable_t* const tweakable = &tweakables[real_size];
			if(!tweakable_init(tweakable, name_json, uniform_name_json, type)) {
				fprintf(stderr, "Failed to init tweakable\n");
				continue;
			}
			real_size++;
		}
		json_decref(root);
		*tweakables_out = tweakables;
		return real_size;
	} else {
		fprintf(stderr, "Error parsing json in %s line: %d column: %d!\n", filename, error.line, error.column);
		fprintf(stderr, error.text, filename);
		fprintf(stderr, "\n");
		return 0;
	}
}
