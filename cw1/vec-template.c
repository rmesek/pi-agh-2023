#include <stdio.h>
#include <math.h>
#define N 100

// generates the sequence by incrementing the start value
// using the step size until it reaches the stop value
// (stop value is not included)
int range(double array[], double start, double stop, double step);

// Returns n evenly spaced samples, calculated over the interval [start, stop].
// n >= 0
// for n = 0 return empty array
// for n = 1 return one-element array, with array[0] = start
void linspace(double array[], double start, double stop, int n);

// multiply each element of v by the value of scalar
void multiply_by_scalar(double v[], int n, double scalar);

// add to each element v1[i] value of v2[i]
void add(double v1[], const double v2[], int n);

// calculate and return the dot product of v1 and v2
double dot_product(const double v1[], const double v2[], int n);

// read double vector of size n
void read_vector(double v[], int n);

// print double vector of size n (with 2 significant figures)
void print_vector(const double v[], int n) {
	for (int i = 0; i < n; ++i) {
		printf("%.2f ", v[i]);
	}
	printf("\n");
}

int main(void) {

	int to_do, len;
	double start, stop, step, scalar;
	double vector_1[N], vector_2[N];

	scanf("%d", &to_do);

	switch (to_do) {
		case 1: // linspace
			scanf("%d %lf %lf", &len, &start, &stop);
			linspace(vector_1, start, stop, len);
			print_vector(vector_1, len);
			break;
		case 2: // add
			scanf("%d", &len);
			read_vector(vector_1, len);
			read_vector(vector_2, len);
			add(vector_1, vector_2, len);
			print_vector(vector_1, len);
			break;
		case 3: // dot product
			scanf("%d", &len);
			read_vector(vector_1, len);
			read_vector(vector_2, len);
			printf("%.2f\n", dot_product(vector_1, vector_2, len));
			break;
		case 4: // multiply by scalar
			scanf("%d %lf", &len, &scalar);
			read_vector(vector_1, len);
			multiply_by_scalar(vector_1, len, scalar);
			print_vector(vector_1, len);
			break;
		case 5: // range
			scanf("%lf %lf %lf", &start, &stop, &step);
			int n = range(vector_1, start, stop, step);
			print_vector(vector_1, n);
			break;
		default:
			printf("Unknown operation %d", to_do);
			break;
	}
	return 0;
}
