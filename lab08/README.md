# Meaning defintion of function:

## File sort.c

+ `void *bsearch2(const void *key, const void *base, size t nitems, size t size, int (*compar)(const void*, const void*), char *result)`There are two possible search results:
1. Search success – the function writes a value other than zero to the result address and returns it
address of the found item (like the original bsearch function).
2. The element with the key *key was not found - the function writes zero to the result address and returns the address where a new element should be entered, maintaining the assumed order. The function does not check whether the returned address does not exceed the memory range allocated to the base array.

+ `Food *add record(Food *tab, int *np, ComparFp compar, Food *new)`
The function which calls the bsearch2() function that checks whether the new article (its data is saved with the pos address *new) is saved in the tab array with *np elements. The function pointed to by the pointer to the compar function (of the ComparFP type - defined in the template) decides whether *new should be considered new.
• If *new is not a new element, the data stored in the array element is modified by the data stored in *new - specifically, the quantity of the article found in the array is increased by the quantity stored in *new. The function returns the address of the modified array element.
• If *new is a new element, the add record function adds a new element in the indicated place (possibly shifting some of the array elements), increases the number of array elements *eg and returns the address of the entered element.

+ `int read goods(Food *tab, int no, FILE *stream, int sorted)`
The function which reads no lines of data from the input stream. In this case, the sorted parameter should have the value 1, which means that for each record it calls the add record function maintaining the sort order.













