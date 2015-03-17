#include "arhosek/ArHosekSkyModel.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc != 5) {
		// turbidity: 1-10
		// albedo: 0-1
		// elevation: 0-90
		// azimuth: 0-360
		fprintf(stderr, "Usage: ./generate <turbidity> <albedo> <elevation> <azimuth>");
		return 1;
	}

	double turbidity = atof(argv[1]);
	double albedo = atof(argv[2]);
	fprintf(stderr, "%f\n", albedo);
	double elevation = atof(argv[3])/180.*acos(-1.);
	double inclination = acos(-1.) * .5 - elevation;
	double azimuth = atof(argv[4])/180.*acos(-1.);
	double sun_x = sin(inclination) * sin(azimuth);
	double sun_y = cos(inclination);
	double sun_z = sin(inclination) * cos(azimuth);
	ArHosekSkyModelState* state = arhosek_rgb_skymodelstate_alloc_init(turbidity, albedo, elevation);
	puts("vec3 sky_radiance(vec3 d) {");
	printf("\tvec3 sun_dir = vec3(%f, %f, %f);\n", sun_x, sun_y, sun_z);
	puts("\tvec3 color;");
	puts("\tfloat cos_gamma = dot(d, sun_dir);");
	puts("\tfloat gamma = acos(cos_gamma);");
	puts("\tfloat cos_theta = d.y;");
	puts("\tfloat rayM = cos_gamma * cos_gamma;");
	puts("\tfloat zenith = sqrt(cos_theta);");
	for (unsigned int i = 0; i < 3; i++) {
		puts("\t{");
		printf("\t\tfloat A = %f;\n", state->configs[i][0]);
		printf("\t\tfloat B = %f;\n", state->configs[i][1]);
		printf("\t\tfloat C = %f;\n", state->configs[i][2]);
		printf("\t\tfloat D = %f;\n", state->configs[i][3]);
		printf("\t\tfloat E = %f;\n", state->configs[i][4]);
		printf("\t\tfloat F = %f;\n", state->configs[i][5]);
		printf("\t\tfloat G = %f;\n", state->configs[i][6]);
		printf("\t\tfloat H = %f;\n", state->configs[i][7]);
		printf("\t\tfloat I = %f;\n", state->configs[i][8]);
		puts("\t\tfloat expM = exp(E * gamma);");
		puts("\t\tfloat mieM = (1. + rayM) / pow((1.0 + I * I - 2.0*I*cos_gamma), 1.5);");
		printf("\t\tcolor[%d] = (1.0 + A * exp(B / (cos_theta + 0.01))) * (C + D * expM + F * rayM + G * mieM + H * zenith);\n", i);
		puts("\t}");
	}
	printf("\tvec3 radiances = vec3(%f, %f, %f);\n", state->radiances[0], state->radiances[1], state->radiances[2]);
	puts("\treturn radiances * color;");
	puts("}");

	arhosekskymodelstate_free(state);
	return 0;
}
