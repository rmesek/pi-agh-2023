#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_CHAR 5
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int find_idents(){
	int c, cn;
	int state = 0;
	int id_no = 0, pos = 0;
	int indices[MAX_IDS];
	
	for (int i = 0; i < MAX_IDS; ++i) {
		indices[i] = i;
	}
	
	while ((c = getc(stdin)) != EOF) {
		switch (state) {
			case IN_LINE_COMMENT:
				if (c == '\n') {
					state = 0;
				}
				break;
				
			case IN_BLOCK_COMMENT:
				if (c == '*') {
					cn = getc(stdin);
					if (cn == '/') {
						state = 0;
					} else ungetc(cn, stdin);
				}
				break;
				
			case IN_STRING:
				if (c == '"') state = 0;
				if (c == '\\') getc(stdin);
				break;
				
			case IN_CHAR:
				if (c == '\'') state = 0;
				if (c == '\\') getc(stdin);
				break;
				
			case IN_ID:
				if (isalnum(c) || c == '_') {
					tab[id_no][pos++] = (char)c;
				} else {  // end of id
					ungetc(c, stdin);
					state = 0;
					tab[id_no][pos] = '\0';
					pos = 0;
					++id_no;
				}
				break;
				
			default:
				if (c == '/') {
					cn = getc(stdin);
					if (cn == '/') {
						state = IN_LINE_COMMENT;
						break;
					}  // end if LINE COMMENT
					if (cn == '*') {
						state = IN_BLOCK_COMMENT;
						break;
					}  // end if BLOCK COMMENT
					ungetc(cn, stdin);
					break;
				}  // end if COMMENT
				
				if (c == '"') {
					state = IN_STRING;
					break;
				}  // end if STRING
				
				if (c == '\'') {
					state = IN_CHAR;
					break;
				}
				
				if (isalpha(c) || c == '_') { 
					state = IN_ID;
					tab[id_no][pos++] = (char)c;
					break;
				}  // end if ID
				
				break;
		}  // end switch
	}  // end while
	
	qsort(indices, (size_t)id_no, sizeof(int), index_cmp);
	int unique = 0;

	size_t n_keywords = sizeof(keywords) / sizeof(char*);
	for (int i = 0; i < id_no; ++i) {
		if (i > 0 && strcmp(tab[indices[i - 1]], tab[indices[i]]) == 0) {
			continue;  // id already found
		}
		char *key = tab[indices[i]];
		//printf("%s\n", key);
		if (bsearch(&key, keywords, n_keywords, sizeof(char*), cmp) == NULL) {
			//printf("---- %s\n", key);
			++unique;  // not a keyword
		}
	}
	return unique;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(tab[a], tab[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

