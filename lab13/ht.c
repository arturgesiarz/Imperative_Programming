#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  (-1)

typedef union { //jest to struktura danych, ktora przydziela pamiec, danym elementa w sposob ciagly, czyli bedziemy musili wiedziec jaki to jest konkrenty element uzywany
	int int_data;
	char char_data;
	// ... other primitive types used
	void *ptr_data;
} data_union;

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef int (*CompareDataFp)(data_union, data_union);
typedef size_t (*HashFp)(data_union, size_t);
typedef data_union (*CreateDataFp)(void*);

typedef struct { //jest to lisa z wartownikiem, tak wiec pierwszym elementem jest zawsze NULL
	size_t size; //szerokosc elementu
	size_t no_elements; //ilosc elemntow
	ht_element *ht; //tablica, w ktorej sa glowy linkedlist!!!
	DataFp dump_data;
	CreateDataFp create_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data,
		 DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) { //size, moze mi oznaczac ilosc kluczy, pod jakim bede przechowywal moje dane
    ht_element *pointer_to_tab=malloc(sizeof(ht_element)*size); //alokuje, nowa pamiec ktora bedzie mi symbolizowac tablice wszystkich nodow, z ktorych bedzie odporowdazal osobne linkedlisty
    p_table->size=size;
    p_table->no_elements=0;
    p_table->ht=pointer_to_tab;
    p_table->dump_data=dump_data;
    p_table->create_data=create_data;
    p_table->free_data=free_data;
    p_table->compare_data=compare_data;
    p_table->hash_function=hash_function;
    p_table->modify_data=modify_data;
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, size_t n) {
    ht_element *pointer_to_tab=p_table->ht[n].next; //wskazuje, na dany element tablicy ktory bedzie chcial wyswietlic -> bo lista z wartownikiem
    while(pointer_to_tab!=NULL){
        p_table->dump_data(pointer_to_tab->data); //wyswietlam, dany element poprzez funkcje dump_data
        pointer_to_tab=pointer_to_tab->next;
    }
}

// Free element pointed by data_union using free_data() function -> znaczy ze jest to caly wezel mojej tablicy, ktora przechwouje
void free_element(DataFp free_data, ht_element *to_delete) {
    ht_element  *pointer_to_delete=to_delete;
    while(pointer_to_delete!=NULL){
        free_data(pointer_to_delete->data); //usuwam jakby caly wezel
        pointer_to_delete=pointer_to_delete->next;
    }

}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
    ht_element *pointer_to_head=p_table->ht;
    int n=(int)p_table->size;
    for(int i=0;i<n;i++){
        free_element(p_table->free_data,pointer_to_head[i].next); //usuwam, poszczegolne cale wezly -> ale ide od nastpeneog elemenetu, ponieaz jest to lista z wartownikiem
    }

}

// calculate hash function for integer k
size_t hash_base(int k, size_t size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (size_t)floor((double)size * (tmp - floor(tmp)));
}

void insert_element_but_in_the_new_table(ht_element *table, data_union *data,size_t hash) { //dodajemy zawsze element z przodu
    ht_element *pointer_guardian=&table[hash];
    ht_element *pointer_to_frist_el=pointer_guardian->next;
    ht_element *new_element=malloc(sizeof(ht_element));
    new_element->data=*data;
    if(pointer_to_frist_el==NULL) new_element->next=NULL;
    else new_element->next=pointer_to_frist_el;
    pointer_guardian->next=new_element;
}

void rehash(hash_table *p_table) { //oczywiscie musze dodawac nowe element na sam poczatek nowej tablicy
    ht_element *pointer_to_new_tab=malloc(sizeof(ht_element)*p_table->size*2); //alokuje, nowa dwa razy wieksza tablice bedzie mi przechowywala glowy list
    ht_element *head_tab=p_table->ht;
    ht_element *pointer_to_elements;
    size_t new_hash;
    int heads=(int)p_table->size;
    for(int i=0;i<heads;i++){ //przegladam wszystkie glowy moich tablic
        pointer_to_elements=head_tab[i].next; //ponieaz pierwszy element jest guardianem
        while(pointer_to_elements!=NULL){ //zwiedzam kazdy wezel, aby przepisac go
            new_hash=p_table->hash_function(pointer_to_elements->data,p_table->size*2); //obliczam, nowe haszowanie mojej funkcji przez podwojenie danej ilosci mojego hashu
            insert_element_but_in_the_new_table(pointer_to_new_tab,&pointer_to_elements->data,new_hash);
            pointer_to_elements=pointer_to_elements->next; //przesuwam sie dalej moim wskaznikiem
        }
    }
    //free_table(p_table); //funkcja usuwa zalokowana pamiec na tablice hash
    p_table->ht=pointer_to_new_tab;
    p_table->size*=2;
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data) {
    size_t hash_position=p_table->hash_function(data,p_table->size); //najpiew obliczam, pod jakim indeksem powininen sie znalezc nasz dany element
    ht_element *pointer_to_tab=p_table->ht[hash_position].next; //bo na pierwszej pozycji, zawsze stoji wartownik
    if(p_table->compare_data(pointer_to_tab->data,data)==0) return &p_table->ht[hash_position];  //jesli, okaze sie ze rowne sa juz pierwsze elementy to musimy zwrocic wskaznik na wartownika, zwracam adres, poczatku mojej listy, bo tam jest element jakby przed
    while(pointer_to_tab!=NULL){
        if(pointer_to_tab->next==NULL) break; //znaczy, to ze nie udalo sie znalezc tego elementu
        if(p_table->compare_data(pointer_to_tab->next->data,data)==0)  return pointer_to_tab; //znaczy ze udalo mi sie znalezc ostatni element, wiec zwroce teraz jego adres
        pointer_to_tab=pointer_to_tab->next; //przesuwam sie dalej, w celu znalezenia mojej wartosci
    }
    return NULL; //znaczy, ze nie udalo mi sie znalezc szukanego elementu
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
    size_t hash_position=p_table->hash_function(*data,p_table->size);
    ht_element *pointer_to_tab=p_table->ht[hash_position].next; //ponieaz pierwszym elementem tablicy jest wartownik
    while(pointer_to_tab!=NULL){
        if(p_table->compare_data(pointer_to_tab->data,*data)==0) return pointer_to_tab; //zwracam wskaznie na ten dany element
        pointer_to_tab=pointer_to_tab->next;
    }
    return NULL; //znaczy, ze nie udalo sie znalezc naszego szukanego punktu
}

// insert element
void insert_element(hash_table *p_table, data_union *data) { //dodajemy zawsze element z przodu -> musimy najpiew sprawdzic czy dany element juz przyapdkeim nie istnieje
    if(get_element(p_table,data)==NULL){ //znaczy, ze nasz element sie nie powtorzy
        size_t hash_position=p_table->hash_function(*data,p_table->size); //pozysukuje, indeks haszowanej tablicy
        ht_element *pointer_guardian=&p_table->ht[hash_position];
        ht_element *pointer_to_frist_el=pointer_guardian->next;
        ht_element *new_element=malloc(sizeof(ht_element));
        new_element->data=*data;
        new_element->next=pointer_to_frist_el;
        pointer_guardian->next=new_element;
        p_table->no_elements++;
        if(p_table->no_elements/p_table->size>MAX_RATE){
            rehash(p_table);
        }
    }

}

// remove element
void remove_element(hash_table *p_table, data_union data) {
    ht_element *pointer_to_act_el=get_element(p_table,&data);
    if(pointer_to_act_el!=NULL){ //zabezpiecznie, przed checia usuniecia, elemntu ktorego nie mamy
        ht_element *pointer_to_prev_el=find_previous(p_table,data);
        ht_element *pointer_to_next_el=pointer_to_act_el->next;
        if(pointer_to_next_el==NULL) pointer_to_prev_el->next=NULL;
        else pointer_to_prev_el->next=pointer_to_next_el;
        p_table->no_elements--;
    }
}

// type-specific definitions

// int element

size_t hash_int(data_union data, size_t size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ",data.int_data);
}

int cmp_int(data_union a, data_union b) {
    int value1=a.int_data;
    int value2=b.int_data;
    return value1-value2;
}

data_union create_int(void* value) {
    int a;
    scanf("%d",&a); //wczytuje, dana liczbe z klawiatury
    if(value==NULL){ //oznacza, to ze musimy stworzyc nowa data_union w oparciu o liczbe calkowita ktora wczytamy
        data_union *new_union=malloc(sizeof(data_union)); //alokuje, pamiec na nowy element
        new_union->int_data=a; //wczytuje, dana liczbe do uni typu int
        return *new_union; //zwracam nowa, unie
    }
    else{ //znaczy, to ze do isteniacej uni mam wpisac dana wartosc
        data_union *exist_union=(data_union*)value;
        exist_union->int_data=a;
        return *exist_union; //zwracam, adres juz istenijacej uni w, ktorej modyfikowalem dane
    }
}

// char element

size_t hash_char(data_union data, size_t size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
    char value1=data.char_data;
    printf("%c ",value1);
}

int cmp_char(data_union a, data_union b) {
    char value1=a.char_data;
    char value2=b.char_data;
    return (int)value1-(int)value2;
}

data_union create_char(void* value) {
    char x;
    scanf(" %c",&x); //wczytuje, znak
    if(value==NULL){ //znaczy, ze ma zostac stworzony nowy, znak
        data_union *new_union=malloc(sizeof(data_union));
        new_union->char_data=x;
        return *new_union;
    }
    else{ //znaczy, ze do istejacej uni mam przypisac ta wlasnie wartosc
        data_union *exist_union=(data_union*)value;
        exist_union->char_data=x;
        return *exist_union;
    }
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data) {
    DataWord *pointer=(DataWord*)data.ptr_data;
    char *pointer_to_print=pointer->word; //wskaznik, sluzacy do wydrukowania danego slowa
    while(*pointer_to_print!='\0'){
        printf("%c",*pointer_to_print);
        pointer_to_print=pointer_to_print+sizeof(char);
    }
    printf(" %d",pointer->counter);
}

void free_word(data_union data) {
    DataWord *pointer=(DataWord*)data.ptr_data;
    free(pointer);
}

int cmp_word(data_union a, data_union b) {
    char *word1=((DataWord*)a.ptr_data)->word;
    char *word2=((DataWord*)b.ptr_data)->word;
    int diff=strcmp(word1,word2);
    return diff;
}

size_t hash_word(data_union data, size_t size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

void modify_word(data_union *data) { //funkcja, ma na celu tylko zwiekszyc wartosc licznika
    DataWord *pointer=(DataWord*)data->ptr_data;
    pointer->counter++;
}

data_union create_data_word(void *value) {
    data_union *new_union=malloc(sizeof(data_union)); //tworze pamiec, na utworzenie nowego elementu
    DataWord *new_data_word=malloc(sizeof(DataWord));
    char *word=(char*)value; //pod wskaznikiem trzymamy dane slowo
    new_data_word->counter=1;
    new_data_word->word=word;
    new_union->ptr_data=new_data_word;
    return *new_union;
}
void *modify_word_to_upper(void *p) { //funckja alokujaca pamiec, oraz zmieniajaca dany wyraz na taki ktory bedzie mial wszystkie litery male -> *p - wskaznik na wyraz
    char *new_word=(char*)p; //kopiujemy z alokacja pamieci danego stringa -> alokacja pamieci
    char *pointer_to_changes=new_word;
    int to_convert;
    while(*pointer_to_changes!='\0'){
        to_convert=(int)*pointer_to_changes; //otrzymujemy dany kod ascii
        if(to_convert>=65 && to_convert<=90){ //znaczy ze, mamy do czynienia z wielkimi literami
            to_convert+=32;
            *pointer_to_changes=(char)to_convert;
        }
        pointer_to_changes=pointer_to_changes+sizeof(char);
    }
    return new_word; //zwracam, wskaznik na stringa ktory zostal pomniejszony
}

// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
    char *pointer_to_line;
    char *help_pointer; //bedzie nam sluzyl do wyciagania po prostu kolejnych elemntow
    char *result;
    char *upper_one; //wskaznik, ktory bedzie sluzyl do zmniejszenia wielkosci danego slowa
    for(;;){
        pointer_to_line=malloc(BUFFER_SIZE*sizeof(char)); //rezerwuje odpowiednio pamiec
        result=fgets(pointer_to_line,BUFFER_SIZE,stream); //potrzebuje wczytac BUFFER_SIZE, znakow wiec nie mnoze tego dodatkowo razy sizeof(char)
        if(result==NULL) break; //koniec, wczytywania
        help_pointer=strtok(pointer_to_line," \n\t\r\v\f.,?!:;-"); //jedynym separatorem jest spacja
        while(help_pointer!=NULL){
            upper_one=modify_word_to_upper(help_pointer); //zmieniam wielkosc, mojego slowa na male litery
            data_union new_word=create_data_word(upper_one); //tworze, sobie strukture data_union
            ht_element *find_same_el=get_element(p_table,&new_word); //bede sprawdzal czy aby napewno nie istenieje ten sam element
            if(find_same_el!=NULL){ //znaczy, ze dany element juz istenieje a wiec nie bede go dodawal tylko, zwieksze jego licznik
                modify_word(&find_same_el->data); //
            }
            else{ //znaczy ze, ten element niewystepuje tak wiec stworze
                insert_element(p_table,&new_word);
            }
            help_pointer=strtok(NULL," \n\t\r\v\f.,?!:;-"); //wyodrebniam osobno dane slowa i bede tworzyl nowe elementy
        }

    }

}

// test primitive type list
void test_ht(hash_table *p_table, int n) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
//		p_table->create_data(&data);
		data = p_table->create_data(NULL); // should give the same result as the line above
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	size_t index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 2: // test char hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 3: // read words from text, insert into hash table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%zu\n", table.size);
			data = table.create_data(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	//free_table(&table);

	return 0;
}