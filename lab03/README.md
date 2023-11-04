# Meaning defintion of function:

## File stat.c

+ `rand_from_interval(...),`
The function which fills the array with integers from the interval [a, b] and then calculates the arithmetic mean and variance of the numbers stored in the array.

+ `bernoulli_gen(...)`
The function which randomly generates an array of n Bernoulli trials. The elements of this table are the test results.

+ `pmf(...)`
The function which simulates n rolls of two dice and saves the values ​​of the resulting approximate probability distribution in an array. When randomizing the number of points of one die - to obtain repeatable results - the rand() function should be called once and its result reduced to the interval [1, 6].

+ `print_histogram(double v[], int n, int x start, double y scale, char mark)`
The function which in sign mode presents a histogram of a function with n values ​​stored in the argument v.

+ `cdf(...)`
The function which, based on a given discrete probability distribution (pmf() function), calculates the values ​​of the distribution function at jump points (at the points of discontinuity of the distribution function, assuming its right-hand continuity).

+ `monty hall(...)`
The function which simulates n <a href="https://en.wikipedia.org/wiki/Monty_Hall_problem">games<a/>. Wherein: 
1. In each game, the function calls rand() exactly 2 times.
2. In the first draw, the number of the door behind which there is a prize is selected. 
3. In the second draw - the number of the door that the player chooses at the beginning of the <a href="https://en.wikipedia.org/wiki/Monty_Hall_problem">game<a/>.













