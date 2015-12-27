#include <iostream>
#include <sndfile.hh>
#include <Eigen/Sparse>
#include <complex>

using namespace Eigen;

using Floating = double;
using Complex = std::complex<Floating>;
Floating PI = std::acos(Floating(-1));
Floating PI_INV = Floating(1) / PI;

template<typename T>
T square(T x) {
	return x * x;
}

template<typename T>
T clamp(T x, T a, T b) {
	return std::min(std::max(x, a), b);
}

int main(int argc, char* argv[]) {
	(void) argc;

	size_t N_t = 32; // number of gabor functions in time
	size_t N_w = 96; // number of gabor functions in omega
	size_t N_pvn = N_t * N_w;

	float* buffer;
	size_t N;
	{
		SndfileHandle inFile(argv[1]);
		N = inFile.frames();
		const size_t fill = std::min(N_pvn - (N % N_pvn), N_pvn - 1);
		N += fill;
		buffer = new float[N];
		inFile.read(buffer, N);
		for (size_t i = inFile.frames(); i < N; i++) {
			buffer[i] = 0;
		}
	}

	Floating dt = Floating(1) / (Floating(44100-1)); // 1/samplerate?


	Floating range_t = N_pvn * dt;
	Floating range_w = 2 * PI / dt;

	Floating D_t = range_t / N_t; // space between gabor functions in time
	Floating D_w = range_w / N_w; // space between gabor functions in omega

	Floating alpha = D_w * .5 / D_t;

	// t_0 = n*D_t + t_0_start
	Floating t_0_start = range_t * .5 - Floating(N_t) * .5 * D_t + .5 * D_t;
	Floating w_0_start = -Floating(N_w) * .5 * D_w + .5 * D_w;

	SparseMatrix<Complex, ColMajor> G_dagger(N_pvn, N_pvn);
	SparseLU<SparseMatrix<Complex, ColMajor>> solver;
	for (size_t m = 0; m < N_w; m++) {
		for (size_t n = 0; n < N_t; n++) {
			const size_t i = m * N_t + n;
			const Floating t_0 = t_0_start + n * D_t;
			const Floating w_0 = w_0_start + m * D_w;
			for (size_t j = 0; j < N_pvn; j++) {
				const Floating t = j * dt;
				const Floating t_offset = t - t_0;
				Complex g = 
					std::pow(2 * alpha / PI, .25) * exp(Complex(
						-alpha*square(t_offset),
						w_0 * t_offset
					));
				if (std::abs(g) > 1e-6) {
					G_dagger.insert(i, j) = std::conj(g);
				}
			}
		}
	}
	std::cout << "computed!" << std::endl;
	G_dagger.makeCompressed();
	std::cout << "compressed!" << std::endl;
	solver.analyzePattern(G_dagger); 
	std::cout << "analyzed!" << std::endl;
	solver.factorize(G_dagger);
	std::cout << "factorized!" << std::endl;

	Matrix<Complex, Dynamic, 1> input(N_pvn);
	for (size_t i = 0; i < N; i += N_pvn) {
		for (size_t j = 0; j < N_pvn; j++) {
			input(j) = buffer[i + j];
		}

		Matrix<Complex, Dynamic, 1> d = G_dagger * input;
		Matrix<Complex, Dynamic, 1> d_normalized = d / sqrt(d.dot(d));
		Floating cutoff = 3e-2;
		size_t discarded = 0;
		for (size_t j = 0; j < N_pvn; j++) {
			if (std::abs(d_normalized[j]) <= cutoff) {
				d[j] = 0;
				discarded++;
			}
		}
		std::cout << "discarded " <<  discarded << std::endl;
		Matrix<Complex, Dynamic, 1> x = solver.solve(d);
		for (size_t j = 0; j < N_pvn; j++) {
			buffer[i + j] = x[j].real();
		}
	}
	{
		SndfileHandle outFile("out.wav", SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 1, 44100);
		outFile.write(buffer, N);
	}

	delete[] buffer;
	return 0;
}
