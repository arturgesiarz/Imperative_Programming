#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list node
typedef struct Node {
    int *data; //tablica liczb calkowitych
    size_t array_size; //liczba elementow w tej tablicy
    struct Node* next;  //nastepny node
    struct Node* prev; //poprzedni node
} Node;

// doubly linked list
typedef struct List { //zszywamym glowne Node, dodajac do nich head oraz tail
    Node *head;
    Node *tail;
} List;

// iterator
typedef struct iterator { //struktura bedzie mi konkretne opisywala, po ilu elementach liczac od 1, bedzie nastepny node i to on dostanie ta wartosc
    struct Node* node_ptr;
    size_t position;
} iterator;

// forward initialization
iterator begin(Node* head) { //zaimplemetnowe w celu funkcji skip/get-forward gdzie pierwszym elemnetem faktycznie jest pierwszy node
    iterator it = { head, 0 };
    return it;
}

// backward initialization;
// points to the element following the last one
iterator end(Node* tail) { //zaimplementowe w celu funkcji skip/get-backward gdzie pierwszym elementem jest osttatni node, i ostatnia komorka -> a my tutaj po prostu przypisujemy pierwszemu elementowi wartoscc liczona od konca
    iterator it = { tail, tail->array_size };
    return it;
}

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) return ptr;
    exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
    void *ptr = realloc(old_ptr, size);
    if (ptr) return ptr;
    free(old_ptr);
    exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t array_size, Node *next, Node *prev) {
    Node *node = safe_malloc(sizeof(Node));
    node->data = data;
    node->array_size = array_size;
    node->next = next;
    node->prev = prev;
    return node;
}

// initialize list
// creates the front and back sentinels
void init(List *list) {
    list->head = create_node(NULL, 0, NULL, NULL);
    list->tail = create_node(NULL, 0, NULL, list->head);
    list->head->next = list->tail;
}

// to implement ...

// append node to the list
void push_back(List *list, int *data, size_t array_size) {
    Node *new_node=create_node(data,array_size,NULL,list->tail);
    list->tail->next=new_node;
    list->tail=list->tail->next;
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, size_t n) { //funkcja zwracajaca iterator od konkretnego elementu do innego
    int counter=0;
    Node *node_pointer=itr->node_ptr; //tworze sobie wskaznik na element od ktorego zaczynam
    for(Node *node = node_pointer; node != NULL; node = node->next) {
        for (int k = 0; k < node->array_size; k++) {
            counter++;
            if(counter==n){ //znaczy, ze juz wystarczajaco sie przesunalem
                itr->node_ptr=node;
                itr->position=k+1;
            }
        }
    }
}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n) {
    iterator it=begin(list->head);
    skip_forward(&it,n);
    int result=it.node_ptr->data[it.position-1];
    return result;

}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, size_t n) {
    int counter=0;
    Node *node_pointer=itr->node_ptr; //tworze sobie wskaznik na element od ktorego zaczynam -> wazne ze jest to node skierowany na TAIL
    for(Node *node = node_pointer; node != NULL; node = node->prev) {
        for (int k = (int)node->array_size-1; k >= 0; k--) {
            counter++;
            if(counter==n){ //znaczy, ze juz wystarczajaco sie przesunalem
                itr->node_ptr=node;
                itr->position=k+1;
            }
        }
    }
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n) {
    iterator it=end(list->tail);
    skip_backward(&it,n);
    int result=it.node_ptr->data[it.position-1];
    return result;

}

void remove_node(Node *node_ptr) {
    Node *node_to_delete=node_ptr;
    if(node_ptr->next==NULL && node_ptr->prev==NULL){ //ostatni element listy
        free(node_to_delete);
    }
    else if(node_ptr->next==NULL){ //znaczy ze next jest nullem, ale prev juz nie
        Node *node_prev=node_ptr->prev;
        node_prev->next=NULL;
        free(node_to_delete);
    }
    else if(node_ptr->prev==NULL){ //znaczy, ze jest to head, ale node_ptr.next nie jest nullem
        Node *node_next=node_ptr->next;
        node_next->prev=NULL;
        free(node_to_delete);
    }
    else{
        Node *node_next=node_ptr->next;
        Node *node_prev=node_ptr->prev;
        node_prev->next=node_next; //nastepny element, poprzedniego wezla bedzie nastepnym elementem glownego wezla
        node_next->prev=node_prev; //poprzednim elementem nastepnego node, bedzie poprzedni glownego
        free(node_to_delete);
    }
}

// remove n-th element; if array empty remove node
//DOROBIC KIEDY COS KIEDY MAM USUNAC OSTATNI ELEMNT!!!
void remove_at(List *list, size_t n) { //bede potrzebowal dodatkowo wyznaczyc, gdzie konkretnie bedzie sie znajdowala ta komorka
    iterator it = begin(list->head); //bedzie mi to sluzylo do wykrycia danego elementu
    skip_forward(&it,n); //otrzymam odpowiedni element, oraz noda ktorego musze usunac ale bedzie to numer odpowiednio liczony od 1
    if(it.node_ptr->array_size==1){ //znaczy to po prostu ze mamy usunac calego noda
        remove_node(it.node_ptr);
    }
    else{
        int *new_data = malloc(sizeof(int) * (it.node_ptr->array_size-1)); //alokuje pamiec, potrzebna mi do zapisania nowej talicy o liczbe elemetnow pomniejszonych o 1}
        int *old_mem=it.node_ptr->data;
        memmove(new_data,it.node_ptr->data,sizeof(int)*(it.position-1));
        memmove(&new_data[it.position-1],&it.node_ptr->data[it.position],sizeof(int)*(it.node_ptr->array_size-it.position));
        Node *node_next=it.node_ptr->next;
        Node *node_prev=it.node_ptr->prev;
        it.node_ptr->array_size--; //zmiejszamy ilosc elementow tablicy
        if(node_next!=NULL && node_prev!=NULL){
            it.node_ptr->data=new_data;
            node_next->prev->data=new_data;
            node_prev->next->data=new_data;
        }
        else if(node_next==NULL){
            it.node_ptr->data=new_data;
            node_prev->next->data=new_data;
        }
        else{
            it.node_ptr->data=new_data;
            node_next->prev->data=new_data;
        }
        free(old_mem);
    }
}
int abs(int x){
    if(x<0) return x*(-1);
    else return x;
}
// return the number of digits of number n
size_t digits(int n) {
    if(n==0) return 1;
    n=abs(n);
    int digits=0;
    while(n>0){
        digits++;
        n/=10;
    }
    return digits;
}
int cmp_int(const void* value1, const void* value2){ //funkcja pomocniczna do uzycja quicksort
    int v1=*(int*)value1;
    int v2=*(int*)value2;
    return v1-v2;
}

void sort_dll(List *list){ //funkcja sortujaca liste dwukierunkowa po dlugosci, poczatkowej liczb
    int amount_node1;
    int amount_node2=(int)digits(*(int*)list->tail->data);
    for(Node *node1 = list->head->next->next; node1 != NULL; node1 = node1->next) {
        amount_node1=(int)digits(*(int*)node1->data);
        if(cmp_int(&amount_node1,&amount_node2)>0){ //musi nastapic zamiana
            Node *node1_prev=node1->prev;
            Node *node1_next=node1->next;
            Node *node2=list->tail;
            if(node1_prev!=NULL && node1_next!=NULL){
                list->tail=node2->prev;
                list->tail->next=NULL;
                node1_prev->next=node2;
                node2->next=node1;
                node2->prev=node1_prev;
                node1->prev=node2;

            }
            else{ //bede wstawial na sam poczatek mojej listy dwukierunowej
                list->tail=node2->prev;
                list->tail->next=NULL;
                node1->prev=node2;
                node2->prev=NULL;
                node2->next=node1;
                list->head=node2;
            }
            break;
        }

    }

}

// inserts 'value' to the node with the same digits' count
// otherwise insert new node
void dumpList(const List *list);
void put_in_order(List *list, int value, int cmp_function(const void*,const void*)) {
    int token=0;
    for(Node *node = list->head->next->next; node != NULL; node = node->next) {
        if(digits(*node->data)==digits(value)){ //znaczy ze istenieje
            int *new_data = malloc(sizeof(int) * (node->array_size+1)); //alokuje pamiec, potrzebna mi do zapisania nowej talicy o liczbe elemetnow pomniejszonych o 1}
            int *old_mem=node->data;
            memmove(new_data,old_mem,sizeof(int)*(node->array_size));
            new_data[node->array_size]=value; //przypisuje moja nowa wartosc
            qsort(new_data,node->array_size+1,sizeof(int),cmp_function); //aby posortowac rosnaco
            Node *node_next=node->next;
            Node *node_prev=node->prev;
            node->array_size++; //zmiejszamy ilosc elementow tablicy
            if(node_next!=NULL && node_prev!=NULL){
                node->data=new_data;
                node_next->prev->data=new_data;
                node_prev->next->data=new_data;
            }
            else if(node_next==NULL){
                node->data=new_data;
                node_prev->next->data=new_data;
            }
            else{
                node->data=new_data;
                node_next->prev->data=new_data;
            }
            free(old_mem);
            token=1;
            break;
        }
    }
    if(token==0){ //przypadek, kiedy nie isteja takie node, ktory ma taka ilosc cyfr wiec go sobie stowrzymy ale w odpowiedniej kolejnosci
        int *new_data=malloc(sizeof(int)); //alokuje pamiec, na jeden element bo i tak, potem go bede powiekszal a niewiem ile bede mial elementow
        Node *new_node= create_node(new_data,1,NULL,list->tail);
        *new_data=value;
        list->tail->next=new_node;
        list->tail=new_node;
        sort_dll(list); //wywoluja funkcje sortujaca, w razie dodania nowego noda

    }
    
}


// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
    for(Node *node = list->head->next->next; node != NULL; node = node->next) {
        printf("-> ");
        for (int k = 0; k < node->array_size; k++) {
            printf("%d ", node->data[k]);
        }
        printf("\n");
    }
}

// free list
void freeList(List *list) {
    Node *to_delete = list->head->next, *next;
    while(to_delete != list->tail) {
        next = to_delete->next;
        remove_node(to_delete);
        to_delete = next;
    }
}

// read int vector
void read_vector(int tab[], size_t n) {
    for (size_t i = 0; i < n; ++i) {
        scanf("%d", tab + i);
    }
}

// initialize the list and push data
void read_list(List *list) {
    int n;
    size_t size;
    scanf("%d", &n); // number of nodes
    for (int i = 0; i < n; i++) {
        scanf("%zu", &size); // length of the array in i-th node
        int *tab = (int*) safe_malloc(size * sizeof(int));
        read_vector(tab, size);
        push_back(list, tab, size);
    }
}

int main() {
    int to_do, value;
    size_t size, m;
    List list;
    init(&list);

    scanf("%d", &to_do);
    switch (to_do) {
        case 1:
            read_list(&list);
            dumpList(&list);
            break;
        case 2:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                printf("%d ", get_forward(&list, m));
            }
            printf("\n");
            break;
        case 3:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                printf("%d ", get_backward(&list, m));
            }
            printf("\n");
            break;
        case 4:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                remove_at(&list, m);
            }
            dumpList(&list);
            break;
        case 5:
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%d", &value);
                put_in_order(&list, value,cmp_int);
            }
            dumpList(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    freeList(&list);

    return 0;
}