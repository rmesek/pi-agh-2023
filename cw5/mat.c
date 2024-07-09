#include <stdio.h>
#include <math.h>

#define SIZE 40
#define TRUE 1
#define FALSE 0

void fill_ind(int A[], int n);
void swap_col(int ind[], int i, int j);
int swap_max_ind(double A[][SIZE], int ind[], int n, int Ax, int Ay);

void TEST_swap_col();
void TEST_swap_max_ind();
void TEST_print_mat_ind();

void swap(double *a, double*b) {
	double temp = *a;
	*a = *b;
	*b = temp;
}

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

void print_mat_ind(double A[][SIZE], int m, int n, const int indices[]);

// 5.1
// Calculate matrix product, AB = A X B
// A[m][p], B[p][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n);

// Calculate matrix - vector product
void mat_vec_product(double A[][SIZE], const double b[], double Ab[], int m, int n);

void backward_substit(double A[][SIZE], double x[], int n);

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n);

// 5.2
// Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n);

// 5.3
// Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps);

// 5.4
// Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps);

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if(det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A,n,n);
			printf("%.4f\n",matrix_inv(A, B, n, eps));
			print_mat(B, n, n);
			break;
		default:
			TEST_swap_col();
			TEST_print_mat_ind();
			TEST_swap_max_ind();
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {  // OK
	int ABx = 0, ABy = 0;
	int Ax, Ay, Bx, By;
	double product;
	
	for (Ay = 0; Ay < m; ++Ay) {
		for (Bx = 0; Bx < n; ++Bx) {
			product = 0;
			for (Ax = 0, By = 0; Ax < p; ++Ax, ++By) {
				product += A[Ay][Ax] * B[By][Bx];
			}
			ABx = Ay, ABy = Bx;
			AB[ABx][ABy] = product;
		}
	}
}

void mat_vec_product(double A[][SIZE], const double b[], double Ab[], int m, int n) {  // NOT USED
	
}

void backward_substit(double A[][SIZE], double x[], int n) {  // NOT USED
	
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {  // NOT USED
	
}

double gauss_simplified(double A[][SIZE], int n) {  // OK
	int Ax = 0, Ay = 1;
	double ratio, det = 1;
	
	for (Ax = 0; Ax < n; ++Ax) {
		for (Ay = Ax + 1; Ay < n; ++Ay) {
			ratio = A[Ay][Ax] / A[Ax][Ax];
			//printf("\nA[%d][%d]/A[%d][%d]=", Ax, Ax, Ay, Ax);
			//printf("%lf\n", ratio);
			
			for (int x = 0; x < n; ++x) {
				A[Ay][x] -= ratio*A[Ax][x];
			}
			//print_mat(A, n, n);
		}
	}
	
	for (Ax = 0, Ay = 0; Ax < n; ++Ax, ++Ay) {
		det *= A[Ay][Ax];
	}
	
	if (det == 0) det = NAN;
	return det;
}

void print_arr(int A[], int n) {  // OK
	for (int i = 0; i < n; ++i) {
		printf("%d ", A[i]);
	}
	printf("\n");
}

void fill_ind(int A[], int n) {  // OK
	for (int i = 0; i < n; ++i) A[i] = i;
}

void TEST_swap_col() {  // OK
	int OK = TRUE;
	int ind[] = {0, 1, 2, 3};
	swap_col(ind, 0, 3);  // [3, 1, 2, 0]
	swap_col(ind, 0, 1); // [1, 3, 2, 0]
	swap_col(ind, 1, 3);
	swap_col(ind, 1, 3);
	
	int SOL_ind[] = {1, 3, 2, 0};
	printf("\n\n\tTEST_swap_col()\n");
	for (int i = 0; i < 4; ++i) {
		if (ind[i] != SOL_ind[i]) OK = FALSE;
		printf("%d ", ind[i]);
	}
	printf("= ");
	for (int i = 0; i < 4; ++i) {
		printf("%d ", SOL_ind[i]);
	}
	printf("\n");
	if (OK == FALSE) printf("ERROR WITH TEST_swap_col()\n");
}

void swap_col(int ind[], int i, int j) {  // OK
	int temp = ind[i];
	ind[i] = ind[j];
	ind[j] = temp;
}

void print_mat_ind(double A[][SIZE], int m, int n, const int indices[]) {  // OK
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			//printf("%d ", indices[i]);
			printf("%.4f ", A[ indices[i] ][j]);
		}
		printf("\n");
	}
}

void TEST_print_mat_ind() {  // OK
	int ind[] = {2, 0, 1};
	double mat[][SIZE] = { 
		{1.12345, 2.12344, 3.12345}, //0
		{4.12344, 5.12345, 6.12344}, //1
		{7.12345, 8.12344, 9.12345}  //2
	};
	
	printf("\n\n\tTEST_print_mat_ind()\n");
	//print_mat_ind(mat, 3, 3, ind);
	print_mat_ind(mat, 3, 3, ind);
	printf("=\n");
	printf("7.1235 8.1234 9.1235\n");  //2
	printf("1.1235 2.1234 3.1235\n");  //0
	printf("4.1234 5.1235 6.1234\n\n");  //1
}

void TEST_swap_max_ind() {  // OK
	int ind[] = {0, 1, 2};
	double mat[][SIZE] = { 
		{1, 3, 4}, //0
		{0, 2, 2}, //1
		{2, 9, 3}  //2
	};
	
	swap_max_ind(mat, ind, 3, 0, 0);
	printf("\n\n\tTEST_swap_max_ind() 1ST ROW\n");
	print_mat_ind(mat, 3, 3, ind);
	printf("=\n");
	printf("2.0000 9.0000 3.0000\n");  //2
	printf("0.0000 2.0000 2.0000\n");  //1
	printf("1.0000 3.0000 4.0000\n\n");  //0
	
	swap_max_ind(mat, ind, 3, 1, 1);
	printf("\n\n\tTEST_swap_max_ind() 2ND ROW (FROM [1][1])\n");
	print_mat_ind(mat, 3, 3, ind);
	printf("=\n");
	printf("2.0000 9.0000 3.0000\n");  //2
	printf("1.0000 3.0000 4.0000\n");  //0
	printf("0.0000 2.0000 2.0000\n\n");  //1
	
	swap_max_ind(mat, ind, 3, 2, 2);
	printf("\n\n\tTEST_swap_max_ind() 3RD ROW (FROM [2][2])\n");
	print_mat_ind(mat, 3, 3, ind);
	printf("=\n");
	printf("2.0000 9.0000 3.0000\n");  //2
	printf("1.0000 3.0000 4.0000\n");  //0
	printf("0.0000 2.0000 2.0000\n\n");  //1
	
	int ind2[] = {1, 0, 2, 3};
	double mat2[][SIZE] = { 
		{0, 0, 0.5, 0.5}, //0
		{2, -2, 3, -3}, //1
		{0, 2, -0.5, 1.5}, //2
		{0, 0, 2.5, 4.5}, //3
	};
	
	swap_max_ind(mat2, ind2, 3, 1, 1);
	printf("\n\n\tTEST_swap_max_ind() 2ND ROW (FROM [1][1])\n");
	print_mat_ind(mat2, 4, 4, ind2);
	printf("=\n");
	printf("2.0000 -2.0000 3.0000 -3.0000\n");
	printf("0.0000 2.0000 -0.5000 1.5000\n");
	printf("0.0000 0.0000 0.5000 0.5000\n");
	printf("0.0000 0.0000 2.5000 4.5000\n\n");
}

int swap_max_ind(double A[][SIZE], int ind[], int n, int Ax, int Ay) {  // OK
	int isSwapped = FALSE;
	int col_max_ind = Ay;
	double col_max = fabs(A[ ind[Ay] ][Ax]);
	
	for (int y = Ay; y < n; ++y) {
		if (fabs(A[ ind[y] ][Ax]) > col_max) {
			isSwapped = TRUE;
			col_max = fabs(A[ ind[y] ][Ax]);
			col_max_ind = y;
		}
	}
	swap_col(ind, Ay, col_max_ind);
	if (isSwapped) return -1;
	return 1;
}

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {  // OK
	int Ax, Ay;
	int det_sign = 1;
	double ratio, det = 1;
	int ind[SIZE] = {};
	fill_ind(ind, n);
	
	if (n >= SIZE) {
		printf("NOT ENOUGH SPACE, INCREASE SIZE!\n");
		return 0.;
	}
	
	for (int Ay = 0; Ay < n; ++Ay) {
		A[Ay][n] = b[Ay];
	}
	
	for (Ax = 0; Ax < n; ++Ax) {
		det_sign *= swap_max_ind(A, ind, n, Ax, Ax);
		if (fabs(A[ ind[Ax] ][Ax]) < eps) return 0.;
		
		for (Ay = Ax + 1; Ay < n; ++Ay) {
			ratio = A[ ind[Ay] ][Ax] / A[ ind[Ax] ][Ax];
			for (int x = Ax; x < n+1; ++x) {
				A[ ind[Ay] ][x] -= ratio * A[ ind[Ax] ][x];
			}
		}
	}
	//print_arr(ind, n);
	//print_mat_ind(A, n, n, ind);
	for (Ax = 0, Ay = 0; Ax < n; ++Ax, ++Ay) {
		det *= A[ ind[Ay] ][Ax];
	}
	
	if (b != NULL && x != NULL) {
		for (int Ay = n-1; Ay >= 0; --Ay) {
			x[Ay] = A[ ind[Ay] ][n];
			for (int Ax = n-1; Ax > Ay; --Ax) {
				x[Ay] -= A[ ind[Ay] ][Ax] * x[Ax];
			}
			x[Ay] /= A[ ind[Ay] ][Ay];
		}
	}
	
	return det_sign*det;
}

int swap_max_ind_up(double A[][SIZE], int ind[], int n, int Ax, int Ay) {  // OK
	int isSwapped = FALSE;
	int col_max_ind = Ay;
	double col_max = fabs(A[ ind[Ay] ][Ax]);
	
	for (int y = Ay; y < n; ++y) {
		if (fabs(A[ ind[y] ][Ax]) > col_max) {
			isSwapped = TRUE;
			col_max = fabs(A[ ind[y] ][Ax]);
			col_max_ind = y;
		}
	}
	swap_col(ind, Ay, col_max_ind);
	if (isSwapped) return -1;
	return 1;
}

double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {  // OK
	int Ax, Ay;  // A[Ay][Ax], Ax - left/right, Ay - up/down
	int det_sign = 1;
	double ratio, det = 1;
	int ind[SIZE] = {};
	fill_ind(ind, n);
	
	// Identity matrix B
	for (int x = 0; x < n; ++x) B[x][x] = 1;
	
	// Transform A into upper triangular matrix
	for (Ax = 0; Ax < n; ++Ax) {
		det_sign *= swap_max_ind(A, ind, n, Ax, Ax);
		if (fabs(A[ ind[Ax] ][Ax]) < eps) return 0.;
		
		for (Ay = Ax + 1; Ay < n; ++Ay) {
			ratio = A[ ind[Ay] ][Ax] / A[ ind[Ax] ][Ax];
			for (int x = 0; x < n; ++x) {
				A[ ind[Ay] ][x] -= ratio * A[ ind[Ax] ][x];
				B[ ind[Ay] ][x] -= ratio * B[ ind[Ax] ][x];
			}
		}
	}
	//print_arr(ind, n);
	//print_mat_ind(A, n, n, ind);
	
	// Calculate determinant
	for (Ax = 0, Ay = 0; Ax < n; ++Ax, ++Ay) {
		det *= A[ ind[Ay] ][Ax];
	}
	det = det_sign*det;
	
	// Transform A to have only 1 at diagonal
	for (Ay = 0; Ay < n; ++Ay) {
		ratio = 1 / A[ ind[Ay] ][Ay];
		for (Ax = 0; Ax < n; ++Ax) {
			A[ ind[Ay] ][Ax] *= ratio;
			B[ ind[Ay] ][Ax] *= ratio;
		}
	}
	//print_arr(ind, n);
	//print_mat_ind(A, n, n, ind);
	
	// Transform A into identity matrix
	for (Ax = 0; Ax < n; ++Ax) {
		for (Ay = Ax-1; Ay >= 0; --Ay) {
			ratio = A[ ind[Ay] ][Ax] / 1;
			//printf("%lf ", ratio);
			for (int x = 0; x < n; ++x) {
				A[ ind[Ay] ][x] -= ratio * A[ ind[Ax] ][x];
				B[ ind[Ay] ][x] -= ratio * B[ ind[Ax] ][x];
			}
		}
	}
	//print_arr(ind, n);
	//print_mat_ind(B, n, n, ind);
	
	// Reorder rows in B
	// https://medium.com/@kevingxyz/permutation-in-place-8528581a5553
	
	// Iterate through every element in the given arrays
	for (int i = 0; i < n; ++i) {
		// We look at P to see what is the new index
		int index_to_swap = ind[i];
		
		// Check index if it has already been swapped before
		while (index_to_swap < i) {
			index_to_swap = ind[index_to_swap];
		}
		
		// Swap the position of elements
		for (int x = 0; x < n; ++x) {
			swap(&B[index_to_swap][x], &B[i][x]);
		}
	}
	
	return det;
}
