# Meaning defintion of function:

## File ht.c (hash_table)

+ `init_ht()`
Function allocates the array to a given initial length and initializes the remaining fields.

+ `dump_list()`
Function prints a list of elements associated with a given hash function value.

+ `free_element()`
Function frees data and array element memory.

+ `free_table()`
Function frees memory of all elements of an array (as well as the array itself).

+ `insert_element()`
Function add an item to the array.

+ `rehash()`
Function double the size of the array and arrange the elements according to the new hash function values. Note: the function retrieves subsequent elements from the "old" array and moves them to the "new" one, adding them to the beginning of the proper list.

+ `remove_element()`
Function removes element with given key.

+ `get_element()`
Function return the address of the element with the given key.

+ `hash_int()`
Function calculated the value of the hash function for an int parameter

+ `hash_char()`
Function calculated the value of the hash function for the parameter type char

+ `hash_word()`
Function calculated the value of the hash function for a DataWord type parameter.

+ `stream_to_ht()`
Function read lines of text, extract words (defined as in the one-way list problem) and add to the array after converting to lowercase; for repeated keys (words), increase the word counter (modify data() function.

+ `create_data_word()`
Function created and return a data union with the given parameter word
function and the counter equal to 1


# Meaning commend in the file:

## File ht.c (hash_table)

+ `i: insert_element(value)`
+ `r: remove_element(value)`










