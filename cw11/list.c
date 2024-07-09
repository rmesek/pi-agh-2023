#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

struct tagList;

typedef void (*ConstDataFp)(const void *);

typedef void (*DataFp)(void *);

typedef int (*CompareDataFp)(const void *, const void *);

typedef void (*InsertInOrder)(struct tagList *, void *);

typedef struct tagListElement {
    struct tagListElement *next;
    void *data;
} ListElement;

typedef struct tagList {
    ListElement *head;
    ListElement *tail;
    ConstDataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    InsertInOrder insert_sorted;
} List;

// -----------------------------------------------------------------
// generic functions - they are common for all instances of the list
// (independent of the data type)
// -----------------------------------------------------------------

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
               CompareDataFp compare_data, InsertInOrder insert_sorted) {  // OK
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->insert_sorted = insert_sorted;

}

// Print all elements of the list
void dump_list(const List *p_list) {  // OK???
    ListElement *p;

    if (p_list->dump_data == NULL) return;

    for (p = p_list->head; p != NULL; p = p->next) {
        p_list->dump_data(p->data);
    }
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {  // OK???
    ListElement *p;

    if (!p_list->dump_data || !p_list->compare_data) return;

    for (p = p_list->head; p != NULL; p = p->next) {
        if (p_list->compare_data(p->data, data) == 0)
            p_list->dump_data(p->data);
    }
}

// Free all elements of the list
void free_list(List *p_list) {  // OK
    ListElement *p = p_list->head;

    while (p != NULL) {
        ListElement *to_delete = p;
        p = p->next;
        if (p_list->free_data) p_list->free_data(to_delete->data);
        free(to_delete);
    }

    p_list->head = NULL;
    p_list->tail = NULL;
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data) {  // OK
    ListElement *element = (ListElement *) malloc(sizeof(ListElement));
    if (element == NULL) exit(MEMORY_ALLOCATION_ERROR);
    element->data = data;
    element->next = p_list->head;
    p_list->head = element;
    // if list was empty
    if (!p_list->tail) p_list->tail = p_list->head;
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {  // OK
    ListElement *element = (ListElement *) malloc(sizeof(ListElement));
    if (element == NULL) exit(MEMORY_ALLOCATION_ERROR);
    element->data = data;
    element->next = NULL;
    if (p_list->tail) p_list->tail->next = element;
    p_list->tail = element;
    if (!p_list->head) p_list->head = p_list->tail;
}

// Remove the first element
void pop_front(List *p_list) {
    if (!p_list->head) return;

    void *data = p_list->head->data;
    ListElement *to_delete = p_list->head;
    p_list->head = p_list->head->next;

    p_list->free_data(data);
    free(to_delete);

    if (p_list->head == NULL) p_list->tail = NULL;
}

// Reverse the list
void reverse(List *p_list) {
    ListElement *p_prev, *p, *p_next;
    if (!p_list->head) return;
    if (!p_list->head->next) return;

    p_prev = p_list->head;
    p = p_prev->next;

    while (p != NULL) {
        p_next = p->next;

        p->next = p_prev;

        p_prev = p;
        p = p_next;
    }
    p_list->tail = p_list->head;
    p_list->tail->next = NULL;
    p_list->head = p_prev;
}

// insert element preserving the ordering (defined by insert_sorted function)
void insert_in_order(List *p_list, void *data) {
    p_list->insert_sorted(p_list, data);
}

// find element in sorted list after which to insert given element
ListElement *find_insertion_point(const List *p_list, ListElement *p_element) {  // ???
    ListElement *insertion_point = NULL;
    ListElement *p_current;

    for (p_current = p_list->head; p_current != NULL; p_current = p_current->next) {
        if (p_list->compare_data(p_current->data, p_element->data) <= 0) {
            insertion_point = p_current;
        }
    }
    return insertion_point;
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
    if (previous == NULL) {
        push_front(p_list, data);
        return;
    }

    ListElement *node = (ListElement *) malloc(sizeof(ListElement));
    node->data = data;
    node->next = previous->next;
    previous->next = node;
}

// Insert element preserving order (no counter)
void insert_elem(List *p_list, void *p_data) {
    ListElement *tmp_node = (ListElement *) malloc(sizeof(ListElement));
    tmp_node->data = p_data;
    ListElement *previous = find_insertion_point(p_list, tmp_node);
    if (previous == NULL || p_list->compare_data(previous->data, p_data) != 0)
        push_after(p_list, p_data, previous);
    free(tmp_node);
}

// ---------------------------------------------------------------
// type-specific definitions
// ---------------------------------------------------------------

// int element

typedef struct DataInt {
    int id;
} DataInt;

void dump_int(const void *d) {
    const DataInt *data = (const DataInt *) d;
    printf("%d ", data->id);
}

void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    DataInt *a_int = (DataInt *) a;
    DataInt *b_int = (DataInt *) b;
    return a_int->id - b_int->id;
}

DataInt *create_data_int(int v) {
    DataInt *p_int = (DataInt *) malloc(sizeof(DataInt));
    p_int->id = v;
    return p_int;
}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word(const void *d) {
    const DataWord *data = (const DataWord *) d;
    printf("%s\n", data->word);
}

char *word_to_lowercase(char *word) {
//    printf(" |STRING: %s", word);
//    return word;
    size_t len = strlen(word);
    char *low_word = (char *) malloc((len + 1) * sizeof(char));
    for (size_t i = 0; i < len + 1; ++i) low_word[i] = (char) tolower(word[i]);
    return low_word;
}

void dump_word_lowercase(const void *d) {
    const DataWord *data = (const DataWord *) d;
    char *word = word_to_lowercase(data->word);
    printf("%s\n", word);
    free(word);
}

void free_word(void *d) {
    DataWord *word = (DataWord *) d;
    free(word->word);
    free(word);
}

// compare words case-insensitive
int cmp_word_alphabet(const void *a, const void *b) {
    const char *word_a = word_to_lowercase(((const DataWord *) (a))->word);
    const char *word_b = word_to_lowercase(((const DataWord *) (b))->word);

    int result = strcmp(word_a, word_b);
    free((void *) word_a);
    free((void *) word_b);
    return result;
}

int cmp_word_counter(const void *a, const void *b) {
    return ((const DataWord *) a)->counter - ((const DataWord *) b)->counter;
}

// insert element; if present increase counter
void insert_elem_counter(List *p_list, void *Sdata) {
    ListElement *p_current = p_list->head;
    ListElement *p_insert = NULL;

    while (p_current != NULL) {
        if (p_list->compare_data(p_current->data, Sdata) <= 0) {
            p_insert = p_current;
        }
        p_current = p_current->next;
    }

    if (p_insert != NULL && (p_list->compare_data(p_insert->data, Sdata) == 0)) {
        ++((DataWord *) (p_insert->data))->counter;
        free(((DataWord *)Sdata)->word);
        free(Sdata);
        return;
    }

    push_after(p_list, Sdata, p_insert);
}

// read text, parse it to words, and insert those words to the list
// in order given by the last parameter (0 - read order,
// 1 - alphabetical order)
void stream_to_list(List *p_list, FILE *stream, int order) {
    const char *delimiters = ",.?!:;-\t\n\b\r ";
    char *line = NULL;
    size_t len;
    while (getline(&line, &len, stream) > 0) {
        char *token = NULL;
        for (token = strtok(line, delimiters); token != NULL; token = strtok(NULL, delimiters)) {
            DataWord *data = (DataWord *) malloc(sizeof(DataWord));
//            printf("STRING: %s ", token);
            data->word = strndup(token, strlen(token));
            data->counter = 1;

            if (order) { // alphabetical order
                p_list->insert_sorted(p_list, data);
            } else { // load order
                push_back(p_list, data);
            }
        }
        free(line);
        line = NULL;
    }
    free(line);
}

// test integer list
void list_test(List *p_list, int n) {
    char op[2];
    int v;
    for (int i = 0; i < n; ++i) {
        scanf("%s", op);
        switch (op[0]) {
            case 'f':
                scanf("%d", &v);
                push_front(p_list, create_data_int(v));
                break;
            case 'b':
                scanf("%d", &v);
                push_back(p_list, create_data_int(v));
                break;
            case 'd':
                pop_front(p_list);
                break;
            case 'r':
                reverse(p_list);
                break;
            case 'i':
                scanf("%d", &v);
                insert_in_order(p_list, create_data_int(v));
                break;
            default:
                printf("No such operation: %s\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n;
    List list;

    scanf("%d", &to_do);
    switch (to_do) {
        case 1: // test integer list
            scanf("%d", &n);
            init_list(&list, dump_int, free_int,
                      cmp_int, insert_elem);
            list_test(&list, n);
            dump_list(&list);
            free_list(&list);
            break;
        case 2: // read words from text, insert into list, and print
            init_list(&list, dump_word, free_word,
                      cmp_word_alphabet, insert_elem_counter);
            stream_to_list(&list, stdin, 0);
            dump_list(&list);
            free_list(&list);
            break;
        case 3: // read words, insert into list alphabetically, print words encountered n times
            scanf("%d", &n);
            init_list(&list, dump_word_lowercase, free_word,
                      cmp_word_alphabet, insert_elem_counter);
            stream_to_list(&list, stdin, 1);
            list.compare_data = cmp_word_counter;
            DataWord data = {NULL, n};
//			list.dump_data = dump_word_lowercase;
            dump_list_if(&list, &data);
            printf("\n");
            free_list(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

