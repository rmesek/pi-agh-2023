#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 100

void swap(int *a, int *b){
	int tempnum = *a;
	*a = *b;
	*b = tempnum;
}

// Losuje jedna liczbe calkowita z przedzialu [a,b] stosujac funkcje rand() i operator %
// Jezeli a>b to zwraca INT_MIN 
// Jezeli b-a > RAND_MAX to zwraca INT_MAX 
// Jezeli a=b to zwraca a (bez wywolywania funkcji rand)
// Else zwraca liczbe z przedzialu [a,b]
int rand_from_interval(int a, int b){
	if (a > b){ return INT_MIN; }
	if (b - a > RAND_MAX){ return INT_MAX; }
	if (a == b){ return a; }
	
	return (rand() % (b-a+1)) + a;
	
}

// Losowa permutacja elementow zbioru liczb {0, 1, 2,..., n-1} (z rozkladem rownomiernym)
// wg algorytmu przedstawionego w opisie zadania
// jest zapisywana w tablicy tab.
// 0 < n <= 100, jezeli nie to elementy tablicy tab maja wartosci nieokreslone.
void rand_permutation(int n, int tab[]){
	for (int i = 0; i <= n - 1; ++i){
		tab[i] = i;
	}
	int k;
	for (int i = 0; i <= n-2; i++){
		k = rand_from_interval(i, n-1);
		swap(&tab[i], &tab[k]);
	}
}

// Metoda babelkowa sortowania n elementow tablicy tab w porzadku od wartosci najmniejszej do najwiekszej. 
// Zwraca numer iteracji petli zewnetrznej (liczony od 1), po ktorej tablica byla uporzadkowana,
// np. dla { 0 1 2 3 7 4 5 6 } -> 1, 
//     dla { 1 2 3 7 4 5 6 0 } -> 7, 
//     dla { 0 1 2 3 4 5 6 7 } -> 0. 
int bubble_sort(int n,int tab[]){
	int counter = 0;
	unsigned short int changes = 0;
	for (int i = n-1; i >= 0; --i){
		changes = 0;
		for (int j = 1; j <= i; ++j){
			if (tab[j-1] > tab[j]) {
				swap(&tab[j-1], &tab[j]);
				changes = 1;
				}
		}
		if (changes) {counter++;}
	}
	return counter;
}

int main(void) {
	int nr_testu, seed;
	int a, b, n;
	int tab[N];

	scanf("%d %d",&nr_testu, &seed);
	srand((unsigned)seed); // ustawienie ziarna generatora (dla powtarzalnosci wynikow)

	switch(nr_testu) {
		case 1:
			scanf("%d %d",&a, &b);
			for(int i = 0; i < 3; ++i) {
				printf("%d ", rand_from_interval(a, b));
			}
			printf("\n");
			break;
		case 2:
			scanf("%d", &n);
			rand_permutation(n, tab);
			for(int i = 0; i < n; ++i) printf("%d ",tab[i]);
			printf("\n");
			break;
		case 3:
			scanf("%d", &n);
			rand_permutation(n, tab);
			printf("%d\n", bubble_sort(n, tab));
			break;
		default:
			printf("NOTHING TO DO!\n");
			break;
	}
	return 0;
}

