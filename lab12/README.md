# Meaning defintion of function:

## File dll.c

+ `push_back()`
Function  build a bidirectional list of arrays by inserting subsequent nodes containing arrays at the end of the list.

+ `skip_forward()`
+ `get_forward()`
Functions allowing by iterating forward through the list of arrays, to write the contents of selected cells to standard output.

+ `skip_backward()`
+ `get_backward()`
Functions allowing by iterating backwards through the list of arrays, to write the contents of selected cells to standard output.

+ `remove_at()`
Function deletes the specified cells of a bidirectional array list. If the cell being deleted is the only array cell in a node (single-element array), the entire node is deleted.

+ `insert_in_order()`
Function reads consecutive integers and then:
1. If there is already a node in the list containing numbers with length equal to d(n), the function adds the number n to this array, maintaining its ascending order.
2. Otherwise, the function adds a new node to the list (so that the list is ordered ascending according to the length of the numbers in subsequent nodes).









