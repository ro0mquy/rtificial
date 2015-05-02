#include "Ventilate.h"

#include "ComputeShaderRenderer.h"

int main() {
	Ventilate<ComputeShaderRenderer> ventilate("Compute shader renderer", 800, 600);
	return 0;
}
