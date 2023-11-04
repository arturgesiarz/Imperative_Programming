#include <stdio.h>
#include <math.h>

#define RECURS_LEVEL_MAX  10
#define N_MAX             10

// pointer to function of one variable
typedef double (*Func1vFp)(double);

// example functions of one variable
double f_poly(double x) { // polynomial a[0] + a[1]x + ... + a[n]x^n
    double result;
    result=2*pow(x,5)-4*pow(x,4)+3.5*pow(x,2)+1.35*x-6.25;
    return result;
}

double f_rat(double x) {
    double result;
    double denominator=1;
    denominator=pow(x-0.5,2)+0.01;
    result=1/denominator;
    return result;
}

double f_exp(double x) {
    double result;
    result=2*x*pow(M_E,-1.5*x)-1;
    return result;
}

double f_trig(double x) {
    double result;
    result=x*tan(x);
    return result;
}

// Quadratures

// rectangle rule, leftpoint
double quad_rect_left(Func1vFp f1, double a, double b, int n) { //c=a
    double result=0;
    double last_var=a;
    const double h=(b-a)/n;
    for(int i=0;i<n;i++){
        result+=f1(last_var);
        last_var+=h;
    }
    result*=h;
    return result;
}

// rectangle rule, rightpoint
double quad_rect_right(Func1vFp f1, double a, double b, int n) {
    double result=0;
    const double h=(b-a)/n;
    double last_var=a+h;
    for(int i=0;i<n;i++){
        result+=h*f1(last_var);
        last_var+=h;
    }

    return result;
}

// rectangle rule, midpoint
double quad_rect_mid(Func1vFp f1, double a, double b, int n) {
    double result=0;
    const double h=(b-a)/n;
    double last_var=(a+(a+h))/2;
    for(int i=0;i<n;i++){
        result+=h*f1(last_var);
        last_var+=h;
    }
    return result;
}

// trapezoidal rule
double quad_trap(Func1vFp func, double a, double b, int n) {
    double result=0;
    const double h=(b-a)/n;
    double last=a;
    double next=a+h;
    for(int i=0;i<n;i++){
        result+=((next-last)/2*(func(last)+func(next)));
        last=next;
        next+=h;
    }
    return result;
}

// Simpson's rule
double quad_simpson(Func1vFp f, double a, double b, int n) {
    double result=0;
    const double h=(b-a)/n;
    double last=a;
    double next=a+h;
    double middle=(last+next)/2;
    for(int i=0;i<n;i++){
        result+=((next-last)/6*(f(last)+4*f(middle)+f(next))); //za kazdym razem (naxt-last) bedzie sie skracac do po prostu przedialu h
        last=next;
        next+=h;
        middle=(last+next)/2;
    }
    return result;
}

// pointer to quadrature function
typedef double (*QuadratureFp)(Func1vFp,double,double,int);

// array of pointers to integrand functions
Func1vFp func_tab[] = { f_poly, f_rat, f_trig, f_exp };

// array of pointers to quadrature functions
QuadratureFp quad_tab[] = {
	quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson };

// calls 'quad_no' quadrature function for 'fun_no' integrand function
// on interval [a, b] and n subintervals
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
    double solution;
    solution=quad_tab[quad_no](func_tab[fun_no],a,b,n);
    return solution;
}

// adaptive algorithm

double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) { //delta jest niezmienna dla wszystkich przedzialow, jedynie co z nia robimy to pomniejszamy ja dzielac przez dwa
    double c=(a+b)/2;
    double S_1=quad(f,a,c,1); //wyznaczenie pierwszego przedzialu dla konkretnej funkcji
    double S_2=quad(f,c,b,1); //wyznaczanie drugiego przedzialu
    if( ( (S_1+S_2)-S<0 && (S_1+S_2)-S>=-delta) || ( (S_1+S_2)-S>=0 && (S_1+S_2)-S<=delta) ){ //robie ogranicznia dole oraz gorne, aby zwiualizowac dzialanie abs
        return (S_1+S_2);
    }
    if(level>RECURS_LEVEL_MAX) return NAN; //z polecenia wynika to, ze mamy to sprawdzic dopiero po tym jak sprawdzimy glo
    return recurs(f,a,c,S_1,delta/2,quad,level+1) + recurs(f,c,b,S_2,delta/2,quad,level+1);
    //stosuje tu rekurencje dodajac kolejne przedzialy, i przekazujac na wyzszy poziom rekurencji
}

// initialization for adaptive algorithm
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
    double S=quad(f,a,b,1);
    return recurs(f,a,b,S,delta,quad,0);
}

// double integrals

// pointer to function of two variables
typedef double (*Func2vFp)(double,double);

double func2v_2(double x, double y) {
	return 2 - x*x - y*y*y;
}

// sample functions to define the normal domain

double lower_bound2(double x) {
	return 0.7*exp(-2*x*x);
}
double upper_bound2(double x) {
	return sin(10*x);
}

// rectangle rule (leftpoint) - double integral over rectangular domain
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {
    double h_x=(x2-x1)/nx; //dlugosci przedzialow w jakich bedziemy badac nasze konkretne calki
    double h_y=(y2-y1)/ny;
    double temp;
    double result=0;
    for(int i=0;i<nx;i++){ //tworzymy petle w petli aby dla kazdego x, przyporzadkowywac konkretne y
        temp=0;
        for(int j=0;j<ny;j++){
            temp+=f(x1+i*h_x,y1+j*h_y)*h_y*h_x; //przesuwam kolejne elementy aby obliczyc calke dwukrotna
        }
        result+=temp;
    }
    return result;
}

// rectangle rule (midpoint) - double integral over normal domain with respect to the x-axis
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy,
						   Func1vFp fg, Func1vFp fh) {
    double y1,y2;
    double hx=(x2-x1)/nx; //jest to wartosc stala a wiec mozemy ja wczesniej, obliczyc inne rzeczy jak wlasnie y1 czy y2 musimy liczyc na biezaco
    int ny; //bedzie sie to zmienialo wraz z x
    double result=0;
    for(int i=0;i<nx;i++){
        y2=fh((x1+(x1+hx))/2+i*hx); //musze w liczeniu y2, uwzgldnic to ze x, zmienia mi sie wraz z y bo przesuwam go o staly przedzial h
        y1=fg((x1+(x1+hx))/2+i*hx);
        ny=(int)ceil((y2-y1)/hy); //wyznaczam zmienna ilosc przedzialow w zaleznosci od wartosci y2 oraz y1, dla hy, pierwszego podanego
        hy=(y2-y1)/ny; //latwo zauzyc, ze dlugosc mojego przedzialu mogla ulec zmianie poprzez, zmiane ilosci, elementow wniej oraz, zmienne y1 oraz y2, w zaleznosci od okresowsci tak naprawde h
        for(int j=0;j<ny;j++){
            result+=f((x1+(x1+hx))/2+i*hx,(y1+(y1+hy))/2+j*hy)*hx*hy;
        }
    }
    return result;
}

// domains with respect to the x-axis
double min(double x,double y){ //funkcja zwraca mniejsza liczbe
    if (x<=y) return x;
    return y;
}

double max(double x,double y){ //funkcja zwraca wieksza liczbe
    if (x<=y) return y;
    return x;
}
// rectangle rule (leftpoint) - double integral over multiple normal
double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2,
		int ny, Func1vFp fg, Func1vFp fh)  {
    const double h_x=(x2-x1)/nx; //okreslam dlugosc przedzialu
    const double h_y2=(y2-y1)/ny; //bede na biezaco zmienial
    double h_y;
    double new_y1,new_y2; //musze w nowej zmiennej przechowywac, wartosci dla y1 oraz y2, bo bede na biezaco te wartosci zmienial i dodawal do nich stala wartosc h, a jak skoczy mi sie petele to juz strace ta wartosc
    double temp_h,temp_g;
    double solution=0;
    for(int i=0;i<nx;i++){
        temp_h=fh(x1+i*h_x); //przesuwam sie sztucnie po moim przedzaile w celu znalezeiania odpowiedniedniego g() oraz h() aby sprawdzic warunek, zgodnie z regula leftpoint
        temp_g=fg(x1+i*h_x);
        if(temp_g<temp_h){
            new_y2=min(y2,temp_h);
            new_y1=max(y1,temp_g);
            ny=ceil((new_y2-new_y1)/h_y2);//okreskam dlugosc przedzialu na tle stalem wawrtosic h_y2
            h_y=(new_y2-new_y1)/ny; //okreslam jakby chwilowa dlugosc przedzialu w zaleznosci od y1 i y2, aby uzyskac dokladniejszy wynik
            for(int j=0;j<ny;j++){
                solution+=f(x1+i*h_x,new_y1+j*h_y)*h_y*h_x;
            }
        }
    }
    return solution;
}

// multiple quadratures

typedef double (*FuncNvFp)(const double*, int);
typedef int (*BoundNvFp)(const double*, int); //wskaznik do okreslenia czy zadany punkt w przestrzeni n-wymiarowej nalezy do zadanego obszaru calkowania

// sample function of three variables
double func3v(const double v[], int n) {
	return v[0] - v[1] + 2*v[2];
}

// sample predicate in 3D
int bound3v(const double v[], int n) { // a cylinder
	return v[0] > 0 && v[0] < 0.5 && v[1]*v[1] + (v[2]-1)*(v[2]-1) < 1;
}

// sample function of n variables
double funcNv(const double v[], int n) {
	double fv = 1.;
	for (int i = 1; i < n; ++i) {
		fv += sin(i*v[i]);
	}
	return fv;
}
// sample n-dimensional predicate (n-dim hypersphere)
int boundNv(const double v[], int n) {
	double r = 0.0;
	for (int i = 0; i < n; ++i) r += (v[i]-1)*(v[i]-1);
	return r <= 1.;
}

// rectangular rule (rightpoint)
double trpl_quad_rect(FuncNvFp f, double variable_lim[][2], const int tn[], BoundNvFp boundary) { // multiple integrals over a cuboid with predicate (if boundary != NULL)
    double solution=0;
    double height_orginal[3]; //tablica przechowujaca dane o wysokosci mojego przedzialu
    double next[3]={variable_lim[0][0],variable_lim[1][0],variable_lim[2][0]}; //tablica sluzaca do podania wartosci w funkcji do oblicznia
    for(int i=0;i<3;i++){ //wyliczenia dlugosci oringalej, dlugosci przedzialu, potem wiadomo ze w trakcie wykonownia programou ta, wlasnie wysokosc bedzie sie zmieniac
        height_orginal[i]=(variable_lim[i][1]-variable_lim[i][0])/tn[i]; //wyznaczamy tak dlugosc naszego przedzialu kazego
    }
    for(int i=0;i<tn[0];i++){
        //x
        next[0]+=height_orginal[0];//musze dodawac a nie mnozyc razy i,j, lub k, ponieaz trace dokladnosc
        for(int j=0;j<tn[1];j++){
            //y
            next[1]+=height_orginal[1];
            for(int k=0;k<tn[2];k++){
                //z
                next[2]+=height_orginal[2];
                if(boundary==NULL || boundary(next,3)){ //sprawdzamy czy nie isteniaja ograczenia lub czy ograniczenie sa spelnione aporo naszej tablicy next
                    solution+=f(next,3)*height_orginal[0]*height_orginal[1]*height_orginal[2];
                }
            }
            next[2]=variable_lim[2][0];
        }
        next[1]=variable_lim[1][0];
    }
    return solution;

}


void recur_quad_rect_mid_help_funtion(double *p_sum, FuncNvFp f, int variable_no, double t_variable[], //tablica t_variabile bedzie mi sluzyla do przekazywania kolejnmych arugmentow na kolejeny poziom
         double variable_lim[][2], const int tn[], int level, BoundNvFp boundary,double height_tab[]) {
    if(level==variable_no){ //konczymy wykonywanie programu, ponieaz juz policzylismy nasza dana calke n-krotna
        if(boundary==NULL || boundary(t_variable,variable_no)) {
            double template=f(t_variable,variable_no);
            for(int i=0;i<variable_no;i++){
                template*=height_tab[i];
            }
           *p_sum+=template;
        }
    }
    else{
        double height=(variable_lim[level][1]-variable_lim[level][0])/tn[level]; //wysokosc przedzialu na danym poziomie
        height_tab[level]=height;
        t_variable[level]=(variable_lim[level][0]+(variable_lim[level][0]+height))/2; //przesuwamy, nasza wartosc o stala wysokosc aby otrzymac prostokatnie srodek
        for(int i=0;i<tn[level];i++){
            recur_quad_rect_mid_help_funtion(p_sum,f,variable_no,t_variable,variable_lim,tn,level+1,boundary,height_tab);
            t_variable[level]+=height;
        }
    }
}

void recur_quad_rect_mid(double *p_sum, FuncNvFp f, int variable_no, double t_variable[], //tablica t_variabile bedzie mi sluzyla do przekazywania kolejnmych arugmentow na kolejeny poziom
                                      double variable_lim[][2], const int tn[], int level,BoundNvFp boundary) {
    double height_tab[variable_no]; //tablica potrzebna do przechowywania, aktualnych przedzialow
    recur_quad_rect_mid_help_funtion(p_sum,f,variable_no,t_variable,variable_lim,tn,level,boundary,height_tab);
}


int main(void) {
    int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
	int no_funcs = sizeof(func_tab) / sizeof(Func1vFp);
	int no_quads = sizeof(quad_tab) / sizeof(QuadratureFp);
	double a, b, x1, x2, y1, y2, hy, sum, delta;
	double t_variable[N_MAX], variable_lim[N_MAX][2];
	int tn[N_MAX];

	scanf("%d", &to_do);
	switch (to_do) {
		case 1: // loop over quadratures and integrands
			scanf("%lf %lf %d", &a, &b, &n);
			for(int q = 0; q < no_quads; ++q) {
				for(int f = 0; f < no_funcs; ++f) {
					printf("%.5f ",quad_select(f, q, a, b, n));
				}
				printf("\n");
			}
			break;
		case 2: // adaptive algorithm
			scanf("%d %d",&integrand_fun_no,&method_no);
			scanf("%lf %lf %lf", &a, &b, &delta);
			printf("%.5f\n", init_recurs(func_tab[integrand_fun_no],a,b,delta,quad_tab[method_no]));
			break;
		case 3: // double integral over a rectangle
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
			break;
		case 4: // double integral over normal domain
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf", &hy);
			printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
			break;
		case 5: // double integral over multiple normal domains
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n",dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
			break;
		case 6: // triple integral over a cuboid
			scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], tn);
			scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], tn+1);
			scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], tn+2);
			scanf("%d", &flag);
			printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, flag ? bound3v : NULL));
			break;
		case 7: // multiple integral over hyper-cuboid
			scanf("%d", &n);
			if(n > N_MAX) break;
			for(int i = 0; i < n; ++i) {
				scanf("%lf %lf %d", &variable_lim[i][0], &variable_lim[i][1], tn+i);
			}
			scanf("%d", &flag);
			sum = 0.;
			recur_quad_rect_mid(&sum, funcNv, n, t_variable, variable_lim, tn, 0, flag ? boundNv : NULL);
			printf("%.5f\n", sum);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
	return 0;
}