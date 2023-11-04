# Meaning defintion of function:

## File vect.c

+ `init_vector()`
The function which allocated the data array to the given initial capacity and initializes the remaining fields.

+ `reserve()`
The function which reallocated the data array so that it has at least the given capacity. If the set capacity does not exceed the current one, the function does nothing.

+ `resize()`
The function which changed the current number of elements of the vector: if the new size is smaller than the current one, the excess elements are removed; if the new size is larger, the vector is completed with the appropriate number of zeroed elements.

+ `push_back()`
The function which added an element to the end of the vector.

+ `insert()`
The function which added an element at a given position.

+ `clear()`
The function which removed all elements from vector.

+ `erase()`
The function which removed vector element at given position.

+ `erase_value()`
The function which removed all vector elements with a given value.

+ `erase_if()`
The function which removed all elements of the vector that satisfy the predicate.

+ `shrink_to_fit()`
The function which adjusted the size of the array to the current number of vector elements.

+ `print_vector()`
The function which written down the capacity of the vector and its elements

+ `read_int()`
The function readed the integer value to the address pointed to by value

+ `print_int()`
The function printed the integer value

+ `int_cmp()`
The function are integer value comparator (descending sort)

+ `is_even()`
The function return predicate (returns 1 if the number is even).

+ `read_char()`
The function read a value of type char to the address pointed to by value.

+ `print_char()`
The function printed value of type char.

+ `char_cmp()`
Character value comparator (lexicographic order).

+ `is_vowel()`
Predicate (returns 1 if the character is a vowel).

+ `read_person()`
The function read the elements of the Person structure to the address pointed to by value.

+ `print_person()`
The function output the Person structure

+ `person_cmp()`
Person structure comparator (descending by age, then name and surname - ascending).

+ `is_older_than_25()`
Predicate (returns 1 if the person is over 25 years old).




# Meaning commend in the file:

## File vect.c

+ `p: push_back(value)`
+ `i: insert(index, value)`
+ `e: erase(index)`
+ `v: erase_value(value)`
+ `d: erase_if()`
+ `r: resize(new_size)`
+ `c: clear()`
+ `f: shrink_to_fit()`
+ `s: qsort()`








