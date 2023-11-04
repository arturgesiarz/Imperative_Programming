#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
    int first;
    int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);
void print_int_array(int *array, int n);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair*, int); //czy kazdy element jest w relacji z samym soba "is_reflexive(relation, size));"
int is_reflexive(pair *relation,int size){
    int flag;
    int act_f;
    int act_s;
    int act_const;
    for(int i=0;i<size;i++){ //funkcja sprwadzajaca czy pierwsze elementy sa ze soba w relacji
        act_const=relation[i].first;
        flag=0;
        for(int j=0;j<size;j++) {
            act_f = relation[j].first;
            act_s=relation[j].second;
            if (act_const == act_f && act_const==act_s) flag = 1;
        }
        if(!flag){
            return 0;
        }
    }
    for(int i=0;i<size;i++){ //funkcja sprwadzajaca czy pierwsze elementy sa ze soba w relacji
        act_const=relation[i].second;
        flag=0;
        for(int j=0;j<size;j++) {
            act_f = relation[j].first;
            act_s=relation[j].second;
            if (act_const == act_f && act_const==act_s) flag = 1;
        }
        if(!flag){
            return 0;
        }
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair*, int);
int is_irreflexive(pair *relation,int size){
    int test;
    test=is_reflexive(relation,size);
    if(test) return 0;
    else return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair*, int);
int is_symmetric(pair *relation,int size){
    int flag;
    int act_f;
    int act_s;
    int act_const_f;
    int act_const_s;
    for(int i=0;i<size;i++){ //funkcja sprwadzajaca czy pierwsze elementy sa ze soba w relacji
        act_const_f=relation[i].first;
        act_const_s=relation[i].second;
        flag=0;
        for(int j=0;j<size;j++) {
            act_f = relation[j].first;
            act_s=relation[j].second;
            if (act_const_f==act_s && act_const_s==act_f) flag = 1;
        }
        if(!flag){
            return 0;
        }
    }

    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair*, int);
int is_antisymmetric(pair *relation,int size){
    int flag;
    int act_f;
    int act_s;
    int act_const_f;
    int act_const_s;
    for(int i=0;i<size;i++){ //funkcja sprwadzajaca czy pierwsze elementy sa ze soba w relacji
        act_const_f=relation[i].first;
        act_const_s=relation[i].second;
        flag=1;
        for(int j=0;j<size;j++) {
            act_f = relation[j].first;
            act_s=relation[j].second;
            if (act_const_f==act_s && act_const_s==act_f){
                if(act_f!=act_s) flag = 0;
            }
        }
        if(!flag){
            return 0;
        }
    }

    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair*, int);
int is_asymmetric(pair *relation,int size){
    int flag;
    int act_f;
    int act_s;
    int act_const_f;
    int act_const_s;
    for(int i=0;i<size;i++){ //funkcja sprwadzajaca czy pierwsze elementy sa ze soba w relacji
        act_const_f=relation[i].first;
        act_const_s=relation[i].second;
        flag=1; //flaga mi mowi ze na poczatku ze relacja jest asymetryczna
        for(int j=0;j<size;j++) {
            act_f = relation[j].first;
            act_s=relation[j].second;
            if (act_const_f==act_s && act_const_s==act_f) flag = 0; //jesli natrafie na jakas symerie to wiem ze nie jest asymetryczna
        }
        if(!flag){
            return 0;
        }
    }

    return 1;
}
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair*, int);
int is_transitive(pair *relation,int size){
    int flag;
    int act_s;
    int act_f;
    int act_const_s;
    int act_const_f;
    int search_f;
    int search_s;
    int a;
    int b;
    for(int i=0;i<size;i++){
        act_const_f=relation[i].first;
        act_const_s=relation[i].second;
        flag=1;
        for(int j=0;j<size;j++) {
            act_f=relation[j].first;
            act_s=relation[j].second;
            if (act_const_s==act_f) {
                search_f=act_const_f;
                search_s=act_s;
                flag=0; //bo bedzie tutaj nastepowalo sprawdznie, i dopiero tutja daje flag=0, ponieaz wynika to z definicji
                for(int k=0;k<size;k++){
                    a=relation[k].first;
                    b=relation[k].second;
                    if(a==search_f && b==search_s) {
                        flag=1;
                        break;
                    }
                }
                if(flag) break;
            }
        }
        if(!flag){
            return 0;
        }
    }

    return 1;
}

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair*, int);
int is_partial_order(pair *relation,int size){
    int reflexive=0;
    int transitiv=0;
    int antisymmetric=0;
    if(is_reflexive(relation,size)) reflexive=1;
    if(is_transitive(relation,size)) transitiv=1;
    if(is_antisymmetric(relation,size)) antisymmetric=1;
    if(reflexive && transitiv && antisymmetric) return 1;
    else return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair*, int);

// A total order relation is a partial order relation that is connected

int is_total_order(pair*, int);
int is_total_order(pair *relation,int size){
    int partial_order=0;
    int connected=0;
    if(is_connected(relation,size)) connected=1;
    if(is_partial_order(relation,size)) partial_order=1;
    if(connected && partial_order) return 1;
    else return 0;
}

int is_connected(pair *relation,int size){ //funkcja sprawdzajaca czy dana relacja jest spojna

    const int amount_elements_in_tab=2*size;
    int tab[amount_elements_in_tab]; //bede zapisywal tutaj kazdy element ktory zostal w relajci wpisany
    int flag=0; //zmienna ta bedzie mi sluzyla do zapisju elementow wystepujacych w relacji
    int counter=0;

    for(int i=0;i<amount_elements_in_tab;i++){
        if(!flag) { //wykonuje na zmiane te instruk
            tab[i]=relation[counter].first;
            flag=1;
            continue;
        }
        else{
            tab[i]=relation[counter].second;
            flag=0;
            counter++;
        }
    }

    int get1;
    int get2;
    int a;
    int b;

    for(int i=0;i<amount_elements_in_tab;i++){
        get1=tab[i]; //element sluzacy do porownywanie elementow czy bedzie istniala podana relacja
        for(int j=0;j<amount_elements_in_tab;j++){
            if(i!=j){
                flag=0;
                get2=tab[j];
                for(int k=0;k<size;k++){ //przesukiwanie glownej tablicy, w celu znalezienia takich elementow z get1 i get2
                    a=relation[k].first;
                    b=relation[k].second;
                    if(a==get1 && b==get2) flag=1;
                }
                if (!flag) return 0; //ponieaz jakis element nie zostal znaleziony z mojej listy
            }
        }
    }
    return 1;
}

int get_domain(pair*, int, int*);

//Element g ∈ X jest elementem maksymalnym jeżeli nie istnieje element x ∈ X, taki że x != g i gRx.
int find_max_elements(pair*, int, int*);
int find_max_elements(pair *relation, int size, int *max_elements){

    int maybe_max=0;
    int counter_max=0;
    int act_s=0;
    int act_f=0;
    int flag=0;

    int real_domain_size=0;
    int domain[MAX_REL_SIZE];
    real_domain_size=get_domain(relation,size,domain);

    for(int i=0;i<real_domain_size;i++){
        maybe_max=domain[i];
        flag=1;
        for(int j=0;j<size;j++){
            act_s=relation[j].second;
            act_f=relation[j].first;
            if(act_s!=maybe_max && act_f==maybe_max) {
                flag=0;
                break;
            }
        }

        if(flag){ //znaczy ze znalezlimy maksa
            max_elements[counter_max]=maybe_max;
            counter_max++;
        }
    }
    return counter_max;
}
//Podobnie element m ∈ X jest elementem minimalnym jeżeli nie istnieje element x ∈ X taki, że x != m i xRm.
int find_min_elements(pair*, int, int*);
int find_min_elements(pair *relation,int size,int *min_elements){

    int maybe_max=0;
    int counter_max=0;
    int act_s=0;
    int act_f=0;
    int flag=0;

    int real_domain_size=0;
    int domain[MAX_REL_SIZE];
    real_domain_size=get_domain(relation,size,domain);

    for(int i=0;i<real_domain_size;i++){
        maybe_max=domain[i];
        flag=1;
        for(int j=0;j<size;j++){
            act_s=relation[j].second;
            act_f=relation[j].first;
            if(act_f!=maybe_max && act_s==maybe_max) {
                flag=0;
                break;
            }
        }

        if(flag){ //znaczy ze znalezlimy maksa
            min_elements[counter_max]=maybe_max;
            counter_max++;
        }

    }
    return counter_max;
}

int partition(int*,int, int);
int quicksort(int*,int,int);


int get_domain(pair *relation, int size,int *domain){ //MAX_REL_SIZE !! - DOMIAN -> zrobic na osobnej tablicy a nastepnie przepisac do domian!!
    const int amount_elements_in_tab=2*size;
    int tab[amount_elements_in_tab];
    int flag=0;
    int counter=0;

    for(int i=0;i<amount_elements_in_tab;i++){
        if(!flag) { //wykonuje na zmiane te instruk
            tab[i]=relation[counter].first;
            flag=1;
            continue;
        }
        else{
            tab[i]=relation[counter].second;
            flag=0;
            counter++;
        }
    }
    quicksort(tab,0,amount_elements_in_tab-1); //nastepuje sortowanie tablicy
    int pointer=0; //zmienna sluzaca do wklejnia zawrotosci do tablicy domain
    for(int i=0;i<amount_elements_in_tab-1;i++){ //przechodze po calej tablicy w celu zapisania do nowej talbicy nowych wartosci
        if(tab[i]!=tab[i+1]){ //znaczy to tyle, ze znalezlismy punkt krytyczny i musimy dodac nasze wartosci do tablicy domain
            domain[pointer]=tab[i]; //pamietac o dodaniu ostatniego elementu
            pointer++;
        }
    }
    domain[pointer]=tab[amount_elements_in_tab-1];

    return pointer+1;

}


// Case 3:

int composition (pair*, int, pair*, int, pair*);
int composition(pair *relation, int size, pair *relation_2, int size_2, pair *comp_relation){

    int act_s;
    int act_f;
    int act_const_s;
    int act_const_f;

    int counter_elements=0;
    pair object;

    for(int i=0;i<size;i++){
        act_const_f=relation[i].first;
        act_const_s=relation[i].second;
        for(int j=0;j<size_2;j++){
            act_f=relation_2[j].first;
            act_s=relation_2[j].second;
            if(act_const_s==act_f){
                    object.first=act_const_f;
                    object.second=act_s;
                    if(add_relation(comp_relation,counter_elements,object)){
                        //printf("%d %d\n",object.first,object.second);
                        counter_elements++;
                    }
            }
        }
    }
    return counter_elements;
}

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
    return 0;
}

int insert_int (int *tab, int n, int new_element) {
    return 0;
}

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
    if(n+1==MAX_REL_SIZE){ //musimy ten waruenk zrobic zebysmy nie dodali elementu, do naszej pelnej tablicy
        return 0;
    }
    int probe_a=new_pair.first;
    int probe_b=new_pair.second;
    int act_a;
    int act_b;
    int flag=1;
    for(int i=0;i<n;i++){ //sprwadzam czy przypadkiem nie wystapila juz taka relacja jak w new_pair
        act_a=tab[i].first;
        act_b=tab[i].second;
        if(act_a==probe_a && act_b==probe_b){ //znacyz to ze nie znalezlismy juz istneija pare i  dalej nie bedzie jej dodowac
            flag=0;
            break;
        }
    }
    if(flag){
        tab[n].first=probe_a;
        tab[n].second=probe_b;
        return 1; //znaczy to ze poprawnie dodano
    }
    return 0;

}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) { //dodaje tyle, par ile uprzednio podalem
    int n=0;
    int a=0;
    int b=0;
    scanf("%d",&n); //liczba par relacji, ktore maja wystapic
    for(int i=0;i<n;i++){
        scanf("%d",&a);
        scanf("%d",&b);
        relation[i].first=a;
        relation[i].second=b;
    }
    return n;
}

void print_int_array(int *array, int n) {
    printf("%d\n",n);
    for(int i=0;i<n;i++){
        printf("%d ",array[i]);
    }
    printf("\n");
}

int partition(int *tab,int p, int r){
    int template=0;
    int x=tab[p];
    int i=p;
    int j=r;
    while(1){
        while(tab[i]<x) i++;
        while(tab[j]>x) j--;
        if(i<j){ //musi tutac nastapic zamiana naszych elementow
            template=tab[i];
            tab[i]=tab[j];
            tab[j]=template;
        }
        else return j;
        i++;
        j--;
    }
}
int quicksort(int *tab,int p,int r){
    while(p<r){
        int q=partition(tab,p,r);
        quicksort(tab,p,q);
        p=q+1;
    }
}

int main(void) {
    int to_do;
    pair relation[MAX_REL_SIZE]; //czyli maksymlanie mamy 100 relacji!
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];
    int max_elements[MAX_REL_SIZE];
    int min_elements[MAX_REL_SIZE];

    scanf("%d",&to_do);
    int size = read_relation(relation);
    int ordered, size_2, n_domain;

    switch (to_do) {
        case 1:
            printf("%d ", is_reflexive(relation, size));
            printf("%d ", is_irreflexive(relation, size));
            printf("%d ", is_symmetric(relation, size));
            printf("%d ", is_antisymmetric(relation, size));
            printf("%d ", is_asymmetric(relation, size));
            printf("%d\n", is_transitive(relation, size));
            break;
        case 2:
            ordered = is_partial_order(relation, size);
            n_domain = get_domain(relation, size, domain);
            printf("%d %d\n", ordered, is_total_order(relation, size));
            print_int_array(domain, n_domain);
            if (!ordered) break;
            int no_max_elements = find_max_elements(relation, size, max_elements);
            int no_min_elements = find_min_elements(relation, size, min_elements);
            print_int_array(max_elements, no_max_elements);
            print_int_array(min_elements, no_min_elements);
            break;
        case 3:
            size_2 = read_relation(relation_2);
            printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}