#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

typedef union data_union {
    int int_data;
    char char_data;
    void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);

typedef void (*DataPFp)(data_union *);

typedef int (*CompareDataFp)(data_union, data_union);

typedef int (*HashFp)(data_union, int);

typedef struct ht_element {
    struct ht_element *next;
    data_union data;
} ht_element;

typedef struct hash_table {
    int size;
    int no_elements;
    ht_element **ht;
    DataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    HashFp hash_function;
    DataPFp modify_data;
} hash_table;

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) return ptr;
    exit(MEMORY_ALLOCATION_ERROR);
}

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
             CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
    p_table->ht = calloc((size_t)size, sizeof(ht_element));
    p_table->size = size;
    p_table->no_elements = 0;
    p_table->dump_data = dump_data;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->hash_function = hash_function;
    p_table->modify_data = modify_data;
}

// print elements of the list with hash n
void dump_list(const hash_table *p_table, int n) {
    for (ht_element *p = (p_table->ht)[n]; p != NULL; p = p->next) {
        p_table->dump_data(p->data);
    }
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
    free_data(to_delete->data);
}

// free all elements from the table (and the table itself)
void free_table(hash_table *p_table) {
    for (int i = 0; i < p_table->size; ++i) {
        for (ht_element *p = (p_table->ht)[i]; p != NULL;) {
            ht_element *p_next = p->next;
            if (p_table->free_data != NULL) free_element(p_table->free_data, p);
            free(p);
            p = p_next;
        }
    }
    free(p_table->ht);
}

void free_table_without_element(hash_table *p_table) {
    for (int i = 0; i < p_table->size; ++i) {
        for (ht_element *p = (p_table->ht)[i]; p != NULL;) {
            ht_element *p_next = p->next;
            free(p);
            p = p_next;
        }
    }
    free(p_table->ht);
}

// calculate hash function for integer k
int hash_base(int k, int size) {
    static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
    double tmp = k * c;
    return (int) floor(size * (tmp - floor(tmp)));
}

void rehash(hash_table *);

// insert element
void insert_element(hash_table *p_table, data_union *data) {
    int hash = p_table->hash_function(*data, p_table->size);

    // CHECK IF ELEMENT EXISTS
    for (ht_element *p = (p_table->ht)[hash]; p != NULL; p = p->next) {
        if (p_table->compare_data(p->data, *data) == 0) {
            //printf("HASH: %d|", hash);
            //p_table->dump_data(*data);
            if (p_table->modify_data != NULL) p_table->modify_data(&p->data);
            if (p_table->free_data != NULL) p_table->free_data(*data);
            return;
        }
    }

    ht_element *ht_element_to_insert = malloc(sizeof(ht_element));
    ht_element_to_insert->data = *data;
    ht_element_to_insert->next = (p_table->ht)[hash];
    (p_table->ht)[hash] = ht_element_to_insert;

    p_table->no_elements += 1;

    if (p_table->no_elements / p_table->size > MAX_RATE) {
        rehash(p_table);
    }
}

void rehash(hash_table *p_table) {  // TODO: NIEDZIALA
    hash_table p_table_copy = *p_table;
    p_table->ht = calloc((size_t) p_table->size * 2, sizeof(ht_element));
    p_table->size *= 2;
    p_table->no_elements = 0;
    for (int i = 0; i < p_table_copy.size; ++i) {
        for (ht_element *p = (p_table_copy.ht)[i]; p != NULL; p = p->next) {
//            printf("HASH: %d", p_table->hash_function(p->data, p_table->size));
            insert_element(p_table, &p->data);
        }
    }
    free_table_without_element(&p_table_copy);  // TODO: Przy word nie działa
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data, int *first) {

}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
    int hash = p_table->hash_function(*data, p_table->size);
    ht_element *ht_p = (p_table->ht)[hash];

    while (ht_p != NULL) {
        if (p_table->compare_data(ht_p->data, *data) == 0) {
            return ht_p;
        }
        ht_p = ht_p->next;
    }
    return NULL;
}


// remove element
void remove_element(hash_table *p_table, data_union data) {
    int hash = p_table->hash_function(data, p_table->size);
    ht_element *ht_p = (p_table->ht)[hash];
    ht_element *ht_p_prev = NULL;

    while (ht_p != NULL) {
        ht_element *ht_p_next = ht_p->next;
        ht_element *ht_p_prev_next = ht_p;
        if (p_table->compare_data(ht_p->data, data) == 0) {
            if (ht_p_prev == NULL) {  // 1 ELEMENT
                (p_table->ht)[hash] = ht_p->next;
            } else if (ht_p->next == NULL) {  // OSTATNI ELEMENT
                ht_p_prev->next = NULL;
            } else {
                ht_p_prev->next = ht_p->next;
            }
            if (p_table->free_data != NULL) free_element(p_table->free_data, ht_p);
            free(ht_p);
            p_table->no_elements -= 1;
        }
        ht_p_prev = ht_p_prev_next;
        ht_p = ht_p_next;
    }
}

// type-specific definitions

// int element

int hash_int(data_union data, int size) {
    return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
    return (a.int_data - b.int_data);
}

// read int value and insert to the union
data_union create_data_int() {
    data_union data;
    scanf("%d", &data.int_data);
    return data;
}

// char element

int hash_char(data_union data, int size) {
    return hash_base((int) data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
    return (a.char_data - b.char_data);
}

// read char value and insert to the union
data_union create_data_char() {
    data_union data;
    scanf(" %c", &data.char_data);
    return data;
}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word(data_union data) {
    DataWord *data_word = (DataWord *) data.ptr_data;
    printf("%s %d ", data_word->word, data_word->counter);
}

void free_word(data_union data) {
    DataWord *data_word = (DataWord *) data.ptr_data;
    //printf("STRING %s ", data_word->word);
    free(data_word->word);
    free(data_word);
}

int cmp_word(data_union a, data_union b) {
    const char *word_a = ((DataWord *) a.ptr_data)->word;
    const char *word_b = ((DataWord *) b.ptr_data)->word;
    return strcmp(word_a, word_b);
}

int hash_word(data_union data, int size) {
    int s = 0;
    DataWord *dw = (DataWord *) data.ptr_data;
    char *p = dw->word;
    while (*p) {
        s += *p++;
    }
    return hash_base(s, size);
}

// increase the counter
void modify_word(data_union *data) {
    DataWord *data_word = (DataWord *) data->ptr_data;
    ++(data_word->counter);
    //dump_word(*data);
}

void str_tolower(char *s) {
    for (; *s; ++s)
        *s = (char)tolower(*s);
}

// allocate DataWord structure and insert to the union
data_union create_data_word(char *value) {
    data_union data;
    DataWord *data_word = (DataWord *) malloc(sizeof(DataWord));
    str_tolower(value);
    data_word->word = strndup(value, strlen(value));
    data_word->counter = 1;
    data.ptr_data = data_word;
    return data;
}

// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
    const char *delimiters = ",.?!:;-\t\n\b\r ";
    char *line = NULL;
    size_t len;
    while (getline(&line, &len, stream) > 0) {
        char *token = NULL;
        for (token = strtok(line, delimiters); token != NULL; token = strtok(NULL, delimiters)) {
            data_union data = create_data_word(token);
            insert_element(p_table, &data);

        }
        free(line);
        line = NULL;
    }
    free(line);
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
    char op;
    data_union data;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
        data = create_data();
        switch (op) {
            case 'r':
                remove_element(p_table, data);
                break;
            case 'i':
                insert_element(p_table, &data);
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n, index;
    hash_table table;
    char buffer[BUFFER_SIZE];
    data_union data;

    scanf("%d", &to_do);
    switch (to_do) {
        case 1: // test integer table
            scanf("%d %d", &n, &index);
            init_ht(&table, 4, dump_int, NULL,
                    cmp_int, hash_int, NULL);
            table_test(&table, n, create_data_int);
            printf("%d\n", table.size);
            dump_list(&table, index);
            free_table(&table);
            break;
        case 2: // test char table
            scanf("%d %d", &n, &index);
            init_ht(&table, 4, dump_char, NULL,
                    cmp_char, hash_char, NULL);
            table_test(&table, n, create_data_char);
            printf("%d\n", table.size);
            dump_list(&table, index);
            free_table(&table);
            break;
        case 3: // read words from text, insert into table, and print
            scanf("%s", buffer);
            init_ht(&table, 8, dump_word, free_word,
                    cmp_word, hash_word, modify_word);
            stream_to_ht(&table, stdin);
            printf("%d\n", table.size);
            data = create_data_word(buffer);
            ht_element *e = get_element(&table, &data);
            if (e) table.dump_data(e->data);
            if (table.free_data) table.free_data(data);
            free_table(&table);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}
