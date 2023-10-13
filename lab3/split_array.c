#include <stdio.h>
#include <stdlib.h>


int *build_array(char **strs, int size);

/* Return a pointer to an array of two dynamically allocated arrays of ints.
   The first array contains the elements of the input array s that are
   at even indices.  The second array contains the elements of the input
   array s that are at odd indices.

   Do not allocate any more memory than necessary.
*/
int **split_array(const int *s, int length) {
  length = length - 1;
  int **splited = malloc(2 * sizeof(int *));
  int even = length - length / 2;
  int odd = length / 2;
  splited[0] = malloc(even * sizeof(int));
  splited[1] = malloc(odd * sizeof(int));
  int even_c = 0;
  int odd_c = 0;
  for (int i = 0; i < length; i++) {
    if (i % 2 == 0) {
      splited[0][even_c] = s[i];
      even_c += 1;
    }
    else {
      splited[1][odd_c] = s[i];
      odd_c += 1;
    }
  }
  return splited;
}

/* Return a pointer to an array of ints with size elements.
   - strs is an array of strings where each element is the string
     representation of an integer.
   - size is the size of the array
 */

int *build_array(char **strs, int size) {
  int *arr = malloc((size - 1) * sizeof(int));
  for (int i = 0; i < size - 1; i++) {
    arr[i] = strtol(strs[i+1], NULL, 10);
  }
  return arr;
}


int main(int argc, char **argv) {
    /* Replace the comments in the next two lines with the appropriate
       arguments.  Do not add any additional lines of code to the main
       function or make other changes.
     */
    int *full_array = build_array(argv, argc);
    int **result = split_array(full_array, argc);

    printf("Original array:\n");
    for (int i = 0; i < argc - 1; i++) {
        printf("%d ", full_array[i]);
    }
    printf("\n");

    printf("result[0]:\n");
    for (int i = 0; i < argc / 2; i++) {
        printf("%d ", result[0][i]);
    }
    printf("\n");

    printf("result[1]:\n");
    for (int i = 0; i < (argc - 1) / 2; i++) {
        printf("%d ", result[1][i]);
    }
    printf("\n");
    free(full_array);
    free(result[0]);
    free(result[1]);
    free(result);
    return 0;
}
