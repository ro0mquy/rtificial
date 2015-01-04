#include "Ventilate.h"

#include "FlareRenderer.h"

int main() {
	Ventilate<FlareRenderer> ventilate("Example renderer", 800, 600);
	return 0;
}
