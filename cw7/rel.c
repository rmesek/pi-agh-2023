#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000
#define TRUE 1
#define FALSE 0

typedef struct {
	int first;
	int second;
} pair;

void sort_relation(pair*, int);

int compare_pair(const void* , const void*);

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

 //Case 1:
 //The relation R is reflexive if xRx for every x in X
int is_reflexive(pair*, int);
 //The relation R on the set X is called irreflexive
 //if xRx is false for every x in X
int is_irreflexive(pair*, int);
 //A binary relation R over a set X is symmetric if:
 //for all x, y in X xRy <=> yRx
int is_symmetric(pair*, int);
 //A binary relation R over a set X is antisymmetric if:
 //for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair*, int);
 //A binary relation R over a set X is asymmetric if:
 //for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair*, int);
 //A homogeneous relation R on the set X is a transitive relation if:
 //for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair*, int);

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair*, int);
// A total order relation is a partial order relation that is connected
int is_total_order(pair*, int);
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_strongly_connected(pair*, int);
int compareints (const void * a, const void * b);
int find_max_elements(pair*, int, int*);
int find_min_elements(pair*, int, int*);
int get_domain(pair*, int, int*);

// Case 3:
int composition (pair*, int, pair*, int, pair*);

int cmp (pair p1, pair p2) {
	if (p1.first == p2.first) return p1.second - p2.second;
	return p1.first - p2.first;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair*);

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

void print_relation(pair*, int);

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int max_elements[MAX_REL_SIZE];
			int min_elements[MAX_REL_SIZE];
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size,
			   relation_2, size_2, comp_relation));
			break;
		default:
			//pair x;
			//x.first = 9, x.second = 7;
			//print_relation(relation, size);
			//size = add_relation(relation, size, x);
			//sort_relation(relation, size);
			//print_relation(relation, size);
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

void sort_relation(pair* relation, int n) {  // OK
	qsort(relation, (size_t)n, sizeof(pair), compare_pair);
}

int compare_pair(const void* a, const void* b) {  // OK
	const pair* p1 = (pair*)a;
	const pair* p2 = (pair*)b;
	
	if (p1->first == p2->first) 
		return p1->second - p2->second;
	return p1->first - p2->first;
}

int add_relation(pair* relation, int n, pair x) {  // OK
	if (n+1 >= MAX_REL_SIZE) {
		printf("NOT ENOUGH SPACE, INCREASE MAX_REL_SIZE!\n");
		return -1;
	}
	
	for (int i = 0; i < n; ++i) {
		if (relation[i].first == x.first 
			&& relation[i].second == x.second) {
			return n;
			}
	}
	
	relation[n].first = x.first;
	relation[n].second = x.second;
	
	//sort_relation(relation, n+1);
	
	return n+1;
}

int is_reflexive(pair* relation, int n) {  // SLOW-OK
	int Found;
	for (int x = 0; x < n; ++x) {
		Found = FALSE;
		for (int y = 0; y < n; ++y) {
			if (relation[x].first == relation[y].second) {
				Found = TRUE;
				break;
			}
		}
		if (!Found) return FALSE;
	}
	return TRUE;
	
	//if (n == 0) return TRUE;
	
	//int x = relation[0].first;
	//int Found = FALSE;
	
	//for (int i = 0; i < n; ++i) {		
		//if (relation[i].first != x) {
			//x = relation[i].first;
			//if (!Found) return FALSE;
			//Found = FALSE;
		//}
		
		//if (relation[i].second == x) {
			//Found = TRUE;
		//}
	//}
	
	//return Found;
}

int is_irreflexive(pair* relation, int n) {	 // OK
	for (int i = 0; i < n; ++i) {
		if (relation[i].first == relation[i].second) return FALSE;
	}
	return TRUE;
}

int is_symmetric(pair* relation, int n) {  // SLOW-OK
	int Found;
	for (int i = 0; i < n; ++i) {
		Found = FALSE;
		for (int j = 0; j < n; ++j) {
			if (relation[i].first == relation[j].second 
				&& relation[i].second == relation[j].first) {
					Found = TRUE;
					break;
				}
		}
		if (!Found) return FALSE;
	}
	return TRUE;
}

int is_antisymmetric(pair* relation, int n) {  // SLOW-OK
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (relation[i].first == relation[j].second 
				&& relation[i].second == relation[j].first
				&& !(relation[i].first == relation[j].first 
				&& relation[i].second == relation[j].second)) {
					return FALSE;
				}
		}
	}
	return TRUE;
}

int is_asymmetric(pair* relation, int n) {  // SLOW-OK
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (relation[i].first == relation[j].second 
				&& relation[i].second == relation[j].first) {
					return FALSE;
				}
		}
	}
	return TRUE;
}

int is_transitive(pair* relation, int n) {  // SLOW-OK
	int Found;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (relation[i].second == relation[j].first) {
				Found = FALSE;
				for (int k = 0; k < n; ++k) {
					if (relation[i].first == relation[k].first
						&& relation[j].second == relation[k].second) {
							Found = TRUE;
							break;
						}
				}
				if (!Found) return FALSE;
			}
		}
	}
	return TRUE;
}

int is_partial_order(pair* relation, int n) {  // OK
	if (is_reflexive(relation, n)
		&& is_antisymmetric(relation, n)
		&& is_transitive(relation, n))
		return TRUE;
	return FALSE;
}

int is_total_order(pair* relation, int n) {  // OK
	if (is_reflexive(relation, n)
		&& is_antisymmetric(relation, n)
		&& is_transitive(relation, n)
		&& is_strongly_connected(relation, n))
		return TRUE;
	return FALSE;
}

int is_strongly_connected(pair* relation, int n) {  // OK?
	return is_irreflexive(relation, n);
}

int compareints (const void * a, const void * b) {  // OK
    return ( *(int*)a - *(int*)b );
}

int find_max_elements(pair* relation, int n, int* max_elements) {  // OK?
    int first, second;
    int maxP, count = 0;
    
    for (int i = 0; i < n; ++i) {
        second = relation[i].second;
        maxP = TRUE;
        for (int j = 0; j < n; ++j) {
            first = relation[j].first;
            if (first == second && relation[j].second != second) {
                maxP = FALSE;
                break;
            }
        }
        
        for (int i = 0; i < count; ++i) {
                if (max_elements[i] == second) maxP = FALSE;
            }
        
        if (maxP) {
            max_elements[count] = second;
            ++count;
        }
    }
    
    qsort(max_elements, (size_t) count, sizeof(int), compareints);
    
    return count;
}

int find_min_elements(pair* relation, int n, int* min_elements) {  // OK?
    int first, second;
    int minP, count = 0;
    
    for (int i = 0; i < n; ++i) {
        first = relation[i].first;
        minP = TRUE;
        for (int j = 0; j < n; ++j) {
            second = relation[j].second;
            if (second == first && relation[j].first != first) {
                minP = FALSE;
                break;
            }
        }
        
        for (int i = 0; i < count; ++i) {
                if (min_elements[i] == first) minP = FALSE;
            }
        
        if (minP) {
            min_elements[count] = first;
            ++count;
        }
    }
    
    qsort(min_elements, (size_t) count, sizeof(int), compareints);
    
    return count;
}

int get_domain(pair* relation, int n, int* domain) {  // OK
	int first, second;
    int firstF, secondF;
    int count = 0;
    
    for (int i = 0; i < n; ++i) {
        first = relation[i].first;
        second = relation[i].second;
        firstF = secondF = FALSE;
        for(int j = 0; j < count; ++j) {
            if (first == domain[j]) firstF = TRUE;
            if (second == domain[j]) secondF = TRUE;
        }
        if (!firstF) {
            domain[count] = first;
            ++count;
        }
        if (!secondF && first != second) {
            domain[count] = second;
            ++count;
        }
    }
    
    qsort(domain, (size_t) count, sizeof(int), compareints);
    
    return count;
}

int composition (pair* relation, int n, 
	pair* relation2, int n2, pair* comp_relation) {  // OK
	int n3 = 0;
    int first, second, first2, second2;
    int duplicate;
    pair cpair;
    
    for (int i = 0; i < n; ++i) {
        first = relation[i].first;
        second = relation[i].second;
        
        for (int j = 0; j < n2; ++j) {
            first2 = relation2[j].first;
            second2 = relation2[j].second;
            duplicate = FALSE;
            
            if (second == first2) {
                cpair.first = first;
                cpair.second = second2;
                
                for (int k = 0; k < n3; ++k) {
                    if (comp_relation[k].first == cpair.first 
                    && comp_relation[k].second == cpair.second) {
                        duplicate = TRUE;
                    }
                }
                
                if (!duplicate) {
                    comp_relation[n3] = cpair;
                    ++n3;
                }
            }
        }
    }
    
    return n3;
}

int read_relation(pair* relation) {  // OK
	int n;
	scanf("%d", &n);
	if (n >= MAX_REL_SIZE) {
		printf("NOT ENOUGH SPACE, INCREASE MAX_REL_SIZE!\n");
		return -1;
	}
	
	for (int i = 0; i < n; ++i) {
		scanf("%d %d", &relation[i].first, &relation[i].second);
	}
	
	//sort_relation(relation, n);  // SORT RELATION
	
	return n;
}

void print_relation(pair* relation, int n) {  // OK
	for (int i = 0; i < n; ++i) {
		printf("%d %d\n", relation[i].first, relation[i].second);
	}
}
