# Meaning defintion of function:

## File perm.c

+ `rand_from_interval(...),`
The function uses the library function rand() and the modulo division operation to return a number from the closed interval [a, b]

+ `rand_permutation(...)`
The function that is to randomly select one of the permutations of n elements of the set of natural numbers. The elements of this set - natural numbers from the interval [0, n − 1] - are to be entered into the table in ascending order

+ `bubble_sort(...)`
Just a bubble_sort :)

## File stck.c

+ `stack_push(double n)`
The function puts the parameter value on the stack and returns the OK constant, and in case of overflow it does not change the contents of the stack and returns the OVERFLOW constant.

+ `stack_pop(void)`
The function removes one element from the stack and returns its value. If the stack is empty, it returns the UNDERFLOW constant.

+ `stack_state(void)`
The function returns the number of items on the stack.

+ `queue_push(int in nr)`
The function increases the queue by other number of customers. The current customer number is stored in the global variable curr no. Returns the value of the OK constant if all clients have been successfully added to the queue. If the number of people entering the queue is greater than the number of free places in the queue, places in the queue are occupied until the seats are filled, and the remaining "would-be customers" give up (keeping their numbers). In such a situation, the function returns the OVERFLOW constant.

+ `queue_pop(int out nr)`
The function simulates out service for the longest waiting customers. The function returns the length of the remaining queue. If out nr is greater than the queue length, the function returns the UNDERFLOW constant.

+ `queue_state()`
The function returns the number of customers waiting in line.

+ `queue print()`
The function lists the numbers of waiting customers (in the order they entered
queues).

+ `cbuff_push(int cli_nr)`
The function increases the queue by one customer with number cli nr and returns the OK constant. Returns OVERFLOW if the queue is full.

+ `cbuff_pop()`
The function simulates the service of the longest waiting customer. The function returns the number of the customer leaving the queue, and if the queue was empty, it returns the UNDERFLOW constant.

+ `cbuff_state()`
The function returns the number of waiting customers

+ `cbuff print()`
The fuction prints the numbers of waiting customers (according to the order in the queue).

## File var.c
This file contains simulations of a card game called war.







