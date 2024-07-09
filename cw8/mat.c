#include <stdio.h>

#include <stdlib.h>

#include <stddef.h>

#include <string.h>

#define TAB_SIZE 1000
#define BUF_SIZE 1000

#define MAX_LINE_LEN 128

double get(int cols, int row, int col,
  const double * A);

void set(int cols, int row, int col, double * A, double value);

void prod_mat(int rowsA, int colsA, int colsB, double * A, double * B, double * AB);

void read_mat(int rows, int cols, double * t);

void print_mat(int rows, int cols, double * t);

int read_char_lines(char * tab[]);

void write_char_line(char * tab[], int n);

void delete_lines(char * tab[], int line_count);

int read_dbl_lines_v1(double * ptr_tab[]);

void write_dbl_line_v1(double * ptr_tab[], int n);

int main(void) {
  int to_do;

  scanf("%d", & to_do);

  double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
  int n, lines_counter, rowsA, colsA, rowsB, colsB;
  char * char_lines_table[TAB_SIZE];
  double series_table[TAB_SIZE];
  double * ptr_table[TAB_SIZE];

  switch (to_do) {
  case 1:
    scanf("%d %d", & rowsA, & colsA);
    read_mat(rowsA, colsA, A);
    scanf("%d %d", & rowsB, & colsB);
    read_mat(rowsB, colsB, B);
    prod_mat(rowsA, colsA, colsB, A, B, C);
    print_mat(rowsA, colsB, C);
    break;
  case 2:
    scanf("%d", & n);
    ptr_table[0] = series_table;
    lines_counter = read_dbl_lines_v1(ptr_table);
    write_dbl_line_v1(ptr_table, n);
    break;
  case 3:
    scanf("%d", & n);
    lines_counter = read_char_lines(char_lines_table);
    write_char_line(char_lines_table, n);
    delete_lines(char_lines_table, lines_counter);
    break;
  default:
    printf("NOTHING TO DO FOR %d\n", to_do);
  }
  return 0;
}

double get(int cols, int row, int col,
  const double * A) { // OK
  return A[cols * row + col];
}

void set(int cols, int row, int col, double * A, double value) { // OK
  A[cols * row + col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, double * A, double * B, double * AB) { // OK
  int colAB = 0, rowAB = 0;
  int colA, rowA, colB, rowB;
  double product;

  for (rowA = 0; rowA < rowsA; ++rowA) {
    for (colB = 0; colB < colsB; ++colB) {
      product = 0;
      for (colA = 0, rowB = 0; colA < colsA; ++colA, ++rowB) {
        product += get(colsA, rowA, colA, A) * get(colsB, rowB, colB, B);
      }
      colAB = colB, rowAB = rowA;
      //printf("set(%d, %d, %d, AB, %f)\n", rowsA, rowAB, colAB, product);
      set(rowsA, rowAB, colAB, AB, product);
    }
  }
}

void read_mat(int rows, int cols, double * t) { // OK
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      double n;
      scanf("%lf", & n);
      set(cols, row, col, t, n);
    }
  }
}

void print_mat(int rows, int cols, double * t) { // OK
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      printf("%.2lf ", get(cols, row, col, t));
    }
    printf("\n");
  }
}

int read_char_lines(char * tab[]) { // OK
  int line_count = 0;
  char line[MAX_LINE_LEN];
  char * pStr;

  int c;
  while ((c = getchar()) != '\n' && c != EOF);

  while (fgets(line, MAX_LINE_LEN, stdin) != NULL) {
    pStr = (char * ) malloc((strlen(line) + 1) * sizeof(char));
    strcpy(pStr, line);
    tab[line_count] = pStr;

    ++line_count;

  }

  return line_count;
}

void write_char_line(char * tab[], int n) { // OK
  printf("%s", tab[n - 1]);
}

void delete_lines(char * tab[], int line_count) { // OK
  for (int i = 0; i < line_count; ++i) {
    free(tab[i]);
  }
}

int read_dbl_lines_v1(double * ptr_tab[]) { // OK
  int lines = 0;
  double d;
  double * pDouble = ptr_tab[0];
  char line[MAX_LINE_LEN];
  char * pStart, * pEnd;

  while (fgets(line, MAX_LINE_LEN, stdin) != NULL) {
    pStart = line;
    if ( * pStart == '\n') continue;
    while (1) {
      d = strtod(pStart, & pEnd);
      if (pStart == pEnd) {
        break;
      }
      * pDouble = d;

      ++pDouble;
      pStart = pEnd;
    }
    ++lines;
    ptr_tab[lines] = pDouble;
  }
  ptr_tab[lines + 1] = pDouble + 1;

  return lines;
}

void write_dbl_line_v1(double * ptr_tab[], int n) { // OK
  double * dStart = ptr_tab[n - 1];
  while (dStart != ptr_tab[n]) {
    printf("%.2lf ", * dStart);
    ++dStart;
  }
}
