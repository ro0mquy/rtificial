#include "Ventilate.h"

#include "FluidRenderer.h"

int main() {
	Ventilate<FluidRenderer> ventilate("Fluid renderer", 800, 600);
	return 0;
}
