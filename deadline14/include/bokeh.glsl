float kernelSize = 13.;

bool shouldBlur(float this_coc, float other_coc, float distance) {
	float quotient = other_coc / this_coc;
	return
		abs(other_coc) - distance > -1e-6 && (
				// TODO: edit the following lines
		(other_coc < 0. && (quotient > .99 || quotient < 0.)) ||
		(other_coc > 0. && (quotient > 0. && quotient < 1.01))
		);
}
