#include <sndfile.hh>
#include <iostream>
#include <cmath>
using namespace std;

const double PI = acos(-1.);

void dct(float* source, float* dest, size_t n) {
	for (size_t i = 0; i < n; i++) {
		double sum = 0;
		for (size_t j = 0; j < n; j++) {
			sum += source[j] * cos(PI / n * (j + .5) * (i + .5));
		}
		dest[i] = sum * sqrt(2. / n);
	}
}

int main(int argc, char* argv[]) {
	SndfileHandle inFile(argv[1]);

	float* buffer = new float[inFile.frames()];
	float* transformed = new float[inFile.frames()];
	float* inv_transformed = new float[inFile.frames()];
	inFile.read(buffer, inFile.frames());

	dct(buffer, transformed, inFile.frames());
	for (size_t i = 2000; i < inFile.frames(); i++) {
		transformed[i] = 0;
	}
	dct(transformed, inv_transformed, inFile.frames());

	cout << inFile.frames() << endl;

	SndfileHandle outFile("out.wav", SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 1, 44100);
	outFile.write(inv_transformed, inFile.frames());

	delete[] inv_transformed;
	delete[] transformed;
	delete[] buffer;
	return 0;
}
