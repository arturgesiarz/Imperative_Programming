#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#define TWO_DICE_SUM 11

// Calculates arithmetic mean and variance of numbers from n-element array v[]
// for n = 0: mean = variance = 0

void print_histogram(const double v[], int n, int x_start, double y_scale, char mark) {
    double characters=0;
    int number=0;
    int flag=1;
    for(int i=x_start-2;i<n;i++){
        flag=1;
        characters=round(v[i]/y_scale);
        number=i+2;
        printf("%2d |",number);
        int j=0;
        do{
            if(characters<=0){
                printf(" 0\n");
                flag=0;
                break;
            }
            printf("%c",mark);
            j++;
        }
        while(j<characters);
        if(flag==1) printf(" %.3f\n",v[i]);
    }

}

int rand_from_interval(int a, int b) {
    if(a>b){
        return INT_MIN;
    }
    if(b-a>RAND_MAX){
        return INT_MAX;
    }
    if(a==b){
        return a;
    }
    int los=0;
    los=rand()%(b-a+1)+a; //loswanie liczby z pewnego zakresu
    return los;
}

void mean_variance(const int v[], int n, double *mean, double *variance) {
    double sum=0;
    double sum_variance=0;
    for(int i=0;i<n;i++){
        sum+=v[i];
    }
    *mean=sum/n;
    for(int i=0;i<n;i++){
        sum_variance+=(v[i]-*mean)*(v[i]-*mean);
    }
    *variance=sum_variance/n;
}

// Fill n-element array v[] with Bernoulli sample (binary values)

void bernoulli_gen(int v[], int n, double probability) {
    double los=0;
    double quest=0;
    int result=0;
    quest=probability*(RAND_MAX);
    for(int i=0;i<n;i++){
        los=rand_from_interval(0,RAND_MAX);
        if(los<quest){
            result=1;
        }
        else {
            result=0;
        }
        v[i]=result;
    }
}
// Calculate pmf (probability mass function) of discrete random variable.
// Two dice are thrown simultaneously The random variable is the sum of the
// numbers shown (the result is from [2, 12]).
// trials - number of trials
void pmf(double v[], int trials) { //dlugosc wektora musi byc rowna do 11.
    int los1=0;
    int los2=0;
    int sum_eye=0;
    int tab[11];
    for(int i=0;i<11;i++){
        tab[i]=0;
    }
    for(int i=0;i<trials;i++){
        los1= rand_from_interval(1,6);
        los2=rand_from_interval(1,6);
        sum_eye=los1+los2;
        v[sum_eye-2]++;
        tab[sum_eye-2]=1;
    }
    for(int i=0;i<TWO_DICE_SUM;i++){
        if(tab[i]==0) v[i]=0;
        else {
            v[i]=v[i]/trials;
        }
    }
}
// Calculates the cumulative distribution function (v) for 'trials' numbers
// Two dice are thrown simultaneously The random variable is the sum of the
// numbers shown (the result is from [2, 12]).
// trials - number of trials
void cdf(double v[], int trials) { //funkcja obliczajca dystrybuante
    pmf(v,trials); //tworze rozklad prawpodobienstwa
    double sum_acctualy=0;
    for(int i=0;i<TWO_DICE_SUM;i++){
        sum_acctualy+=v[i];
        v[i]=sum_acctualy;
    }

}

// Histogram - bar chart for the values from v[] array of length n
// x_start - the first value on the abscissa (x increment is 1),
// y_scale - y value corresponding to single char of the histogram



// Simulate Monty-Hall problem
// input: n - number of trials
// output: wins - number od wins if door switched
// output: win_probability - probability of win if door switched

// This function calls rand() exactly 2 times:
// the first one returns winning door number,
// the second one returns door number that the player has chosen the first time,
void monty_hall(int n, int *p_switch_wins) {  //0 1 2; 2-win; 0-pick;
    int win=0;
    int chose=0;
    int stay_win=0;
    int go_win=0;
    for(int i=0;i<n;i++){
        win=rand()%3;
        chose=rand()%3;
        if(chose==0){
            if(win==0){
                stay_win++;
            }
            if(win==1 || win==2){
                go_win++;
            }
        }
        else if(chose==1){
            if(win==1){
                stay_win++;
            }
            if(win==0 || win==2){
                go_win++;
            }
        }
        else if(chose==2){
            if(win==2){
                stay_win++;
            }
            if(win==1 || win==0){
                go_win++;
            }
        }
    }
    *p_switch_wins=go_win;

}

// print double vector of size n (with 2 figures after the decimal point)
//void print_vector(const double v[], int n) {
//	for (int i = 0; i < n; ++i) {
//		printf("%.2f ", v[i]);
//	}
//	printf("\n");
//}

// print integer vector
void print_int_vector(const int v[], int n) {
	for (int i = 0; i < n; ++i) {
		printf("%d ", v[i]);
	}
	printf("\n");
}


void fill_with_randoms(int i_vector[],int n,int a,int b){ //funkcja wypelniajaca tablice n elementowa, liczbami losowami z przedzialu a oraz b
    for(int i=0;i<n;i++) i_vector[i]=rand_from_interval(a,b);
}


int main(void) {

	char mark;
	int to_do, n, seed, m_h_wins, a, b, i_vector[100];
	double arithmetic_mean, variance, probability, d_vector[100];
	scanf("%d", &to_do);
	scanf("%d",&seed);
	scanf("%d",&n);
	srand((unsigned)seed);

	switch (to_do) {
		case 1: // mean_variance
			scanf("%d %d", &a, &b);
			fill_with_randoms(i_vector, n, a, b);
			mean_variance(i_vector, n, &arithmetic_mean, &variance);
			printf("%.2f %.2f\n", arithmetic_mean, variance);
			break;
		case 2: // bernoulli_gen
			scanf("%lf", &probability);
			bernoulli_gen(i_vector, n, probability);
			print_int_vector(i_vector, n);
			break;
		case 3: // pmf
			scanf(" %c", &mark);
			pmf(d_vector, n);
			print_histogram(d_vector, TWO_DICE_SUM, 2, 0.005, mark);
			break;
		case 4: // cdf
			scanf(" %c", &mark);
			cdf(d_vector, n);
			print_histogram(d_vector, TWO_DICE_SUM, 2, 0.02, mark);
			break;
		case 5: // monty_hall
			monty_hall(n, &m_h_wins);
			printf("%d %d\n", m_h_wins, n - m_h_wins);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}