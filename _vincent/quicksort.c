#include <stdlib.h>
#include <stdio.h>

typedef int (*compare_t)(const void*,const void*);

void swap(size_t size, void* a_, void* b_) {
	char* a = (char*) a_;
	char* b = (char*) b_;
	for (size_t i = 0; i < size; i++) {
		char t = b[i];
		b[i] = a[i];
		a[i] = t;
	}
}

void* pickPivot(void* base, size_t size, size_t l, size_t r, compare_t cmp) {
	return base + size * l;
}

size_t partition(void* base, size_t size, size_t l, size_t r, void* pivot, compare_t cmp) {
	swap(size, base + r * size, pivot);
	size_t i = l;
	for (size_t j = l; j < r; j++) {
		// compare to pivot
		if (cmp(base + j * size, base + r * size) <= 0) {
			swap(size, base + i * size, base + j * size);
			i++;
		}
	}
	swap(size, base + i * size, base + r * size);
	return i;
}

void insertionSort(void* base, size_t size, size_t l, size_t r, compare_t cmp) {
	for (size_t i = l + 1; i <= r; i++) {
		size_t j = i;
		while (j >= l + 1 && cmp(base + (j - 1) * size, base + j * size) > 0) {
			swap(size, base + (j - 1) * size, base + j * size);
			j--;
		}
	}
}


void qsort_helper(void* base, size_t l, size_t r, size_t size, compare_t cmp) {
	const size_t n0 = 1;

	while (r - l + 1 > n0) {
		void* pivot = pickPivot(base, size, l, r, cmp);
		size_t m = partition(base, size, l, r, pivot, cmp);
		if (m < (l + r) / 2) {
			qsort_helper(base, l, m - 1, size, cmp);
			l = m + 1;
		} else {
			qsort_helper(base, m + 1, r, size, cmp);
			r = m - 1;
		}
	}
	insertionSort(base, size, l, r, cmp);
}

void qsort(void* base, size_t num, size_t size, compare_t cmp) {
	if (num < 2) return;
	qsort_helper(base, 0, num - 1, size, cmp);
}

int intcmp(const void* a_,const void* b_) {
	int a = *((const int*) a_);
	int b = *((const int*) b_);
	return a - b;
}

void main() {
	int foo[] = { 8, 1, 3, 2, 5, 1, 8, 3, 2, 3, 4, 5, 1, 2, 9, 3, 4, 5, 2, 6, 8, 20, 10, 4, 2 };
	//int foo[] = {2, 3, 1, 5};
	qsort(foo, sizeof(foo)/sizeof(int), sizeof(int), intcmp);
	//insertionSort(foo, sizeof(foo)/sizeof(int), 0, sizeof(foo)/sizeof(int) - 1, intcmp);
	for (size_t i = 0; i < sizeof(foo)/sizeof(int); i++) {
		printf("%d\n", foo[i]);
	}
}
