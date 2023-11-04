# Meaning defintion of function:

## File vec.c

+ `void linspace(double v[], double start, double stop, int n)`
The function fills the real array v with n values ​​evenly distributed in the interval [start, stop]. The value of n should be non-negative; for n = 1 the function returns a single-element array containing the start value. For n = 0, the function returns an empty array.

+ `void add(double v1[], const double v2[], int n)`
The function adds the i-th element of the real array v2 to the i-th element of the array v1. Both arrays have n-elements.

+ `double dot product(const double v1[], const double v2[], int n)`
The function calculates and returns the dot product of vectors v1 and v2 of length n.

+ `void multiply by scalar(double v[], int n, double s)`
The function multiplies each element of the real array v (of length n) by the real number s.

+ `void range(double v[], int n, double start, double step)`
The function fills the real array v with n values ​​from start (inclusive) including step. Step can have a negative value - then subsequent elements of the array will constitute a descending sequence.




