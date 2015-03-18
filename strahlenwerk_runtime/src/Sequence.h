#ifndef SEQUENCE_H
#define SEQUENCE_H

#define SEQ_INTERPOLATION_STEP 0
#define SEQ_INTERPOLATION_LINEAR 1
#define SEQ_INTERPOLATION_CCRSPLINE 2

struct Sequence {
	int start;
	int end;
	int interpolation;
	int numKeyframes;
};

#endif
