#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int rand_from_interval(int a, int b) { //funkcja losujaca liczbe z zakresu <a,b>
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
void swap (int array[], int i, int k) { //funkcja zamieniajaca komorki w tablicy
    int temp=0;
    temp=array[i];
    array[i]=array[k];
    array[k]=temp;
}
void rand_permutation(int n, int array[]) { //funkcja permutujaca tablice
    int k;
    if (n>=0){
        for(int i=0;i<=n-1;i++){
            array[i]=i;
        }
        for(int i=0;i<=n-2;i++){
            k=rand_from_interval(i,n-1);
            swap(array,i,k);
        }
    }
}

#define PACK_A 52
#define PACK_B 52
#define CARD_SIZE 52

int cards[CARD_SIZE];
int playerA[PACK_A];
int playerB[PACK_B];
int statusA=26;
int statusB=26;
int conflicts=0;

void create_cards(void){

    for(int i=0;i<CARD_SIZE;i++) cards[i]=i;

    rand_permutation(CARD_SIZE,cards);

    for(int i=0;i<CARD_SIZE/2;i++) playerA[i]=cards[i];
    for(int i=CARD_SIZE/2;i<CARD_SIZE;i++) playerB[i-CARD_SIZE/2]=cards[i];
    for(int i=0;i<CARD_SIZE;i++){ //petla ma na celu wypelnic miejsca ktore, nie sa wykorzystywane wartoscia -1
        if(i>CARD_SIZE/2) {
            playerA[i]=-1;
            playerB[i]=-1;
        }

    }

}
void add_card(int force, char player){ //funkcja dodajaca, na sam koniec talii, wygrane karty
    if(player=='A') {
        playerA[statusA]=force;
        statusA++;
    }
    if(player=='B') {
        playerB[statusB]=force;
        statusB++;
    }
}
void delete_card(char player){
    if(player=='A'){
        if(statusA>1){
            for(int i=1;i<statusA;i++){
                playerA[i-1]=playerA[i];
            }
            playerA[statusA-1]=-1;
        }
        else if(statusA==1){
            playerA[0]=-1;
        }
        statusA--;
    }
    if(player=='B'){
        if(statusB>1){
            for(int i=1;i<statusB;i++){
                playerB[i-1]=playerB[i];
            }
            playerB[statusB-1]=-1;
        }
        else if(statusB==1){
            playerB[0]=-1;
        }
        statusB--;
    }
}
void show_card(char player){
    if(player=='A'){
        for(int i=0;i<statusA;i++){
            printf("%d ",playerA[i]);
        }
    }
    if(player=='B'){
        for(int i=0;i<statusB;i++){
            printf("%d ",playerB[i]);
        }
    }

}
int value_card(int value){ //funkcja odczytujaca wartosc rzeczywista z id karty
    if(value>=0 && value<=3)        return 2;
    else if(value>=4 && value<=7)   return 3;
    else if(value>=8 && value<=11)  return 4;
    else if(value>=12 && value<=15) return 5;
    else if(value>=16 && value<=19) return 6;
    else if(value>=20 && value<=23) return 7;
    else if(value>=24 && value<=27) return 8;
    else if(value>=28 && value<=31) return 9;
    else if(value>=32 && value<=35) return 10;
    else if(value>=36 && value<=39) return 11; //jopek
    else if(value>=40 && value<=43) return 12; //dama
    else if(value>=44 && value<=47) return 13; //krol
    else if(value>=48 && value<=51) return 14; //as
}

int fight_1(int max_conflicts) { //tryb-0

    int temp = 0; //zmienne temp oraz save sa zmiennymi pomocniczymi
    int save;

    int pointerA = 0; //zmienna, wskazujaca na aktualnie wystawiana karte
    int pointerB = 0;
    int idA=0;
    int idB=0;

    int forceA; //zmienna, mowiaca o wartosci rzeczywistej wystawionej karty
    int force_fightA;
    int forceB;
    int force_fightB;

    while (conflicts <= max_conflicts && playerA[pointerA]!=-1 && playerB[pointerA]!=-1) {
        pointerA=0;
        pointerB=0;
        idA=playerA[pointerA];
        idB=playerB[pointerB];
        forceA = value_card(playerA[pointerA]);
        forceB = value_card(playerB[pointerB]);

        if (forceA > forceB) { //zgarnia karty, grasz A
            delete_card('A'); //wyciagniecie karty
            delete_card('B');
            add_card(idA, 'A');
            add_card(idB, 'A');
            conflicts++;
        } else if (forceB > forceA) {
            delete_card('A');
            delete_card('B');
            add_card(idB, 'B');
            add_card(idA, 'B');
            conflicts++;
        }
        else{ //wojna
            pointerA+=2;
            pointerB+=2;
            conflicts++;
            while (conflicts <= max_conflicts && playerA[pointerA]!=-1 && playerB[pointerA]!=-1) {
                conflicts++;
                force_fightA = value_card(playerA[pointerA]);
                force_fightB = value_card(playerB[pointerB]);
                if (force_fightA > force_fightB) { //znaczy ze wojna sie wkoncu skonczyala
                    while(temp<=pointerA){ // 0 1 2
                        save=playerA[0];
                        delete_card('A');
                        add_card(save,'A');
                        temp++;
                    }
                    temp=0;
                    while(temp<=pointerB){
                        save=playerB[0];
                        delete_card('B');
                        add_card(save,'A');
                        temp++;
                    }
                    temp=0;
                    break; //wychodze poniiewaz zakonczylem wojne
                } else if (force_fightB > force_fightA) {
                    while(temp<=pointerB){
                        save=playerB[0];
                        delete_card('B');
                        add_card(save,'B');
                        temp++;
                    }
                    temp=0;
                    while(temp<=pointerA){
                        save=playerA[0];
                        delete_card('A');
                        add_card(save,'B');
                        temp++;
                    }
                    temp=0;
                    break; //wychodze poniiewaz zakonczylem wojne
                } else {
                    pointerA += 2;
                    pointerB += 2;
                }
            }
            if(playerA[pointerA]==-1 || playerB[pointerA]==-1) {
                return 1; //zabralko nam kart, aby rozstrzynac wojne
            }
            else if(conflicts > max_conflicts){
                return 0; //przekroczylismy limit konfliktow
            }

        }
        if (statusA == 52) return 2;
        if (statusB == 52) return 3;
    }
    pointerA=0;
    pointerB=0;
    if(conflicts>max_conflicts) return 0;
    if(playerA[pointerA]==-1 || playerB[pointerA]==-1) return 1;
    if (statusA == 52) return 2;
    if (statusB == 52) return 3;


}
int fight_2(int max_conflicts) { //tryb 1-uproszczony

    int pointerA = 0;
    int pointerB = 0;
    int forceA;
    int forceB;
    int idA=0; //zmienna sluzaca do zapisu id karty, nie chcemy dodawac do kolejki wartosci naszej konkretnej karty
    int idB=0;

    while (conflicts <= max_conflicts && playerA[pointerA]!=-1 && playerB[pointerA]!=-1) {

        conflicts++;
        idA=playerA[pointerA];
        forceA = value_card(playerA[pointerA]);
        idB=playerB[pointerB];
        forceB = value_card(playerB[pointerB]);

        if (forceA > forceB) { //zgarnia karty, grasz A
            delete_card('A'); //wyciagniecie karty
            delete_card('B');
            add_card(idA, 'A');
            add_card(idB, 'A');

        } else if (forceB > forceA) {
            delete_card('A');
            delete_card('B');
            add_card(idB, 'B');
            add_card(idA, 'B');
        } else { //przesuwamy podspod nasze karty
            delete_card('A');
            add_card(idA, 'A');
            delete_card('B');
            add_card(idB, 'B');
        }

        if (statusB == 0) return 2; //bo max tablica
        if (statusA == 0) return 3;


    }
    if (statusB == 0) return 2;
    else if(statusA == 0) return 3;
    else if(conflicts > max_conflicts) return 0;
    else return 1;

}
void ending(int option){
    if(option==0){ //zabralko kart jednemu z graczy lub obydwom
        printf("%d",0);
        printf(" %d",statusA);
        printf(" %d",statusB);
    }
    if(option==1){ //nierozstrzygniecie wojny10444 0 100
        printf("%d",1);
        printf(" %d",statusA);
        printf(" %d",statusB);

    }
    if(option==2){ //wygrana gracza A
        printf("%d",2);
        printf(" %d",conflicts);
    }
    if(option==3){ //wygrana gracza B
        printf("%d",3);
        printf("\n");
        show_card('B');
    }
}

int main(void){

    int version_game,seed,max_conflicts,result;
    scanf("%d %d %d",&seed,&version_game,&max_conflicts);
    srand((unsigned) seed); //nastepuje tutaj losowanie
    create_cards();

    switch(version_game){

        case 0: //wersja stadardowa
            result= fight_1(max_conflicts);
            ending(result);
            break;

        case 1: //wersja uproszczona
            result= fight_2(max_conflicts);
            ending(result);
            break;

        default:
            printf("WRONG CHOICE!\n");
            break;
    }

    return 0;
}