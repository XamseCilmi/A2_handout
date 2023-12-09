#include "numlib.h"
#include <stdio.h>

int main() {
    double sum = 0.0;
    size_t count = 0;

    while (1) {
        double x;
        int y = read_double_bin(stdin, &x);

        if (y == 0) {
            sum += x;
            count++;
        } else if (y == EOF) {
            if (count == 0) {
                fprintf(stderr, "No numbers in the input.\n");
                return 1;
            } else {
                // Calculate the average and print it
                double average = sum / count;
                write_double_ascii(stdout, average);
                return 0;
            }
        } else {
            fprintf(stderr, "Input error.\n");
            return 1;
        }
    }
}
