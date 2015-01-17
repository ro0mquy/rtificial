#include <array>
#include <iostream>
#include <cmath>
#include <algorithm>

const double PI = acos(-1.0);

double profile(double fi, double bwi) {
	const double x = fi / bwi;
	return exp(-x * x) / bwi;
}

double rnd() {
	return static_cast<double>(rand()) / (static_cast<double>(RAND_MAX) + 1.0);
}

void real_ifft(unsigned int N, float* amp, float* freq, float* output_real, float* output_img, unsigned int stride) {
	if (N == 1) {
		output_real[0] = amp[0];
		output_img[0] = freq[0];
	} else {
		real_ifft(N/2, amp, freq, output_real, output_img, 2 * stride);
		real_ifft(N/2, amp + stride, freq + stride, output_real + N/2, output_img + N/2, 2 * stride);
		for (unsigned int i = 0; i < N; i++) {
			output_real[i] /= 2;
			output_img[i] /= 2;
		}
		for (unsigned int k = 0; k < N/2; k++) {
			const double t_real = output_real[k];
			const double t_img = output_img[k];
			const double twiddle_cos = cos((2.0 * PI * k) / N);
			const double twiddle_sin = sin((2.0 * PI * k) / N);
			const double a = output_real[k + N/2];
			const double b = output_img[k + N/2];
			const double real = a * twiddle_cos - b * twiddle_sin;
			const double img = a * twiddle_sin + b * twiddle_cos;
			output_real[k] = t_real + real;
			output_img[k] = t_img + img;
			output_real[k + N/2] = t_real - real;
			output_img[k + N/2] = t_img - img;
		}
	}
}

void real_ifft(unsigned int N, float* amp, float* freq, float* output) {
	for (unsigned int i = 0; i < N / 2; i++) {
		const double a = amp[i];
		amp[i] = a * cos(freq[i]);
		freq[i] = a * sin(freq[i]);
	}
	// fill up symmetric values
	// TODO use more efficient iFFT of half size
	for (unsigned int k = 0; k < N/2; k++) {
		amp[N - k] = amp[k];
		freq[N - k] = -freq[k];
	}
	std::vector<float> output_img(N);
	real_ifft(N, amp, freq, output, &output_img[0], 1);
}

void normalize_samples(unsigned int N, float* samples) {
	float max_sample = 1e-6;
	for (unsigned int i = 0; i < N; i++) {
		max_sample = std::max(max_sample, std::abs(samples[i]));
	}
	for (unsigned int i = 0; i < N; i++) {
		// normalize to 1/sqrt(2)
		samples[i] /= max_sample * 1.4142;
		//samples[i] /= max_sample;
	}
}

int main() {
	srand(48348545);

	const unsigned int N = 262144;
	const unsigned int samplerate = 44100;
	const unsigned int frequency = 440;
	const unsigned int bandwidth_cents = 50;
	const unsigned int n_harmonics = 10;

	std::array<float, n_harmonics> harmonics_amplitudes = {
		1.0, 0.8, 0.5, 0.3, 0.2, 0.1, 0.08, 0.3, 0.03, 0.02,
	};
	std::array<float, N> freq_amp{{0}};
	std::array<float, N> freq_phase{{0}};

	for (unsigned int harmonic = 1; harmonic <= n_harmonics; harmonic++) {
		const double bandwidth_Hz = (exp2(bandwidth_cents / static_cast<double>(1200.0)) - 1.0) * frequency * harmonic;
		const double bandwidth_i = bandwidth_Hz / (2.0 * samplerate);
		const double frequency_i = static_cast<double>(frequency * harmonic) / samplerate;
		for (unsigned int i = 0; i < N/2; i++) {
			const double hprofile = profile((static_cast<double>(i) / N) - frequency_i, bandwidth_i);
			freq_amp[i] += harmonics_amplitudes[harmonic - 1] * hprofile;
		}
	}

	for (unsigned int i = 0; i < N/2; i++) {
		freq_phase[i] = rnd() * 2.0 * PI;
	}

	std::array<float, N> samples;
	real_ifft(N, &freq_amp[0], &freq_phase[0], &samples[0]);
	normalize_samples(N, &samples[0]);
	std::cout.write(reinterpret_cast<char*>(&samples[0]), sizeof(float) * samples.size() / sizeof(char));

	return 0;
}
