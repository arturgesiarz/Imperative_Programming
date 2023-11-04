#include <stdio.h>

#define STACK_SIZE 10

enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };

int stack[STACK_SIZE];
int top = 0;

int stack_push(int x) {

    if(top<=9){
        stack[top]=x;
        top++; //zwiekszamy szczyt naszego stosu
        return OK;
    }
    else{
        return OVERFLOW;
    }
}

int stack_pop(void) {
    int save=0;
    int index=0;
    if(top!=0){
        save=stack[top-1];
        stack[top-1]=0;
        top--;
        return save; //zwracamy usuniety element
    }
    if(top==0){
        return UNDERFLOW;
    }
}
int stack_state(void) {
    if(top!=0){
        return top; //zwracmy dlugosc wiec nie mamy top-1
    }
    else{
        return 0;
    }
}

// FIFO queue with shifts

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0;

int queue_push(int in_nr) { //dodaje in_nr klientow do kolejki

    int i=0;
    i=curr_nr+1; //poniewaz bedziemy dodawac do aktualnego stanu

    int counter=0;
    counter=in_nr;

    curr_nr+=in_nr;

    while (counter>0){
        if(in<=9) {
            queue[in] = i;
            in++;
            i++;
        }
        else{ //musimy wykonac ta czesc, warunku, poniewaz chcemy wyjsc wczesniej jezeli kolejka nie bedzie w calosci zapelniona
            break;
        }
        counter--;
    }

    if(counter>0){
        return OVERFLOW;
    }
    if(counter==0){
        return OK;

    }
}
// 1 2 3 4 5
int queue_pop(int out_nr) {
    int temp=0;
    if(out_nr>in){
        for(int i=0;i<in;i++){
            queue[i]=0;
        }
        in=0;
        return UNDERFLOW;
    }
    else {
        for (int j = 1; j <= out_nr; j++) {
            for (int i = 1; i < in; i++) {
                queue[i - 1] = queue[i]; //usuwamy pierwszy elemnt z kolejki
            }
            queue[in-1]=0;
            in--;
        }
        return in;
    }
}

int queue_state(void) {
    return in;
}

void queue_print(void) {
    for(int i=0;i<in;i++){
        printf("%d ",queue[i]);
    }
}

// Queue with cyclic buffer

#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out = 0, len = 0;


int cbuff_push(int cli_nr) {

    if(len>=10){
        return OVERFLOW;
    }
    else{
        cbuff[len]=cli_nr;
        len++;
        return OK;
    }
}

int cbuff_pop(void) {
    int client=cbuff[out]; //zapisuje numer klienta
    if(len==0){
        return UNDERFLOW;
    }
    else{
        for(int i=1;i<len;i++){
            cbuff[i-1]=cbuff[i]; //przesuwam, abym mogl sie pozbyc numeru klienta
        }
        out=0; //wracam na najdluzej czekjacego klienta
        len--; //poniewaz pozbylismy sie jednego elementu
        return client;
    }
}

int cbuff_state(void) {
    return len;
}

void cbuff_print(void) {
    for(int i=0; i<len; i++){
        printf("%d ",cbuff[i]);
    }
}

int main(void) {
    int to_do, n, client_no, answer;
    scanf("%d", &to_do);
    switch(to_do) {
        case 1: // stack
            do {
                scanf("%d", &n);
                if (n > 0) {
                    if ((answer = stack_push(n)) < 0) printf("%d ", answer);
                } else if (n < 0) {
                    printf("%d ", stack_pop());
                } else printf("\n%d\n", stack_state());
            } while(n != 0);
            break;
        case 2: // FIFO queue with shifts
            do {
                scanf("%d", &n);
                if (n > 0) {
                    if ((answer = queue_push(n)) < 0) printf("%d ", answer);
                } else if (n < 0) {
                    if ((answer = queue_pop(-n)) < 0) printf("%d ", answer);
                } else {
                    printf("\n%d\n", queue_state());
                    queue_print();
                }
            } while(n != 0);
            break;
        case 3: // queue with cyclic buffer
            client_no = 0;
            do {
                scanf("%d", &n);
                if (n > 0) {
                    if ((answer = cbuff_push(++client_no)) < 0) printf("%d ", answer);
                } else if (n < 0) {
                    printf("%d ", cbuff_pop());
                } else {
                    printf("\n%d\n", cbuff_state());
                    cbuff_print();
                }
            } while(n != 0);
            break;
        default:
            printf("NOTHING TO DO!\n");
    }
    return 0;
}