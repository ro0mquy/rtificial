#ifndef TWEAK_LOADER_H
#define TWEAK_LOADER_H

typedef enum tweakable_type_t {
	COLOR, FLOAT
} tweakable_type_t;

typedef struct tweakable_t {
	char* name;
	char* uniform_name;
	tweakable_type_t type;
	void* value;

} tweakable_t;

int tweak_loader_load(const char filename[], tweakable_t** tweakables_out);

#endif
