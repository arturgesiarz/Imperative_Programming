#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
    struct ListElement *next;
    void *data;
} ListElement;

typedef struct {
    ListElement *head;
    ListElement *tail;
    ConstDataFp dump_data; //wydrukuj dane elementu listy
    DataFp free_data; //zwolnij pamiec, elementu listy -> specjalna funkcja, ponieaz umozliwa to nam korzystanie z uniwersalnych struktur danych
    CompareDataFp compare_data; //porownaj dwa elementy, wedlug konkretnego kryterium
    DataFp modify_data; //zmodyfikuj, dane elementu listy (tu: zwieksz licznik)
} List;

typedef struct DataWord { //kazde, slowo bedzie mialo swoj inkdes ile razy sie tak naprawde pojawilo juz
    char *word;
    int counter;
} DataWord;

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if(ptr) return ptr;
    printf("malloc error\n");
    exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
    void *ptr = strdup(string);
    if(ptr) return ptr;
    printf("strdup error\n");
    exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
               CompareDataFp compare_data, DataFp modify_data) { //inicjalizacja listy jednokierunkowej
    p_list->compare_data=compare_data;
    p_list->free_data=free_data;
    p_list->dump_data=dump_data;
    p_list->free_data=free_data;
    p_list->modify_data=modify_data;
    p_list->head=NULL;
    p_list->tail=NULL;
}

// Print elements of the list
void dump_list(const List* p_list) { //warto dodac ze bedziemy wypisywac tylko i wylacznie liczby calkowite, poniewaz nie przekazujemy nic innego co mowi nam o typie wyswietlanym
    ListElement *pointer_to_element=p_list->head; //bede wypisywal w ten sposob, kolejne wartosci naszej listy jednokierkowej
    while(pointer_to_element!=NULL){
        p_list->dump_data(pointer_to_element->data);
        pointer_to_element=pointer_to_element->next; //przesuwam sie na dalszy
    }
    printf("\n");
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) { //data -> DataWord
    ListElement *pointer_to_element=p_list->head;
    DataWord *data_el1;
    DataWord *data_el2=(DataWord*)data;
    int number2=data_el2->counter;
    int number1;
    while(pointer_to_element!=NULL){
        data_el1=(DataWord*)pointer_to_element->data;
        number1=data_el1->counter;
        if(number1==number2) p_list->dump_data(pointer_to_element->data);
        pointer_to_element=pointer_to_element->next;
    }
    printf(" ");
}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete) { //zakladam, ze w funkcji free_data, zostanie usuniety odpowiendio wskaznik, aby nie zaburzyc dzialania naszej listy
    free_data(to_delete->data); //usuwam dane miejsce w pamieci wskazywane przez data
}

// Free all elements of the list
void free_list(List* p_list) {
    p_list->head=NULL; //pamiec, tutaj nie jest usuwana tylko po prostu jest gubiony wskaznik na konkretne nowe elementy
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    if(p_list->head==NULL){ //znaczy to ze lista jest pusta i musze w tym wypadku postapic inaczej
        ListElement *data_save=malloc(sizeof(ListElement));
        data_save->data=data;
        data_save->next=NULL;
        p_list->head=data_save;
        p_list->tail=data_save;

    }
    else{
        ListElement *data_save=malloc(sizeof(ListElement));
        data_save->data=data;
        data_save->next=p_list->head;
        p_list->head=data_save;
    }

}

// Push element at the end of the list
/*
    scanf("%d", &v);
    push_back(p_list, create_data_int(v));
 */
void push_back(List *p_list, void *data) {
    if(p_list->tail==NULL || p_list->head==NULL){ //znaczy ze nasza lista tak naprawde, nawet nie ma pierwszego elementu
        ListElement *data_save=malloc(sizeof(data)+sizeof(ListElement)); //odpowiednio, alokuje pamiec
        data_save->data=data;
        data_save->next=NULL;
        p_list->head=data_save;
        p_list->tail=data_save;
    }
    else{
        ListElement *data_save=malloc(sizeof(data)+sizeof(ListElement)); //odpowiednio, alokuje pamiec
        data_save->data=data;
        data_save->next=NULL;
        p_list->tail->next=data_save;
        p_list->tail=p_list->tail->next;
    }


}

// Remove the first element
void pop_front(List *p_list) {
    if(p_list->head!=NULL) p_list->head=p_list->head->next; //robimy spraawdzenie, czy wogole istnieje
}

// Reverse the list
void reverse(List *p_list) { //zakladam,ze lista jest conajmniej 3 elementowa
    ListElement *p1=p_list->head;
    if(p1!=NULL){
        ListElement *p2=p_list->head->next;
        if(p2!=NULL){
            ListElement *p3=p_list->head->next->next;
            if(p3!=NULL){
                p1->next=NULL;
                p_list->tail=p1;
                while(p3!=NULL){
                    p2->next=p1;
                    p1=p2;
                    p2=p3;
                    p3=p3->next;
                    if(p3==NULL) p2->next=p1;
                }
                p_list->head=p2;
            }
            else{ //znaczy ze, mamy w liscie tylko dwa elementy i je ze soba po prostu zamieniamy
                p1->next=NULL;
                p2->next=p1;
                p_list->head=p2;
                p_list->tail=p1;
            }
        }
    }

}

// find element in sorted list after which to insert given element -> znalezc element na posortowanie liscie, po ktorym chce wstawic moj dany element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) { // -1 oznacza ze cos bedzie przed czyms, a 1 oznacza ze cos musi byc po tym
    ListElement *pointer_first=p_list->head;
    ListElement *pointer_second=p_list->head->next;
    while(pointer_second!=NULL){ //sprawdzalbym czy, moge postawic element tuz przed pierwszym ale wtedy bym nie mial co zwrocic, tak wiec bedzie to srpawdzal jeszcze gdzie indzie
        if(p_list->compare_data(pointer_second->data,p_element->data)<0){ //oznacza to ze: pointer_first musi byc pierwszy -> wszystko jest w porzadku
            pointer_first=pointer_second;
            pointer_second=pointer_second->next;
        }
        else if(p_list->compare_data(pointer_second->data,p_element->data)>0){ //oznacza to ze: pointer_second musi byc pierwszy
            return pointer_first; //zwracam element, przed abym mogl potem latwo dodac moj element po prostu
        }
        else return NULL; //jesli, elmenet sa rowne, to nic nie robie i nie dodaje
        if(pointer_second==NULL){ //ma tutaj nastapic sprawdzenie elementu pointer_first
            if(p_list->compare_data(pointer_first->data,p_element->data)<0) return pointer_first; //znaczy to ze mozemy dodac element na koniec, a wiec zwroicmy jego adres
        }
    }
    return pointer_first; //znaczy, ze nie znalezino takiego miejsca gdzie wcisniemy nasz dany element, lub format liczby jest bledny
}

ListElement* find_equal_point(const List *p_list, ListElement *p_element) { // funkcja zwraca, wskaznik na rowny element, aby moc zwiekszyc jego potem licznik
    ListElement *pointer_first=p_list->head;

    while(pointer_first!=NULL){ //sprawdzalbym czy, moge postawic element tuz przed pierwszym ale wtedy bym nie mial co zwrocic, tak wiec bedzie to srpawdzal jeszcze gdzie indzie
        if(p_list->compare_data(pointer_first->data,p_element->data)==0){ //oznacza to ze: pointer_first musi byc pierwszy -> wszystko jest w porzadku
            return pointer_first; //zwracamy wskaznik na dany element ktory jest rowny z tym co chcemy go dodac
        }
        pointer_first=pointer_first->next;

    }
    return NULL; //znaczy, to ze nie ma elementow rownych sobie
}

ListElement* check_if_exist_intersion_point_at_start(List *p_list, ListElement *p_element){ //funkcja sprawdzajaca czy nasz p_element, moze stac przed wszystkim, i jesli tak to, dodaje ten element po porstu z przodu a jesli nie to zwraca NULL
    ListElement *pointer_first=p_list->head;
    if(pointer_first==NULL){
        p_list->head=p_element;
        p_list->tail=p_element;
        return p_list->head;
    }
    if(p_list->compare_data(pointer_first->data,p_element->data)>0){ //compare data, musi porownywac po objekcje DATA
        push_front(p_list,p_element->data);
        return p_list->head; //oddajemy miejsce, gdzie wlasnie postawilismy nasz nowy element
    }
    return NULL;
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous, ListElement *element_to_add) { //dodatkowo nastapi tutaj sprawdzenie czy, moge postawic za pierwszym elementem dany element
    if(p_list->tail==previous){ //oznacza to, ze dodaje na sam koniec nasz element
        p_list->tail->next=element_to_add;
        p_list->tail=element_to_add;
    }
    else{
        element_to_add->next=previous->next; //tworze polaczenie
        previous->next=element_to_add;
    }

}

// Insert element preserving order
void dump_word (const void *d);
void *insert_in_order(List *p_list, void *p_data) { //zwrocenie NULL, bedzie oznaczalo pomyslne dodanie elementu, a zwrocenie wskaznika na ten dany element bedzie oznaczalo, ze nie dualo sie dodac elementu o tym wlasnie adresie
    ListElement *element_to_add=malloc(sizeof(p_data)); //tak czy, siak bedziemy musieli dodac element do naszej listy, ewentulanie ta pamiec bedziemy zwalanaic
    ListElement *previous_element;
    element_to_add->data=p_data;
    element_to_add->next=NULL;
    ListElement *equal_element= find_equal_point(p_list,element_to_add);
    if(equal_element!=NULL) return equal_element;
    if(check_if_exist_intersion_point_at_start(p_list,element_to_add)==NULL){ //znaczy ze nie udalo sie dodac elementu na poczatek to wtedy uzywamy wczeniej juz napisanej funkcji
        previous_element=find_insertion_point(p_list,element_to_add);
        if(previous_element!=NULL) {
            push_after(p_list,p_data,previous_element,element_to_add);
            return NULL;
        }
        else free(element_to_add); //znaczy, to ze nie udalo sie tego dodac najprawpodobniej bo element byl taki sam a wiec mozemy zwolnic pamiec -> ale i tak go musialem wykorzystac do porownania elementow
    }
    return NULL;

}

// -----------------------------------------------------------
// --- type-specific definitions


// int element

void dump_int(const void *d) {
    int value=*(int*)d;
    printf("%d ",value);
}

void free_int(void *d) { //d->jest to wskaznik na inta
    free(d);
}

int cmp_int(const void *a, const void *b) {
    int value1=*(int*)a;
    int value2=*(int*)b;
    return value1-value2;
}

int *create_data_int(int v) {
    int *data_for_int= malloc(sizeof(int));
    *data_for_int=v; //zapisuje, w przydzilonej wczesniej pamieci wartosc, jaka dostaje
    return data_for_int; //na koniec zwracam adres do, tej komorki pamieci
}

void dump_word (const void *d) { //zakladam ze na wejsciu bede mial element DataWord
    DataWord *pointer=(DataWord*)d;
    char *pointer_to_print=pointer->word;
    //printf("%c",*pointer_to_print);
    while(*pointer_to_print!='\0'){
        printf("%c",*pointer_to_print); //wyswietlamy po jednym znaku
        pointer_to_print=pointer_to_print+sizeof(char);
    }
    printf(" "); //na czas testow bede wyswietlal ilosc, wystapien danych slow
}

void dump_word_lowercase(const void *d){ //zakladam ze na wejsciu bede mial element DataWord
    DataWord *pointer=(DataWord*)d;
    char *pointer_to_print=pointer->word;
    int to_convert; //otrzymam wartosc liczba litery -> kod asci
    char act_letter; //zmienna potrzebna tylko wtedy, kiedy bedziemy zmienaic litere z duzej na mala
    while(*pointer_to_print!='\0'){
        to_convert=(int)*pointer_to_print;
        if(to_convert>=97 && to_convert<=122){ //znacyz ze litera jest mala i po prostu ja taka wyswietlamy
            printf("%c",*pointer_to_print);
        }
        else{ //znaczy ze nalezy zamienic litere alfabetu na mniesjza -> bo jest aktualnie wieksza
            to_convert+=32; //sprawi to zmienienie naszej aktulanie litery
            act_letter=(char)to_convert;
            printf("%c",act_letter);
        }
        pointer_to_print=pointer_to_print+sizeof(char);
    }
    printf(" ");
}

void free_word(void *d){ //funkcja zwalnia pamiec, ktora wskazuje po prostu wskaznik na znak
    free(d);
}

int cmp_word_alphabet(const void *a, const void *b) { //funckaj porownuajca wyrazy po alfabecie
    char *word1=(char*)a;
    char *word2=(char*)b;
    int diff=strcmp(word1,word2);
    return diff;
}

int cmp_char_in_datatype(const void *a, const void *b) { //na wejsciu otrzymujemy dane z dataTYPE
    DataWord *pointer_word1=(DataWord*)a; //tworze wskaznik na odpowiednie dane
    DataWord *pointer_word2=(DataWord*)b;
    char *word1=pointer_word1->word;
    char *word2=pointer_word2->word;
    int diff=strcmp(word1,word2);
    return diff;
}

int cmp_word_counter(const void *a, const void *b) { //zakladam ze do funkcji, przekazuje jako a -> element DataWord oraz b-> rowniez jako DataWord
    DataWord *pointer_word1=(DataWord*)a; //tworze wskaznik na odpowiednie dane
    DataWord *pointer_word2=(DataWord*)b;
    int amount_word1=pointer_word1->counter;
    int amount_word2=pointer_word2->counter;
    int diff=amount_word1-amount_word2;
    return diff;

}

void modify_word(void *p) { //inaczej inkrementacja, danego wskaznika ktory jest do danej
    DataWord *word=(DataWord*)p;
    word->counter++; //inkrementujemy wystepowanie danego wyrazu
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
    return new_word;
}

void *create_data_word(char *string, int counter) { //jak tworze, data_world to zawsze jest counter ustawiany na zero
    DataWord *new_DataWord=malloc(sizeof(DataWord)); //alokujemy pamiec na ta zmienan bo bedzeimy ja po porstu tworzyc
    new_DataWord->word=string;
    new_DataWord->counter=counter;
    return new_DataWord; //na koniec zwracamy wskaznik do nowo zalokowanej pamieci
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
    char *pointer_to_line;
    char *help_pointer; //bedzie nam sluzyl do wyciagania po prostu kolejnych elemntow
    char *result;
    if(cmp==NULL){ //znaczy, to ze lista nie ma byc posortowana
        for(;;){
            pointer_to_line=malloc(BUFFER_SIZE*sizeof(char)); //rezerwuje odpowiednio pamiec
            result=fgets(pointer_to_line,BUFFER_SIZE,stream); //potrzebuje wczytac BUFFER_SIZE, znakow wiec nie mnoze tego dodatkowo razy sizeof(char)
            if(result==NULL) break; //koniec, wczytywania
            help_pointer=strtok(pointer_to_line," \n\t\r\v\f.,?!:;-"); //jedynym separatorem jest spacja
            while(help_pointer!=NULL){
                //push_back(p_list,strdup(help_pointer)); //strdup - kopiuje do najblizszego znaku termalnego
                DataWord *new_word=create_data_word(help_pointer,1); //tworzymy tylko i wylacznie strukture
                push_back(p_list,new_word);
                help_pointer=strtok(NULL," \n\t\r\v\f.,?!:;-"); //w NULL'u przekazujemy ten sam wyraz, tylko wskaznik idzie do samego konca, a noramlnie otrzymujemy wskaznik na samym poczatku
            }

        }
    }
    else{ //przypadek kiedy, musimy nasza liste wyswietlic w innych sposob
        char *change_word_to_upper; //wskaznik, sluzacy do zmian
        for(;;){ //petla do wczytywania danych
            pointer_to_line=malloc(BUFFER_SIZE*sizeof(char)); //rezerwuje odpowiednio pamiec
            result=fgets(pointer_to_line,BUFFER_SIZE,stream); //potrzebuje wczytac BUFFER_SIZE, znakow wiec nie mnoze tego dodatkowo razy sizeof(char)
            if(result==NULL) break; //koniec, wczytywania
            help_pointer=strtok(pointer_to_line," \n\t\r\v\f.,?!:;-"); //jedynym separatorem jest spacja
            while(help_pointer!=NULL){
                change_word_to_upper=modify_word_to_upper(help_pointer); //zapisujemy do naszego wskaznika
                DataWord *new_word=create_data_word(change_word_to_upper,1); //tworzymy tylko i wylacznie strukture
                //printf("DUMP:"); dump_word(new_word); printf("\n");
                ListElement *pointer=(ListElement*)insert_in_order(p_list,new_word); //bedzie to wskaznik albo na ten sam elemnet albo null -> czyli pomyslnie udalo sie dodac nowe slowo
                if(pointer!=NULL){ //znaczy ze nie udalo nam sie pomyslnie dodac elementu, bo taki owy juz sie znajduje -> w zamian otrzymalismy wskaznik na element ktory jest wlasnie rowny
                    DataWord *to_change_word=(DataWord*)pointer->data; //mam teraz wskaznik na miejsce w pamieci w, ktorym jest dokladnie ten elemnt ktorego szukalem
                    modify_word(to_change_word); //inkrementuje wystapowanie countera
                    //free(change_word_to_upper); //ponieaz, nie udalo sie nam dodac tego elementu, a wiec bede zwalnial nasza pamiec
                }

                help_pointer=strtok(NULL," \n\t\r\v\f.,?!:;-"); //w NULL'u przekazujemy ten sam wyraz, tylko wskaznik idzie do samego konca, a noramlnie otrzymujemy wskaznik na samym poczatku
            }

        }

    }

}

// test integer list
void list_test(List *p_list, int n) {
	char op;
	int v;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		switch (op) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int, cmp_int, NULL);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word, NULL, NULL);
			stream_to_list(&list, stdin, NULL);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word_lowercase, free_word, cmp_char_in_datatype, modify_word);
			stream_to_list(&list, stdin, cmp_word_alphabet);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
			dump_list_if(&list, &data);
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}