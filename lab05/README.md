# Meaning defintion of function:

## File mat.c

+ `matrix_product()`
The function which calculates the product of matrices A and B and stores it in matrix AB.

+ `gauss_simplified()`
The function which transforms a square matrix A with dimensions n × n into an upper triangular form using the Gaussian method and returns the value of the determinant. In case the element on the main diagonal is equal to zero, the triangularization is not completed and the determinant = NAN.

+ `gauss(double A[][SIZE], const double b[], double x[], int n, double eps)`
The function which transforms the square matrix A into an upper triangular form using the Gaussian method and returns the value of the determinant. The rows of the matrix are swapped so that the absolute value of the main element is the largest. Swapping rows is not done by rewriting the rows in the array, but by using a permutation vector of row indices. If, after replacing the rows, the element on the main diagonal is smaller than eps, the triangularization is not completed and the determinant takes the value 0.
If the arguments of the function b and x and the determinant are not zero, the function solves the system of equations and stores the solution in the x array.

+ `matrix inv(double A[][SIZE], double B[][SIZE], size t n, double eps)`
The function which determines matrix B - the inverse of the non-singular matrix A. The Gauss-Jordan method should be used with the expansion of matrix A by the identity matrix. The rows of the extended matrix are replaced in the same way as in task 3. The function returns the determinant of matrix A. If, after replacing the rows, the element on the main diagonal is smaller than eps, the inversion algorithm is not completed and the determinant takes the value 0 (the system of equations is not solved).









