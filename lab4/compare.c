#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Write the main() function of a program that takes exactly two arguments,
  and prints one of the following to standard output:
    - "Same\n" if the arguments are the same.
    - "Different\n" if the arguments are different.
    - "Invalid\n" if the program is called with an incorrect number of
      arguments.
	  NOTE: Cut and paste these strings into your printf statements to 
	  ensure that they will be exactly the strings above.

  Your main function should return 0, regardless of what is printed.
*/

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("%s", "Invalid\n");
		return 0;
	}
	char *a = argv[1];
	char *b = argv[2];
	int check = 1;
	if (strlen(a) != strlen(b)) {
		check = 0;
	}
	else {
		for (int i = 0; i < strlen(a); i++) {
			if (a[i] != b[i]) {
			check = 0;
			}
		}
	}
	if (check == 1) {
		printf("%s", "Same\n");
	}
	else {
		printf("%s", "Different\n");
	}
	return 0;
}