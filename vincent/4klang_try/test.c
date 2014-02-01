#include "4klang.inh"
#include <inttypes.h>
#include <stdio.h>

SAMPLE_TYPE buffer[MAX_SAMPLES * 2];

int main() {
	__4klang_render(buffer);
	fwrite(buffer, sizeof(SAMPLE_TYPE), MAX_SAMPLES * 2, stdout);
	return 0;
}
