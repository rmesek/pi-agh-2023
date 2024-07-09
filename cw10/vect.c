#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR_LEN 64

typedef struct Vector {
    void *data;
    size_t element_size;
    size_t size;
    size_t capacity;
} Vector;

typedef struct Person {
    int age;
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} Person;

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size);

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity);

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size);

// Add element to the end of the vector
void push_back(Vector *vector, void *value);

// Remove all elements from the vector
void clear(Vector *vector);

// Remove the last element from the vector
void pop_back(Vector *vector);

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, int index, void *value);

// Erase element at position index
void erase(Vector *vector, int index);

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, int(*cmp)(const void *, const void *));

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *));

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector);

// Print integer vector
void print_vector_int(Vector *vector);

// Print char vector
void print_vector_char(Vector *vector);

// Print vector of Person
void print_vector_person(Vector *vector);

// integer comparator - increasing order
int int_cmp(const void *v1, const void *v2);

// char comparator - lexicographical order (case sensitive)
int char_cmp(const void *v1, const void *v2);

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2);

// predicate: check if number is even
int is_even(void *value);

// predicate: check if char is a vowel
int is_vowel(void *value);

// predicate: check if person is older than 25
int is_older_than_25(void *person);

// -------------------------------------------------------------

void read_int(void *value) {
    scanf("%d", (int *) value);
}

void read_char(void *value) {
    char c[2];
    scanf("%s", c);
    *(char *) value = c[0];
}

void read_person(void *value) {
    Person *person = (Person *) value;
    scanf("%d %s %s", &person->age, person->first_name, person->last_name);
}

void vector_test(Vector *vector, int n, void(*read)(void *),
                 int (*cmp)(const void *, const void *), int(*predicate)(void *)) {
    char op[2];
    int index;
    size_t size;
    void *v = malloc(vector->element_size);
    for (int i = 0; i < n; ++i) {
        scanf("%s", op);
        switch (op[0]) {
            case 'p': // push_back
                read(v);
                push_back(vector, v);
                break;
            case 'i': // insert
                scanf("%d", &index);
                read(v);
                insert(vector, index, v);
                break;
            case 'e': // erase
                scanf("%d", &index);
                read(v);
                erase(vector, index);
                erase_value(vector, v, cmp);
                break;
            case 'd': // erase (predicate)
                erase_if(vector, predicate);
                break;
            case 'r': // resize
                scanf("%zu", &size);
                resize(vector, size);
                break;
            case 'c': // clear
                clear(vector);
                break;
            case 'f': // shrink
                shrink_to_fit(vector);
                break;
            case 's': // sort
                qsort(vector->data, vector->size,
                      vector->element_size, cmp);
                break;
            case 'h': // debug
                printf("\n\tvector->data \t\t %p", vector->data);
                printf("\n\tvector->element_size \t\t %ld", vector->element_size);
                printf("\n\tvector->size \t\t %ld", vector->size);
                printf("\n\tvector->capacity \t\t %ld\n", vector->capacity);
                printf("--START--\n");
                print_vector_int(vector);
                printf("\n--END--\n");
                break;
            default:
                printf("No such operation: %s\n", op);
                break;
        }
    }
    free(v);
}

int main(void) {
    int to_do, n;
    Vector vector_int, vector_char, vector_person;

    scanf("%d%d", &to_do, &n);

    switch (to_do) {
        case 1:
            init_vector(&vector_int, 4, sizeof(int));
            vector_test(&vector_int, n, read_int, int_cmp, is_even);
            print_vector_int(&vector_int);
            free(vector_int.data);
            break;
        case 2:
            init_vector(&vector_char, 2, sizeof(char));
            vector_test(&vector_char, n, read_char, char_cmp, is_vowel);
            print_vector_char(&vector_char);
            free(vector_char.data);
            break;
        case 3:
            init_vector(&vector_person, 2, sizeof(Person));
            vector_test(&vector_person, n, read_person, person_cmp, is_older_than_25);
            print_vector_person(&vector_person);
            free(vector_person.data);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }

    return 0;
}

void init_vector(Vector *vector, size_t block_size, size_t element_size) {  // OK
    vector->data = malloc(block_size * element_size);
    vector->element_size = element_size;
    vector->size = 0;
    vector->capacity = block_size;
}

void reserve(Vector *vector, size_t new_capacity) {  // OK
    if (vector->size >= new_capacity) return;

    vector->data = realloc(vector->data, (new_capacity + 1) * (vector->element_size));
    vector->capacity = new_capacity;
}

void resize(Vector *vector, size_t new_size) { // OK
    size_t old_size = vector->size;

    if (old_size < new_size) {
        void *v = calloc(1, vector->element_size);
        for (int i = 0; i < new_size - old_size; ++i) {
            push_back(vector, v);
        }
        free(v);
    } else if (old_size > new_size) {
        vector->size = new_size;
        if (vector->size <= 0.5 * (double) vector->capacity) {
            shrink_to_fit(vector);
        }
    }
}

void push_back(Vector *vector, void *value) {  // OK MEMCPY
    if (vector->size == vector->capacity) {
        if (vector->capacity == 0) {
            reserve(vector, 1);
        } else {
            reserve(vector, vector->capacity * 2);
        }
    }
    char *p = vector->data;
    p += (vector->element_size * vector->size);
    memcpy(p, value, vector->element_size);  // OR MEMMOVE

    ++(vector->size);
}

void clear(Vector *vector) {  // OK
    vector->size = 0;
    shrink_to_fit(vector);
}

void pop_back(Vector *vector) {  // OK?
    if (vector->size == 0) return;

    --(vector->size);
    if (vector->size <= 0.5 * (double) vector->capacity) {
        shrink_to_fit(vector);
    }
}

void insert(Vector *vector, int index, void *value) {  // OK
    if (vector->size == vector->capacity) {
        if (vector->capacity == 0) {
            reserve(vector, 1);
        } else {
            reserve(vector, vector->capacity * 2);
        }
    }
    char *p = vector->data;
    p += (vector->element_size * (size_t) index);
    memmove(p + (vector->element_size), p, vector->element_size * (vector->size - (size_t) index));
    memmove(p, value, vector->element_size);

    ++(vector->size);
}

void erase(Vector *vector, int index) {  // OK
    if (vector->size == 0 || index < 0) return;

    char *p = vector->data;
    p += (vector->element_size * (size_t) index);

    memmove(p, p + (vector->element_size), vector->element_size * (vector->size - (size_t) index));

    --(vector->size);
    if (vector->size <= 0.5 * (double) vector->capacity) {
        shrink_to_fit(vector);
    }
}

void erase_value(Vector *vector, void *value, int(*cmp)(const void *, const void *)) {  // OK
    if (vector->size == 0) return;

    char *p = (char *) vector->data;
    size_t index = 0;
    while (p != (char *) vector->data + vector->size * vector->element_size) {
        if (cmp(p, value) == 0) {
            memmove(p, p + (vector->element_size), vector->element_size * (vector->size - (size_t) index));
            --(vector->size);
        } else {
            p += vector->element_size;
            ++index;
        }
    }
    if (vector->size <= 0.5 * (double) vector->capacity) {
        shrink_to_fit(vector);
    }
}

void erase_if(Vector *vector, int (*predicate)(void *)) {  // OK
    if (vector->size == 0) return;

    char *p = (char *) vector->data;
    size_t index = 0;
    while (p != (char *) vector->data + vector->size * vector->element_size) {
        if (predicate(p)) {
            memmove(p, p + (vector->element_size), vector->element_size * (vector->size - (size_t) index));
            --(vector->size);
        } else {
            p += vector->element_size;
            ++index;
        }
    }
    if (vector->size <= 0.5 * (double) vector->capacity) {
        shrink_to_fit(vector);
    }
}

// Czy realloc nie zepsuje danych?
void shrink_to_fit(Vector *vector) {  // OK
    vector->data = realloc(vector->data, vector->size * vector->element_size);
    vector->capacity = vector->size;
}

void print_vector_int(Vector *vector) {  // OK
    printf("%ld\n", vector->capacity);
    for (int *p = (int *) vector->data;
         p < (int *) ((int *) vector->data + vector->size); ++p) {
        printf("%d ", *p);
    }
}

void print_vector_char(Vector *vector) {  // OK
    printf("%ld\n", vector->capacity);
    for (char *p = (char *) vector->data;
         p < (char *) ((char *) vector->data + vector->size); ++p) {
        printf("%c ", *p);
    }
}

void print_vector_person(Vector *vector) {  // OK
    printf("%ld\n", vector->capacity);
    for (Person *p = (Person *) vector->data;
         p < (Person *) ((Person *) vector->data + vector->size); ++p) {
        printf("%d %s %s\n", p->age, p->first_name, p->last_name);
    }
}

int int_cmp(const void *v1, const void *v2) {  // OK
    int *p1 = (int *) v1;
    int *p2 = (int *) v2;
    if (*p1 < *p2) return -1;
    if (*p1 > *p2) return 1;
    return 0;
}

int char_cmp(const void *v1, const void *v2) {  // OK
    char *p1 = (char *) v1;
    char *p2 = (char *) v2;
    if (*p1 < *p2) return -1;
    if (*p1 > *p2) return 1;
    return 0;
}

int person_cmp(const void *p1, const void *p2) {  // OK
    Person person1 = *(Person *) p1;
    Person person2 = *(Person *) p2;

    if (person1.age != person2.age) return -(person1.age - person2.age);
    if (strcmp(person1.first_name, person2.first_name) != 0)
        return strcmp(person1.first_name, person2.first_name);
    return strcmp(person1.last_name, person2.last_name);
}

int is_even(void *value) {  // OK
    return !(*(int *) value % 2);
}

int is_vowel(void *value) {  // OK
    char c = *(char *) value;
    c = (char) tolower(c);

    if (c == 'a' || c == 'e' || c == 'i' || c == 'o'
        || c == 'u' || c == 'y')
        return 1;
    return 0;
}

int is_older_than_25(void *person) {  // OK
    if (((Person *) person)->age > 25) return 1;
    return 0;
}
