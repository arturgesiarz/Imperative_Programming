# Meaning defintion of function:

## File alloc.c

+ `prod_mat()`
The function which calculates the product of the matrix using two helper functions get() and set(), which calculate the address of each element of the matrix based on the row and column numbers and the number of columns.

+ `read_int_lines_cont()`
The function which reads lines (records) containing sequences of numbers (strings of different numbers) and saves them in numerical (not character) form to a contiguous area of ​​memory. The addresses of the starting element of each line are saved in the pointer array sent with the first parameter. The function returns the number of lines loaded.

+ `write_int_line cont()`
The function which prints the selected row of the array.

+ `read_char_lines()`
The function which loads lines (records) containing strings of characters. Each string is padded with a string termination character and written to the allocated memory. The addresses of the starting element of each line are saved in the pointer array sent with the first parameter. The function returns the number of lines loaded

+ `write_char_line()`
The function which prints the selected row of the array.

+ `read_int_lines()`
The function which loads lines (records) containing sequences of numbers (strings of different numbers), each sequence is saved in numerical (not character) form to the memory area allocated by the malloc() function. The addresses of the allocated memory, the number of row elements and the arithmetic mean of its elements are saved in the structure table sent with the first parameter. The function returns the number of lines loaded.

+ `sort_by_average()`
The function which, using the qsort() function, sorts the array rows by increasing average of the elements.

+ `write_int_line()`
The function which prints the selected row of the sorted array.

+ `read_sparse()`
The function which reads lines containing triplets of integers. The first two numbers are the row and column numbers of the matrix element, respectively, and the third is its value. The function returns the number of loaded triples.

+ `make_CSR()`
The function which, based on the loaded triples, generates three vectors corresponding to the CSR format.

+ `multiply_by_vector()`
The function which multiplies a sparse matrix in CSR format by a given vector x.

+ `write_vector()`
The function which prints a vector of integers.


