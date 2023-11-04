# Meaning defintion of function:

## File char.c

+ `wc()`
The function which reads text from the standard input and on this basis counts the lines, words and characters appearing in this text, similarly to the Unix wc command. A word is a string of characters separated by a space, tab, or newline.

+ `char_count()`
The function which reads text from the standard input and, on this basis, counts the number of characters appearing in this text.

+ `digram_count()`
The function which reads text from the standard input and, on this basis, counts the multiplicities of character digrams (character pairs) appearing in this text.

+ `find_comments()`

The function which reads a string of characters from the standard input constituting a C program. The function counts block comments (/* ... */) and single-line comments (// ...) in the read text and returns the obtained numbers to the main() function at using parameters.


## File ids.c

+ `find_idents()`
The function which reads a string of characters from the standard input constituting a C program. The function counts the unique identifiers contained in the read text and returns the obtained number to the main() function.
The definition of an identifier is the same as in the C language, i.e. it is a sequence of letters and numbers starting with a letter; the underscore character is considered a letter, i.e., for example, the identifier _a is legal.








