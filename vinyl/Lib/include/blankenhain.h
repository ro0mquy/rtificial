#pragma once

namespace blankenhain {
	// buffer needs to have a size of at least 2 * lengthInSamples * sizeof(float) bytes
	void render(float* buffer);
	unsigned int lengthInSamples();
}