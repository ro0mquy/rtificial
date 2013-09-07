#include "tweak_loader.h"

void tweak_loader_load(const char filename[]) {
	json_error_t error;
	json_load_file(filename, 0, &error);
}
