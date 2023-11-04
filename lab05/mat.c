#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
    for(int i = 0; i < n; ++i) {
        scanf("%lf", x++);
    }
}
void print_vector(double x[], int n) {
    for(int i = 0; i < n; ++i) {
        printf("%.4f ", x[i]);
    }
    printf("\n");
}
void read_mat(double A[][SIZE], int m, int n) {
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            scanf("%lf", &A[i][j]);
        }
    }
}
void print_mat(double A[][SIZE], int m, int n) {
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            printf("%.4f ", A[i][j]);
        }
        printf("\n");
    }
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) { // m-wiersze macierzy A; p-kolumny macierzy A; p-wiersze macierzy B; n-kolumny macierzy B
    double temp1,temp2,sum=0;
    double solution;
    for(int i=0;i<m;i++){ //bede tutaj sie przesuwal po kolejnych wierszach macierzy A
        for(int k=0;k<n;k++){ //bede sie przesuwal po nastepnych kolumnach macierzy B
            for(int j=0;j<p;j++){ //bede sie tutaj przesuwal po nastepnych kolumnach macierzy A
                temp1=A[i][j];
                temp2=B[j][k];
                sum+=(temp1*temp2);
            }
            solution=sum;
            AB[i][k]=solution;
            sum=0;
        }
    }
}
// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) { //najpiew mnoze wszstkie kolumny w danym wieszu na tym poziomie przez liczbe ktorej jest rowna przez tej co jest najwyzej

    double top;
    double template;
    double determiniant=1;

    for(int i=0;i<n-1;i++){
        top=A[i][i];
        if(top==0){
            return NAN;
        }
        for(int j=i+1;j<n;j++){ //chce poruszac sie w dol po kolumnach
            template=A[j][i];
            if(template!=0){ //musimy unikac dzielenia przez 0, tak wiec po prostu skipujemy przypadek
                for(int m=i;m<n;m++){ //chce wymnozyc wszystkie elementy
                    A[j][m]=(A[j][m]/template)*top-A[i][m];
                }
                determiniant*=1/(top/template); //wynika to z definicji, wyznacznika. Musze, za kazdym razem kiedy mnoze wiersz lub kolumne przez skalar, przez wynikowa musze przez ten skalar podzielic!
            }
        }
    }
    for(int i=0; i<n;i++){
        determiniant*=A[i][i];
    }
    return determiniant;

}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) { //dokonczyc, wymnozenie x przez maceirz A

    double template;
    double max;
    double current_value=0;
    double temp; //zmienna pomocniczna
    double top; //real_value

    double determiniant=1;

    int save_row=0; //zmienna zapisuje wiersz w ktorym wystepuje najwiekszy element, bedzie on zamieniony z wierszem glownym!
    int switch_max;

    for(int i=0;i<n;i++){
        x[i]=b[i]; //przepisuje elementy tablicy b, poniewaz jest stala
    }

    for(int i=0;i<n-1;i++){
        max=A[i][i];
        if(max<0) max*=(-1);
        top=A[i][i];
        switch_max=0;
        for(int j=i;j<n;j++){  //bede szukal teraz najwieszkego elementu jako wartosc bezwgledna i zamienie konkretne wiersze.
            current_value= A[j][i];
            if(A[j][i]<0) current_value*=(-1);
            if(current_value>max){
                top=A[j][i];
                switch_max=1;
                max=current_value;
                save_row=j;
            }
        }
        if(switch_max){ //natepuje tutaj zamiana na najwiekszy element
            for(int j=0;j<n;j++){
                temp=A[i][j];
                A[i][j]=A[save_row][j];
                A[save_row][j]=temp;
            }
            temp=x[i];
            x[i]=x[save_row];
            x[save_row]=temp;
            determiniant*=(-1); //zmieniam wyznacznik z kazda chwila, zmiany wiersza
        }
        if(max<eps) return 0;
        for(int j=i+1;j<n;j++){
            template=A[j][i];
            if(template!=0){
                for(int m=i;m<n;m++){
                    A[j][m]=(A[j][m]/template)*top-A[i][m];
                }
                x[j]=(x[j]/template)*top-x[i];
                determiniant*=1/(top/template);
            }
        }

    }

    double counterer;
    int counter=0;

    x[n-1]/=A[n-1][n-1]; //pozyskuje ostatni element z wlasnosci macierzy trojkatnej

    for(int i=n-1;i>=0;i--){
        counter++; //zwieksza mi ilosc, zmiennych
        counterer=0;
        for(int j=n-1;j>=n-counter+1;j--){ //idziemy do przeostatniego elementu
            counterer+=A[i][j]*x[j];
            if(j==n-counter+1){ //przypadek ostatniego elementu
                x[n-counter]=(x[n-counter]-counterer)/A[i][n-counter];
            }
        }
    }

    for(int i=0; i<n;i++){
        determiniant*=A[i][i];
    }

    return determiniant;

}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {

    double determiniant;
    double probe;

    for(int i=0;i<n;i++){ //stworzenie macierzy jednostkowej
        for(int j=0;j<n;j++){
            B[i][j]=A[i][j];
        }
    }
    double x[n]; //wrzucam cokolwiek do tej tablicy
    determiniant= gauss(B,x,x,n,eps); //uzywam funkcji liczajcej wyznacznik

    for(int i=0;i<n;i++){ //stworzenie macierzy jednostkowej
        for(int j=0;j<n;j++){
            if(i==j) B[i][j]=1;
            else B[i][j]=0;
        }
    }

    double template;
    double max;
    double current_value=0;
    double temp;
    double top;

    int save_row=0;
    int switch_max;


    for(int i=0;i<n-1;i++){
        max=A[i][i];
        if(max<0) max*=(-1);
        top=A[i][i];
        switch_max=0;
        for(int j=i;j<n;j++){
            current_value= A[j][i];
            if(A[j][i]<0) current_value*=(-1);
            if(current_value>max){
                top=A[j][i];
                switch_max=1;
                max=current_value;
                save_row=j;
            }
        }
        if(switch_max){ //natepuje tutaj zamiana na najwiekszy element
            for(int j=0;j<n;j++){
                temp=A[i][j];
                A[i][j]=A[save_row][j];
                A[save_row][j]=temp;

            }

            for(int j=0;j<n;j++){
                temp=B[i][j];
                B[i][j]=B[save_row][j];
                B[save_row][j]=temp;
            }
        }
        if(max<eps) return 0;

        for(int j=i+1;j<n;j++){
            template=A[j][i];
            if(template!=0){
                for(int m=0;m<n;m++){
                    A[j][m]=(A[j][m]/template)*top-A[i][m];
                    B[j][m]=(B[j][m]/template)*top-B[i][m]; //analogicznie postepuje dla moiej macierzy jednostkowej
                    //if(A[j][m]<=0 && A[j][m]>0.00001) A[j][m]*=(-1);
                    //if(B[j][m]<=0 && B[j][m]>0.00001) B[j][m]*=(-1);
                }
            }
        }
    }



    for(int i=n-1;i>=0;i--){ //szukam macierzy trojatnej dolnej z macierzy trojkartnej dolnej czyli juz nie musze znajdowac maxiumum
        top=A[i][i];

        if(top<0) probe=top*(-1);
        else probe=top;

        if(probe<eps) return 0;

        for(int j=i-1;j>=0;j--){
            template=A[j][i];
            if(template!=0){
                for(int m=n-1;m>=0;m--){
                    A[j][m]=(A[j][m]/template)*top-A[i][m];
                    B[j][m]=(B[j][m]/template)*top-B[i][m]; //analogicznie postepuje dla moiej macierzy jednostkowej
                    //if(A[j][m]<=0 && A[j][m]>0.00001) A[j][m]*=(-1);
                    //if(B[j][m]<=0 && B[j][m]>0.00001) B[j][m]*=(-1);


                }
            }
        }
    }

    double multiplier;

    for(int i=0;i<n;i++){ //uzyjemy teraz, macierz A, do zapisu aby nie marnowac pamieci aby przekazac i z niego policzyc wyznacznik
        multiplier=(1/A[i][i]);
        A[i][i]=1;
        for(int j=0;j<n;j++){
            B[i][j]*=multiplier;
            if(B[i][j]>-eps && B[i][j]<eps) B[i][j]=0.0000001*(-1); //oczekuje ze moze byc to bardzo mala wartosc wiec ja zaokraglam 
        }
    }

    return determiniant;

}

int main(void) {
    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;

    int to_do;
    int m, n, p;

    scanf ("%d", &to_do);

    switch (to_do) {
        case 1:
            scanf("%d %d %d", &m, &p, &n);
            read_mat(A, m, p);
            read_mat(B, p, n);
            mat_product(A, B, C, m, p, n);
            print_mat(C, m, n);
            break;
        case 2:
            scanf("%d", &n);
            read_mat(A, n, n);
            printf("%.4f\n", gauss_simplified(A, n));
            break;
        case 3:
            scanf("%d", &n);
            read_mat(A,n, n);
            read_vector(b, n);
            det = gauss(A, b, x, n, eps);
            printf("%.4f\n", det);
            if (det) print_vector(x, n);
            break;
        case 4:
            scanf("%d", &n);
            read_mat(A, n, n);
            det = matrix_inv(A, B, n, eps);
            printf("%.4f\n", det);
            if (det) print_mat(B, n, n);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}