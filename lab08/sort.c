#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day;
	int month;
	int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
	char art[20];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);


int compare_price(const void* _a, const void* _b){ // nie przerobione
    Food a = *((Food*)_a);
    Food b = *((Food*)_b);

    if(a.price < b.price)
        return -1;
    else if(a.price > b.price)
        return 1;
    else{
        return 0;
    }
}

// compare dates
// returns -1 when d2 is bigger, 1 when d1 is bigger, 0 elsewhere
int cmp_date(const void *d1, const void *d2){  // OK
    Date a = *((Date*)d1);
    Date b = *((Date*)d2);

    if(a.year < b.year)
        return -1;
    else if(a.year > b.year)
        return 1;
    else {
        if(a.month < b.month)
            return -1;
        else if(a.month > b.month)
            return 1;
        else{
            if(a.day < b.day)
                return -1;
            else if(a.day > b.day)
                return 1;
            else
                return 0;
        }
    }
}

int compare_date_food(const void* _a, const void* _b){
    Food a = *((Food*)_a);
    Food b = *((Food*)_b);

    return cmp_date(&a.valid_date, &b.valid_date);
}

// compare foods
int cmp(const void *a1, const void *b1) { // OK
    Food a = *((Food*)a1);
    Food b = *((Food*)b1);

    int str = strcmp(a.art, b.art);
    if (str < 0)
        return -1;
    else if (str > 0)
        return 1;
    else{
        int price = compare_price(a1, b1);
        if(price < 0)
            return -1;
        else if (price > 0)
            return 1;
        else {
            int date = compare_date_food(a1, b1);
            if (date < 0)
                return -1;
            else if (date > 0)
                return 1;
            else
                return 0;
        }
    }
}

// bsearch returning address where to insert new element
void *bsearch2 (const void *key, const void *base, size_t nmemb, size_t size, ComparFp compar, char *result) {
    if (nmemb == 0) {
        *result = 0;
        return base;
    }

    size_t middle = nmemb / 2;
    void* middle_ptr = (void*)base + middle * size;
    int cmp = compar(key, middle_ptr);

    if (cmp == 0) {
        *result = 1;
        return middle_ptr;
    } else if (cmp < 0) {
        return bsearch2(key, base, middle, size, compar, result);
    } else {
        return bsearch2(key, middle_ptr + size, nmemb - middle - 1, size, compar, result);
    }
}

// print goods of given name
void print_art(Food *p, int n, char *art) {
    for (int i = 0; i < n; i++){
        int comparison = strcmp(p[i].art, art);
        if (comparison == 0){
            if (p[i].valid_date.day < 10 && p[i].valid_date.month < 10){
                printf("%.2f %.2d 0%d.0%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
            }
            else if (p[i].valid_date.day < 10){
                printf("%.2f %.2d 0%d.%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
            }
            else if (p[i].valid_date.month < 10){
                printf("%.2f %.2d %d.0%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
            }
            else{
                printf("%.2f %.2d %d.%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
            }
        }
    }
}

// add record to table if absent
Food* add_record(Food *tab, size_t tab_size, int *np, ComparFp compar, Food *new) {
    char result;

    Food* address = bsearch2(new, tab, *np, sizeof(Food), compar, &result);


    if(result == 1){
        address->amount += new->amount;
    } else {
        if(*np == tab_size)
            return NULL;

        int index = (int)(address - tab);
        (*np)++;
        int k = *np;

        for(; k > index; k--)
            tab[k] = tab[k-1];

        tab[index] = *new;
    }

    return address;
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {
    int np = 0;
    for(int i = 0; i < no; i++){
        Food f;
        memset(f.art, 0, 20);
        fscanf(stream,"%s", f.art);
        fscanf(stream,"%f", &f.price);
        fscanf(stream,"%d", &f.amount);
        fscanf(stream,"%d.%d.%d", &f.valid_date.day, &f.valid_date.month, &f.valid_date.year);

        add_record(tab, RECORD_MAX, &np, cmp, &f);

    }
    return np;
}

int cmp_qs(const void *a, const void *b) {
	Food *fa = (Food*)a, *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
	Date *pd = (Date*)a;
	Food *fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days) {
    time_t raw_time;
    time(&raw_time);

    struct tm* target_date = localtime(&raw_time);
    target_date->tm_mday = curr_date.day;
    target_date->tm_mon = curr_date.month - 1;
    target_date->tm_year = curr_date.year - 1900;

    mktime(target_date);

    target_date->tm_mday += days;

    mktime(target_date);

    Date valid_date = (Date){target_date->tm_mday, target_date->tm_mon + 1, target_date->tm_year + 1900};

    qsort(food_tab, n, sizeof(Food), compare_date_food);
    Food f = {
            .valid_date = valid_date,
    };
    char res;
    Food* address = bsearch2(&f, food_tab, n, sizeof(Food), compare_date_food, &res);

    if(res == 0)
        return 0;

    float result = 0;

    int index = (int)(address - food_tab);
    int tmp = index;

    while(tmp >= 0 && cmp_date(&food_tab[tmp].valid_date, &valid_date) == 0){
        result += food_tab[tmp].price * food_tab[tmp].amount;
        tmp--;
    }
    tmp = index + 1;

    while(tmp < n && cmp_date(&food_tab[tmp].valid_date, &valid_date) == 0){
        result += food_tab[tmp].price * food_tab[tmp].amount;
        tmp++;
    }

    return result;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
	enum Sex sex:1;
	enum BOOL pretendent:1;
};

typedef struct {
	char *name;
	struct Bit_data bits;
	Date born;
	char *parent;
} Person;

typedef struct {
	char *par_name;
    int index_first;
    int index_last;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
	if (person1->bits.sex == person2->bits.sex) return 0;
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
	return person2->bits.sex - person1->bits.sex;
}


int parent_cmp(const void* _a, const void* _b){
    Parent a = *((Parent*)_a);
    Parent b = *((Parent*)_b);

    return strcmp(a.par_name, b.par_name);
}
// compare persons
int cmp_person(const void *_a, const void *_b) {
    Person a = *((Person*)_a);
    Person b = *((Person*)_b);

    if(a.parent == NULL)
        return 1;
    if(b.parent == NULL)
        return -1;

    return strcmp(a.parent, b.parent);
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
    int no_parents = 0;

    for(int i = 0; i < size; i++){
        int found = 0;

        for(int u = 0; u < no_parents; u++){
            if(strcmp(pers_tab[i].name, idx_tab[u].par_name) == 0){
                found = 1;
                break;
            }
        }

        if(!found){
            idx_tab[no_parents].par_name = pers_tab[i].name;

            Person key = {
                    .parent = pers_tab[i].name
            };

            Person* address = bsearch(&key, pers_tab, size, sizeof(Person), cmp_person);

            if(address == NULL)
                continue;

            int index = (int)(address - pers_tab);
            int tmp = index;

            while(tmp > 0 && cmp_person(&pers_tab[tmp], &pers_tab[tmp-1]) == 0)
                tmp--;

            idx_tab[no_parents].index_first = tmp;
            tmp = index;

            while(tmp < size - 1 && cmp_person(&pers_tab[tmp], &pers_tab[tmp+1]) == 0)
                tmp++;

            idx_tab[no_parents].index_last = tmp;

            no_parents++;
        }
    }

    qsort(idx_tab, no_parents, sizeof(Parent), parent_cmp);

    return no_parents;
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
    int index = 0;

    Person tmp[33];

    while (index < size){
        Parent key = {.par_name = person_tab[index].name};
        Parent* address = bsearch(&key, idx_tab, no_parents, sizeof(Parent), parent_cmp);

        if(address != NULL) {
            int to_copy = address->index_last - address->index_first + 1;

            memmove(tmp, &person_tab[address->index_first], to_copy * sizeof(Person));
            memmove(&person_tab[index + to_copy + 1], &person_tab[index + 1],
                    ((address->index_first) - index - 1) * sizeof(Person));
            memmove(&person_tab[index + 1], tmp, to_copy * sizeof(Person));

            for(int i = 0; i < no_parents; i++){
                if(idx_tab[i].index_first < address->index_first) {
                    idx_tab[i].index_first += to_copy;
                    idx_tab[i].index_last += to_copy;
                }
            }
        }

        index += 1;
    }
}

int cleaning(Person *person_tab, int n) {
    for(int i = 0; i < n; i++){
        if(person_tab[i].bits.pretendent == no){
            memmove(&person_tab[i], &person_tab[i + 1], (n - i)*sizeof(Person));
            i--;
            n--;
        }
    }

    return n;
}

void print_person(const Person *p) {
	printf("%s\n", p->name);
}

int compare(const void* _a, const void* _b){
    Person a = *((Person*)_a);
    Person b = *((Person*)_b);

    if(a.parent == NULL)
        return -1;
    if(b.parent == NULL)
        return 1;

    int parent = strcmp(a.parent, b.parent);

    if(parent < 0)
        return -1;
    else if(parent > 0)
        return 1;
    else {
        int born_date = cmp_date(&a.born, &b.born);

        if(cmp_date(&a.born, &primo_date) < 0 && cmp_date(&b.born, &primo_date) < 0) {
            if (a.bits.sex == M && b.bits.sex == F)
                return -1;
            else if (a.bits.sex == F && b.bits.sex == M)
                return 1;
        }

        if(born_date < 0)
            return -1;
        else if(born_date > 0)
            return 1;
        else {
            if (a.bits.sex > b.bits.sex)
                return 1;
            else if (a.bits.sex < b.bits.sex)
                return -1;
            else
                return 0;
        }
    }
}
int create_list(Person *person_tab, int n) {
    qsort(person_tab, n, sizeof(Person), compare);

    Parent parents_tab[33];

    int no_parents = fill_indices_tab(parents_tab, person_tab, n);

    persons_shifts(person_tab, n, parents_tab, no_parents);

    return cleaning(person_tab, n);
}

int main(void) {
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[ART_MAX];
	fgets(buff, ART_MAX, stdin);
	sscanf(buff, "%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 0);
			Date curr_date;
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d",&no);
			int no_persons = sizeof(person_tab) / sizeof(Person);
			create_list(person_tab,no_persons);
			print_person(person_tab + no - 1);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}