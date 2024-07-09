#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

#define TRUE 1
#define FALSE 0

typedef unsigned int UINT;

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *p1, const void *p2);

// Read data to Person array (till EOF)
int read_person_array(Person *persons);

// Print Person array
void print_person_array(Person *persons, int n);

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (int that order)
int cmp_lines(const void *l1, const void *l2);

// Read lines with students' data (as text)
int read_lines(char lines[][MAX_STR_LEN]);

// Print sorted lines
void print_lines(char lines[][MAX_STR_LEN], int n);

// -------------------------------------------------

int read_int() {
	char buf[MAX_STR_LEN];
	int n;
	fgets(buf, MAX_STR_LEN, stdin);
	sscanf(buf, "%d", &n);
	return n;
}

int main(void) {
	int to_do = read_int();
	int n;
	Person persons[MAX_PERSONS];
	char lines[MAX_PERSONS][MAX_STR_LEN];
	switch (to_do) {
		case 1:
			n = read_person_array(persons);
			qsort(persons, (size_t)n, sizeof(Person), cmp_person);
			print_person_array(persons, n);
			break;
		case 2:
			n = read_lines(lines);
			qsort(lines, (size_t) n, MAX_STR_LEN, cmp_lines);
			print_lines(lines, n);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
}

int cmp_person(const void *p1, const void *p2) {  // OK
    Person person1 = *(Person *)p1;
    Person person2 = *(Person *)p2;
    
    if (person1.age != person2.age) return -(person1.age - person2.age);
    if (person1.first_name != person2.first_name) 
        return strcmp(person1.first_name, person2.first_name);
    return strcmp(person1.last_name, person2.last_name);
    
}

int read_person_array(Person *persons) {  // OK
    int n = 0;
    while (scanf("%d %s %s", &persons[n].age, 
        persons[n].first_name, persons[n].last_name) != EOF) {
        ++n;
    }
    return n;
}

void print_person_array(Person *persons, int n) {  // OK
    for (int i = 0; i < n; ++i) {
        printf("%d %s %s\n", persons[i].age, 
        persons[i].first_name, persons[i].last_name);
    }
    
}

int cmp_lines(const void *l1, const void *l2) {  // OK
    const char *line1 = (const char *)l1;
    const char *line2 = (const char *)l2;
    int age1, kobieta1 = FALSE;
    char first_name1[MAX_STR_LEN], last_name1[MAX_STR_LEN];
    int age2, kobieta2 = FALSE;
    char first_name2[MAX_STR_LEN], last_name2[MAX_STR_LEN];
    
    sscanf(line1, "%d %s %s", &age1, first_name1, last_name1);
    sscanf(line2, "%d %s %s", &age2, first_name2, last_name2);
    
    if (first_name1[strlen(first_name1)-1] == 'a') kobieta1 = TRUE;
    if (first_name2[strlen(first_name2)-1] == 'a') kobieta2 = TRUE;
    
    if (kobieta1 && !kobieta2) return -1;
    if (!kobieta1 && kobieta2) return 1;
    
    if (kobieta1 && kobieta2) return (age1 - age2);
    if (!kobieta1 && !kobieta2) return strcmp(last_name1, last_name2);
    
    return 0;
}

int read_lines(char lines[][MAX_STR_LEN]) {  // OK
    int line_count = 0;
    
    while (fgets(lines[line_count], MAX_STR_LEN, stdin) != NULL) {
        if (!strcmp(lines[line_count], "\n")) continue;
        ++line_count;
    }
    
    return line_count;
}

void print_lines(char lines[][MAX_STR_LEN], int n) {  // OK
    for (int i = 0; i < n; ++i) {
        printf("%s", lines[i]);
    }
}
