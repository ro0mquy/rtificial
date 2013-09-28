#include <string.h>
#include <stdlib.h>

#include "util.h"

char* util_dup_string(const char string[]) {
	size_t length = strlen(string) + 1; // length including 0 byte
	char* const buffer = malloc(sizeof(char) * length);
	if(buffer == NULL) return NULL;
	return strncpy(buffer, string, length);
}

void util_safe_free(void* const pointer) {
	if(pointer != NULL) {
	   free(pointer);
	}
}
