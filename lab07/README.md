# Meaning defintion of function:

## File int.c

+ `f_poly(double x)`
The function which returns the value of the polynomial 2x^5 −4x^4 +3.5x^2 +1.35x−6.25

+ `f_rat(double x)`
The function which returns the value of the function f(x)=1/( (x-0.5)^2 + 0.001 )

+ `f_exp(double x)`
The function which returns the value of the function f(x)=2*x*e^(-1.5x)-1

+ `f_trig(double x)`
The function which returns the value of the function f(x)=xtg(x)

+ `quad_rect_left(...)`
The functions calculating the complex quadrature for the function f with interval division
integrating [a, b] into n subintervals, method: rectangles forward.

+ `quad_rect_right(...)`
The functions calculating the complex quadrature for the function f with interval division
integrating [a, b] into n subintervals, method: rectangles backwards.

+ `quad_rect_mid(...)`
The functions calculating the complex quadrature for the function f with interval division
integrating [a, b] into n subintervals, method: center point rectangles.

+ `quad_trap(...)`
The functions calculating the complex quadrature for the function f with interval division
integrating [a, b] into n subintervals, method: trapeze.

+ `quad_simpson(...)`
The functions calculating the complex quadrature for the function f with interval division
integrating [a, b] into n subintervals, method: Simpson.

+ `double quad select(int quad no, int fun no, double a, double b, int n)`
The function which in the interval [a, b] calculates the quadrature indicated by the quad no index (divided into n subintervals) for the integrand function indicated by the fun no index. Outside the quad select() function, arrays of pointers to functions of the type "Func1vFp" and "QuadratureFp" are defined, respectively.

+ `double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny)`
The function which uses the complex forward rectangular method (leftpoint) to calculate the approximate value of the integral. The parameters nx and ny are the numbers of subintervals of complex quadratures.

+ `double trpl_quad_rect(FuncNvFp f,int variable no, const double variable lim[][2], const int tn[], BoundNvFp boundary)`
The function which computes the quadrature of rectangles backwards (rightpoint) as an approximation of the triple integral over the cuboid. Lower and upper limits of integration intervals along subsequent variables
are passed in the variable lim array, and the numbers of subintervals are passed in the tn array. The boundary parameter is the address of the predicate. A NULL value for this parameter means there is no predicate, i.e no limitations in the integration area.

+ `recur_quad_rect mid(double *psum, FuncNvFp f, int variable no, double tvariable[], const double variable lim[][2], const int tn[], int level, BoundNvFp boundary)`
The function which computes an approximation of the integral of a variable no-dimensional function f over a hypercuboid given the constraints written in variable lim. A complex quadrature of point rectangles is calculated along the i-th variable.
here the middle one, divided into tn[i] sub-intervals. The quadrature value is stored at the address passed to the function by its first parameter.










