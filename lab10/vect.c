#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
	void *data; //wskaznik do poczatku alokowanej tablicy
	size_t element_size; //rozmiar pojedynczego elementu
	size_t size; //liczba elemntow wektora
	size_t capacity; //aktualna pojemnosc zaolokowanej pamieci
} Vector;

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) { //block-size jest to ilosc elmentow, na ktorych chcemy zaokolowc nasza pamiec
    vector->data=malloc(block_size*element_size);
    vector->capacity=block_size;
    vector->element_size=element_size;
    vector->size=0;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) { //funkcja zalakujaca tablice, w taki sposob jesli bedzie za mala aktualan wartosc, a chcemy ja powieszuysc wtedy musimy uzyc realokacji
    if(vector->capacity<new_capacity){ //przypadek, kiedy oczekiwana wielkosc tablicy jest wieksza niz, aktualna musimy powiekszyc nasza aktualna
        vector->data= realloc(vector->data,new_capacity*vector->element_size);
        vector->capacity=new_capacity;
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
    if(vector->size>new_size){ //przypadek kiedy, musimy pomniejszyc ilosc elementow w naszej strukturze -> musimy zwolnic odpowiednio pamiec czyli,
        void *new_memory=malloc(vector->element_size*new_size);
        void *memory_to_delete=vector->data; //wskaznik na pamiec, ktora ma zostac usunieta
        vector->data=memmove(new_memory,vector->data,vector->element_size*new_size);
        free(memory_to_delete); //usuwamy ostatnia pamiec, aby moc ja wykorzystac pozniej
        vector->size=new_size;
    }
    else if(vector->size<new_size){ //przypadek kiedy, musimy nasza nowa pamiec umiescic
        if(vector->capacity>new_size){ // jestli tak to po prostu nadpiszemy zerami poszegolne dane
            void *pointer_to_data=vector->data;
            for(int i=0;i<vector->size;i++) pointer_to_data=pointer_to_data+vector->element_size; //przesuwam sie o kolejny element moim wskaznikiem
            for(int i=(int)(vector->size);i<new_size;i++) {
                *(void**)pointer_to_data=0; //zeruje, konkreny obszar w pamieci
                pointer_to_data=pointer_to_data+vector->element_size; //przesuwam sie do nastepnego obszaru
            }
            vector->size=new_size;
        }
        else{ //znaczy to ze musimy, nowy obszar pamieci zarezerwowac
            void *pointer_to_data= calloc(vector->capacity*2,vector->element_size); //tworzymy gdzies nowy obszar w pamieci, gdzie przeniesiemy, nasz aktualny
            void *pointer_to_free_data=vector->data;
            vector->data=memmove(pointer_to_data,vector->data,vector->size*vector->element_size); //memmove przeniesie mi odpowiednio dane
            vector->size=new_size;
            vector->capacity*=2;
            free(pointer_to_free_data); //zwalniam miejsce z ktorego skopiowalem orginalnie
        }
    }
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
    if((int)vector->capacity>(int)vector->size+1){ //znaczy to, ze moge dodac bez problemu kolejny element, bez alokacji nowej pamieci
        void *pointer_to_data=vector->data; //wskaznik, potrzebny mi aby sie przemiescic na sam koniec
        for(int i=0;i<vector->size;i++) pointer_to_data=pointer_to_data+vector->element_size; //musze przesunac sie po elemencie aby zdobyc adres konkrentego miejsca w pamieci
        memcpy(pointer_to_data,value,vector->element_size);
        vector->size++;
    }
    else{
        void *pointer_to_new_data= malloc(vector->capacity*(vector->element_size*2)); //mnoze razy, dwa bo chce 2 razy taka pamiec zaalokowac, aby pozniej mi jej nie brakowalo
        void *pointer_to_clear_data=vector->data;
        vector->data=memmove(pointer_to_new_data,vector->data,vector->size*vector->element_size); //memmove przeniesie mi odpowiednio dane
        void *pointer_to_data=pointer_to_new_data;
        for(int i=0;i<vector->size;i++) pointer_to_data=pointer_to_data+vector->element_size;
        memcpy(pointer_to_data,value,vector->element_size);
        vector->size++;
        vector->capacity*=2; //powiekszylem dwukrotniem zakres mojej pamieci
        free(pointer_to_clear_data); //zwalniam miejsce z ktorego skopiowalem orginalnie
    }
}

// Remove all elements from the vector
void clear(Vector *vector) {
    free(vector->data); //zwalniamy, zaalokowana pamiec
    vector->size=0; //ustawiamy ilosc elementow wektora na 0, poniewaz wszystkie wczesniejsze usunelismy
    vector->capacity=0; //liczba dostepnego miejsca rowniez jest zerowa
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value) {
    if(vector->capacity>vector->size+1){ //znaczy to ze mam wystardzajaca pamiec aby dodac element a wiec nie musze dodatkowo, przydzielac pamieci
        void *pointer_to_moves_el=vector->data;
        int amount_elements_to_move=(int)vector->size-(int)index;
        for(int i=0;i<index;i++) pointer_to_moves_el=pointer_to_moves_el+vector->element_size; //pointer jest teraz ustawiony na konkretny element 'index'
        void *pointer_to_move_destination=pointer_to_moves_el+vector->element_size; //jest to cel naszych przenosin
        memmove(pointer_to_move_destination,pointer_to_moves_el,amount_elements_to_move*vector->element_size); //przenosimy dane o jeden element po prostu dalej
        memcpy(pointer_to_moves_el,value,vector->element_size); //kopiuje, do wskaznika na moj element insert, wlasnie wskaznik na value
        vector->size++;
    }
    else{ //znaczy to ze nie mam wsytarczajacej pamieci, i musze ja powiekszyc aby moc rozszerzyc moj wektor
        void *new_mem= malloc(vector->element_size*vector->capacity*2); //zwiekszamy mozliwy zakres do osiagniecia
        void *old_mem=vector->data;
        vector->data=memmove(new_mem,vector->data,vector->element_size*vector->size);
        free(old_mem);
        vector->capacity*=2;
        void *pointer_to_moves_el=vector->data;
        int amount_elements_to_move=(int)vector->size-(int)index;
        for(int i=0;i<index;i++) pointer_to_moves_el=pointer_to_moves_el+vector->element_size; //pointer jest teraz ustawiony na konkretny element 'index'
        void *pointer_to_move_destination=pointer_to_moves_el+vector->element_size; //jest to cel naszych przenosin
        memmove(pointer_to_move_destination,pointer_to_moves_el,amount_elements_to_move*vector->element_size); //przenosimy dane o jeden element po prostu dalej
        memcpy(pointer_to_moves_el,value,vector->element_size);
        vector->size++;
    }
}


// Erase element at position index
void erase(Vector *vector, size_t index) { //dodac sprawdzanie czy w ogole isteniej dany index
    if((int)index<(int)vector->size && (int)index>=0){ //zabezpiecznie pozwalajce unikac usuniecia czegos niewiadomo czego
        void *pointer_to_moves_el=vector->data;
        int amount_elements_to_move=(int)vector->size-(int)index-1; //poniewaz nie wliczam w przesuwanie elementu index
        for(int i=0;i<index;i++) pointer_to_moves_el=pointer_to_moves_el+vector->element_size; //pointer jest teraz ustawiony na konkretny element 'index'
        void *pointer_to_move_destination=pointer_to_moves_el+vector->element_size; //jest to cel naszych przenosin
        memmove(pointer_to_moves_el,pointer_to_move_destination,amount_elements_to_move*vector->element_size); //przenosimy dane o jeden element po prostu dalej
        vector->size--;
    }
}


// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp) {
    void *pointer_to_values=vector->data; //wskaznik, pomagajacy nam dotrzec do konkrentych wartosci naszego wektora
    int range=(int)vector->size;
    int counter=0;
    for(int i=0;i<range;i++){ //chcemy przejrzec wszystkie elementy, naszego wektora
        if(cmp(pointer_to_values,value)==0){ //jesli beda sobie rowne, wartosc szukana i wartosc w wektorze to nastapi wywolanie funkcji erase, ktora sobie z tym poradzi
            erase(vector,counter); //poniewaz udalo mi sie znalezc szukana wartosc, bede usuwal dany element na danej pozycji
        }
        else {
            pointer_to_values=pointer_to_values+vector->element_size; //przesuwamy sie, dalej naszym wskaznikiem na wartosc
            counter++; //osobna zmienna mamy odpowiedzialna wlasnie ze inkrementacje
        }
    }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) { //zakladam ze, jesli predykat jest spelniony to po prostu wartosc otrzymana bedzie dodatnia -> nie jestem pewny czy to jest dobrze
    void *pointer_to_values=vector->data; //wskaznik, pomagajacy nam dotrzec do konkrentych wartosci naszego wektora
    int range=(int)vector->size;
    int counter=0;
    for(int i=0;i<range;i++){ //chcemy przejrzec wszystkie elementy, naszego wektora
        if(predicate(pointer_to_values)==1){ //jesli, wartosc zwrocna przez predykat bedzie dodatnia znaczy to tyle, ze jest on speliony
            erase(vector,counter); //poniewaz udalo mi sie znalezc szukana wartosc, bede usuwal dany element na danej pozycji
        }
        else {
            pointer_to_values=pointer_to_values+vector->element_size; //przesuwamy sie, dalej naszym wskaznikiem na wartosc
            counter++; //osobna zmienna mamy odpowiedzialna wlasnie ze inkrementacje
        }
    }
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) { //funkcja ma usunac, niepotrzebnie zaalokowana pamiec
    if(vector->capacity>vector->size){ //znaczy to tyle, ze musimy usunac nie uzywana pamiec
        void *pointer_to_data=vector->data; //wskaznik, potrzebny mi jako, pomoc okreslania gdzie jest konkretnie przechowywana pamiec
        void *new_memory=malloc(vector->element_size*vector->size); //alokuje pamiec mieszaca akurat tyle danych ile bede potrzebowal
        memcpy(new_memory,pointer_to_data,vector->element_size*vector->size);
        vector->data=new_memory;
        vector->capacity=vector->size; //musimy przypisac zmienic aktualnie dostepna pamiec
        free(pointer_to_data); //usuwam tego czego nie potrzebuje
    }

}

// integer comparator
int int_cmp(const void *v1, const void *v2) {
    int num_1=*(int*)v1;
    int num_2=*(int*)v2;
    return num_1-num_2; //na odwort, bo potrzebuje posortowac malejaco
}

// char comparator
int char_cmp(const void *v1, const void *v2) { //musi zostac przyjety porzadek leksykograficzny
    char char_1=*(char*)v1;
    char char_2=*(char*)v2;
    int diff=(int)char_1-(int)char_2;
    return diff;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
    const Person *pointer_human_1=p1;
    const Person *pointer_human_2=p2;
    char *name_1=pointer_human_1->first_name;
    char *name_2=pointer_human_2->first_name;
    char *surname_1=pointer_human_1->last_name;
    char *surname_2=pointer_human_2->last_name;
    int age_1=pointer_human_1->age;
    int age_2=pointer_human_2->age;
    int diff_age=age_2-age_1;
    if(diff_age!=0) return diff_age;
    else{ //lata sa sobie, rowne a wiec bedziemy porownywac po imieniu i nazwisku rosnaco
        int diff_name=strcmp(name_1,name_2);
        if(diff_name!=0) return diff_name;
        else{
            int diff_surnames=strcmp(surname_1,surname_2);
            return diff_surnames;
        }
    }
}

// predicate: check if number is even
int is_even(void *value) { //sprawdzamy czy dana liczba jest parzysta
    int number=*(int*)value;
    return (number%2==0) ? 1 : -1;
}

// predicate: check if char is a vowel
int is_vowel(void *value) { //funkcja sprawdza czy dana litera jest samogloska
    char vowels[]={'a','e','i','o','u','y','A','E','I','O','U','Y'};
    char charakter=*(char*)value;
    for(int i=0;i<12;i++){ //przechodze po
        if(charakter==vowels[i]) return 1;
        }
    return -1;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person) {
    Person *pointer_to_human=person;
    if(pointer_to_human->age>25) return 1; //osoba ma wiecej niz 25 lat
    return -1;
}

// print integer value
void print_int(const void *v) {
    int number=*(int*)v;
    printf("%d ",number);
}

// print char value
void print_char(const void *v) {
    char charakter=*(char*)v;
    printf("%c ",charakter);
}

// print structure Person
void print_person(const void *v) {
    const Person *pointer_to_human=v;
    printf("%d ",pointer_to_human->age); //najpiew wyswietlam wiek, uzytkownika
    char *print_name=pointer_to_human->first_name; //wskaznik, na imie danej osoby
    for(int i=0;i<MAX_STR_LEN;i++){ //bedziemy przechodzic po wszystkich mozliwych elementach aby moc wyswietlic wlasnie tamten
        if(*print_name=='\0') break; //jest to nasz warunek konca
        printf("%c",*print_name);
        print_name+=sizeof(char); //przesuwamy sie odpowiednio dalej wskanzikiem

    }
    printf(" ");
    char *print_surname=pointer_to_human->last_name;
    for(int i=0;i<MAX_STR_LEN;i++){ //bedziemy przechodzic po wszystkich mozliwych elementach aby moc wyswietlic wlasnie tamten
        if(*print_surname=='\0') break;
        printf("%c",*print_surname);
        print_surname+=sizeof(char);
    }
    //printf(" ");
    printf("\n");

}

// print capacity of the vector and its elements
void print_vector(Vector *vector, print_ptr print) {
    printf("%d\n",(int)vector->capacity);
    void *pointer_to_data=vector->data;
    for(int i=0;i<(int)vector->size;i++){
        print(pointer_to_data); //przekazuje do funkcji print odpowiednio, to co bede chcial wyswielic
        pointer_to_data=pointer_to_data+vector->element_size; //przesuwam sie dalej wskaznikiem
    }
}

// read int value
void read_int(void* value) {
    int number;
    scanf(" %d",&number);
    *(int*)value=number;
}

// read char value
void read_char(void* value) {
    char character;
    scanf(" %c",&character);
    *(char*)value=character;
    //void *pointer_to_char=&character;
    //memmove(value,pointer_to_char,sizeof(char)); -> OPCJONALNIE
}

// read struct Person
void read_person(void* value) {
    Person *pointer=value; //pomocniczy wskaznik, pozwalajacy okreslic typ, danych
    char name[65]; //tak naprawde alokujemy pamiec, na ta tablice
    char surname[65];
    int age;
    scanf("%d %64s %64s",&age,name,surname);
    pointer->age=age;
    memmove(pointer->first_name,name,sizeof(char)*64); //bardzo wazny krok, aby skopiowac nasza dana pamiec
    memmove(pointer->last_name,surname,sizeof(char)*64);

}

void *safe_malloc(size_t elements){ //funkcja zwracajaca nieznany wskaznik
    void *result=malloc(elements);
    if(result==NULL) exit(0); //znaczy to, ze informujemy o bledzie poprzez zabrankniecie pamieci na wywolanie funkcji malloc
    return result; //zwracamy wskaznik na przydzielona nasza pamiec

}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
		 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
	init_vector(vector, block_size, elem_size);
	void *v = safe_malloc(vector->element_size);
	size_t index, size;
	for (int i = 0; i < n; ++i) {
		char op;
		scanf(" %c", &op);
		switch (op) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%zu", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%zu", &index);
				erase(vector, index);
				break;
			case 'v': // erase
				read(v);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
				is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
				is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person,
				person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}