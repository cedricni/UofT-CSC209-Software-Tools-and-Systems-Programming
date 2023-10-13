#include <stdio.h>
#include <math.h>    // Need this for sqrt()
#include <stdlib.h>
#include <string.h>

#include "knn.h"

/* Print the image to standard output in the pgmformat.  
 * (Use diff -w to compare the printed output to the original image)
 */
void print_image(unsigned char *image) {
    printf("P2\n %d %d\n 255\n", WIDTH, HEIGHT);
    for (int i = 0; i < NUM_PIXELS; i++) {
        printf("%d ", image[i]);
        if ((i + 1) % WIDTH == 0) {
            printf("\n");
        }
    }
}

/* Return the label from an image filename.
 * The image filenames all have the same format:
 *    <image number>-<label>.pgm
 * The label in the file name is the digit that the image represents
 */
unsigned char get_label(char *filename) {
    // Find the dash in the filename
    char *dash_char = strstr(filename, "-");
    // Convert the number after the dash into a integer
    return (char) atoi(dash_char + 1);
}

/* ******************************************************************
 * Complete the remaining functions below
 * ******************************************************************/


/* Read a pgm image from filename, storing its pixels
 * in the array img.
 * It is recommended to use fscanf to read the values from the file. First, 
 * consume the header information.  You should declare two temporary variables
 * to hold the width and height fields. This allows you to use the format string
 * "P2 %d %d 255 "
 * 
 * To read in each pixel value, use the format string "%hhu " which will store
 * the number it reads in an an unsigned character.
 * (Note that the img array is a 1D array of length WIDTH*HEIGHT.)
 */
void load_image(char *filename, unsigned char *img) {
    // Open corresponding image file, read in header (which we will discard)
    FILE *f2 = fopen(filename, "r");
    if (f2 == NULL) {
        perror("fopen");
        exit(1);
    }
	//TODO
    int width;
    int height;
    fscanf(f2, "P2 %d %d 255 ", &width, &height);

    for (int i = 0; i < width * height; i++) {
        fscanf(f2, "%hhu ", &img[i]);
    }

    fclose(f2);
}


/**
 * Load a full dataset into a 2D array called dataset.
 *
 * The image files to load are given in filename where
 * each image file name is on a separate line.
 * 
 * For each image i:
 *  - read the pixels into row i (using load_image)
 *  - set the image label in labels[i] (using get_label)
 * 
 * Return number of images read.
 */
int load_dataset(char *filename, 
                 unsigned char dataset[MAX_SIZE][NUM_PIXELS],
                 unsigned char *labels) {
    // We expect the file to hold up to MAX_SIZE number of file names
    FILE *f1 = fopen(filename, "r");
    if (f1 == NULL) {
        perror("fopen");
        exit(1);
    }

    //TODO
    int size = 0;
    char temp[81];
    while (fscanf(f1, "%s", temp) != EOF) {
        if (size + 1 > MAX_SIZE) {
            break;
        }
        load_image(temp, dataset[size]);
        labels[size] = get_label(temp);
        size += 1;
    }
    
    fclose(f1);
    return size;
}

/** 
 * Return the euclidean distance between the image pixels in the image
 * a and b.  (See handout for the euclidean distance function)
 */
double distance(unsigned char *a, unsigned char *b) {

    // TODO
    int size = sizeof(a);
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(sum);
}

/**
* A selection sort helper.
**/
void sort(double arr[], int size) {
    int minimum_index = 0;
    for (int i = 0; i < size - 1; i++) {
        minimum_index = i;
        for (int j = i + 1; j < size; j++) {
            if (arr[minimum_index] > arr[j]) {
                minimum_index = j;
            }
        }
        swap(&arr[minimum_index], &arr[i]);
    }
}

void swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Return the most frequent label of the K most similar images to "input"
 * in the dataset
 * Parameters:
 *  - input - an array of NUM_PIXELS unsigned chars containing the image to test
 *  - K - an int that determines how many training images are in the 
 *        K-most-similar set
 *  - dataset - a 2D array containing the set of training images.
 *  - labels - the array of labels that correspond to the training dataset
 *  - training_size - the number of images that are actually stored in dataset
 * 
 * Steps
 *   (1) Find the K images in dataset that have the smallest distance to input
 *         When evaluating an image to decide whether it belongs in the set of 
 *         K closest images, it will only replace an image in the set if its
 *         distance to the test image is strictly less than all of the images in 
 *         the current K closest images.
 *   (2) Count the frequencies of the labels in the K images
 *   (3) Return the most frequent label of these K images
 *         In the case of a tie, return the smallest value digit.
 */ 
int knn_predict(unsigned char *input, int K,
                unsigned char dataset[MAX_SIZE][NUM_PIXELS],
                unsigned char *labels,
                int training_size) {
    
    double distances[training_size];
    double temp[training_size];
    for (int i = 0; i < training_size; i++) {
        distances[i] = distance(input, dataset[i]);
        temp[i] = distance(input, dataset[i]);
    }

    sort(temp, training_size);

    int klabels[K];
    for (int j = 0; j < K; j++) {
        double valid = temp[j];
        for (int k = 0; k < training_size; k++) {
            if (distances[k] == valid) {
                klabels[j] = labels[k];
                break;
            }
        }
    }

    int f[K];
    int cnt;
    for (int m = 0; m < K; m++) {
        f[m] = -2;
    } 

    for (int n = 0; n < K; n++) {
        cnt = 1;
        for (int o = n + 1; o < K; o++) {
            if (klabels[n] == klabels[o]) {
                cnt = cnt + 1;
                f[n] = -1;
            }
        }
        if (f[n] != -1) {
            f[n] = cnt;
        }
    }

    int final[K];
    int largest = -1;
    for (int p = 0; p < K; p++) {
        if (f[p] != -1 && f[p] > largest) {
            largest = f[p];
        }
    }
    int same = 0;
    for (int q = 0; q < K; q++) {
        if (f[q] == largest) {
            final[same] = klabels[q];
            same += 1;
        }
    }
    int smallest = final[0];
    for (int r = 0; r < K; r++) {
        if (final[r] < smallest) {
            smallest = final[r];
        }
    }
    return smallest;
}
