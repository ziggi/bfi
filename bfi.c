/*
 *
 *     Copyright (C) 2013 ziggi
 *     Using GNU GPL v3 license
 *
 */

#include <stdio.h>
#include <string.h>


#define MAX_CELLS 30000 // brainfuck memory size
#define MAX_VALUE 256 // max memory value

// error defines
enum { ERROR_OPEN, ERROR_ARGS, ERROR_MCL, ERROR_MOL };

// validate source
int validate(const char *src);

// debug function
void show_error(const int error);

// brainfuck function
void brainfuck(const char *src);


int main(int argc, char *argv[])
{
	if (argv[1]) {
		// reading file
		FILE *src_file = fopen(argv[1], "rt");
		
		if (!src_file) {
			show_error(ERROR_OPEN);
			return 0;
		}
		
		char src[MAX_CELLS] = {0};
		int c, i = 0;
		
		while ((c = fgetc(src_file)) != EOF) {
			src[i++] = c;
		}
		
		fclose(src_file);
		
		// validate code
		int error = validate(src);
		
		// if no error run else show error
		if (error == 0) {
			brainfuck(src);
		} else {
			show_error(error);
		}
	} else {
		show_error(ERROR_ARGS);
	}
	return 0;
}

// validate source
int validate(const char *src)
{
	int len = strlen(src),
		l = 0,
		i = 0;
	
	for (i = 0; i < len; i++) {
		if (src[i] == '[') {
			l++;
		} else if (src[i] == ']') {
			l--;
			if (l < 0) {
				return ERROR_MOL;
			}
		}
	}
	
	if (l > 0) {
		return ERROR_MCL;
	}
	return 0;
}

// debug function
void show_error(const int error)
{
	switch (error) {
		case ERROR_OPEN:
			printf("Error: Could not open file.\n");
			break;
		case ERROR_ARGS:
			printf("Usage: ./bfi <filename>\n");
			break;
		case ERROR_MCL:
			printf("Error: Closing bracket(s) missing.\n");
			break;
		case ERROR_MOL:
			printf("Error: Opening bracket(s) missing.\n");
			break;
		default:
			printf("Error: %d\n", error);
			break;
	}
}

void brainfuck(const char *src)
{
	char mem[MAX_CELLS] = {0};
	int pointer = 0,
		l = 0,
		i = 0,
		len = strlen(src);
	
	for (i = 0; i < len; i++) {
		switch (src[i]) {
			case '>':
				if (pointer < MAX_CELLS) {
					pointer++;
				}
				break;
			
			case '<':
				if (pointer > 0) {
					pointer--;
				}
				break;
			
			case '+':
				if (mem[pointer] < MAX_VALUE) {
					mem[pointer]++;
				}
				break;
			
			case '-':
				if (mem[pointer] > 0) {
					mem[pointer]--;
				}
				break;
			
			case '.':
				putchar(mem[pointer]);
				break;
			
			case ',':
				do {
					mem[pointer] = getchar();
				} while (mem[pointer] == '\n');
				break;
			
			case '[':
				if (mem[pointer] == 0) {
					while (src[i] != ']' || l > 0) {
						i++;
						if (src[i] == '[') {
							l++;
						} else if (src[i] == ']') {
							l--;
						}
					}
				}
				break;
			
			case ']':
				if (mem[pointer] != 0) {
					while (src[i] != '[' || l > 0) {
						i--;
						if (src[i] == '[') {
							l--;
						} else if (src[i] == ']') {
							l++;
						}
					}
				}
				break;
		}
	}
}
