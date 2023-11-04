#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};


int find_idents(void) {

    int exsists[MAX_IDS];

    for(int i=0;i<MAX_IDS;i++){
        exsists[i]=0;
        for(int j=0;j<MAX_ID_LEN;j++){
            identifiers[i][j]=0;
        }
    }

    char x,last_char;
    int start=1; //funkcja sluzaca do wylaczenia pewnych funkcji kiedy, to jest za wczesnnie

    int activate_bc=0; //zmienna sluzaca, do wiedzy na temat kiedy sie nam zaczal nasz komentarz
    int activate_in=0;
    int activate_poet=0;

    int counting_words=0;

    int counting_letters=0;
    int act_length=0;
    int test;

    x=getchar();
    last_char=x;
    while (x!=EOF){ //trzeba dorobic sprawdzanie lancuchow samych ze soba
        if( (last_char=='"' || last_char=='\'') && activate_bc==0 && activate_in==0 && activate_poet==0 && start==0){
            activate_poet=1;
            last_char=x;
            x=getchar();
            start=1;
            continue;
        }
        if((last_char=='"' || last_char=='\'') && activate_bc==0 && activate_in==0 && activate_poet==1 && start==0){
            activate_poet=0;
            last_char=x;
            x=getchar();
            start=1;
            continue;
        }
        if(last_char=='/' && x=='*' && activate_bc==0 && activate_in==0 && activate_poet==0 &&  start==0){
            activate_bc=1;
            last_char=x;
            x=getchar();
            start=1;
            continue;
        }
        if(last_char=='*' && x=='/' && activate_bc==1 && activate_in==0 &&  activate_poet==0 && start==0){
            activate_bc=0;
            last_char=x;
            x=getchar();
            start=1;
            continue;
        }
        if(last_char=='/' && x=='/' && activate_bc==0 && activate_in==0 &&  activate_poet==0 && start==0){
            activate_in=1;
            last_char=x;
            x=getchar();
            start=1;
            continue;
        }
        if((last_char=='\n' || last_char=='\r' ) && activate_bc==0 && activate_in==1 && activate_poet==0 && start==0 ){
            activate_in=0;
        }
        if(activate_in==0 && activate_bc==0 && activate_poet==0 && start==0){ //znaczy to, ze nie mamy do czynienia z zadnym komentarzem i mozemy zliczac wystpienia naszych wyrazow

            if (( (last_char=='\t' || last_char==' ' ||  last_char=='\n' || last_char=='\r' || last_char==46 || last_char==35 || last_char==40 || last_char==41 || last_char==44 || last_char==60 || last_char==126 || last_char==91 || last_char==42) && ( (x>=65 && x<=90) || (x>=97 && x<=122) || x==95 ) && counting_letters==0 && counting_words<MAX_IDS ) ) { //mamy do czynienia z wyrazem, bo mnusi sie on zaczynac do litery rozrozniamy tutaj jej wielkosc, albo od podkreslenia
                identifiers[counting_words][counting_letters]=x;
                counting_letters++;
                act_length++;
            }
            else if (  ( (x>=65 && x<=90) || (x>=97 && x<=122) || x==95 || (x>=48 && x<=57) ) && ( (last_char>=65 && last_char<=90) || (last_char>=97 && last_char<=122) || last_char==95 || (last_char>=48 && last_char<=57) ) && counting_letters>0 && counting_words<MAX_IDS ){ //przypadek kiedy jestesmy w srodku naszego wyrazu
                identifiers[counting_words][counting_letters]=x;
                counting_letters++;
                act_length++;
            }
            else if( ( (last_char>=65 && last_char<=90) || (last_char>=97 && last_char<=122) || last_char==95 || (last_char>=48 && last_char<=57) ) && (x=='\t' || x==' ' ||  x=='\n' || x=='\r') && counting_letters>0  ){ //oznacza to koniec slowa
                identifiers[counting_words][counting_letters]='\0'; //dolaczamy null na koncu tak samo jak w stringu, aby compare mogl rozpoznac ze jest to koniec naszego lancucha
                test=1;
                for(int i=0;i<32;i++){
                    test=strcmp(identifiers[counting_words],keywords[i]); //porownuje ze soba stringi
                    if(test==0){ //musi nastapic usuniecie tego elementu
                        for(int m=0;m<MAX_ID_LEN;m++){
                            identifiers[counting_words][m]=0; //nastepuje usuniecie danego elementu
                        }
                        break; //znaczy to ze znalezlismy, takie same lancuchy znakow
                    }
                }
                counting_words++;
                counting_letters=0;
                act_length=0;
            }
            else if( ( (last_char>=65 && last_char<=90) || (last_char>=97 && last_char<=122) || last_char==95 || (last_char>=48 && last_char<=57) ) && ( x<=47 || (x>=58 && x<=64) || (x>=91 && x<=96) || (x>=123 && x<=127) ) ){ //zmienna moze sie tez konczyc innym znakiem np main() konczy sie () lub int x=12, to po rownasie pownnismy zakonczyc dzialanie
                identifiers[counting_words][counting_letters]='\0'; //dolaczamy null na koncu tak samo jak w stringu, aby compare mogl rozpoznac ze jest to koniec naszego lancucha
                test=1;
                for(int i=0;i<32;i++){
                    test=strcmp(identifiers[counting_words],keywords[i]); //porownuje ze soba stringi
                    if(test==0){ //musi nastapic usuniecie tego elementu
                        for(int m=0;m<MAX_ID_LEN;m++){
                            identifiers[counting_words][m]=0; //nastepuje usuniecie danego elementu
                        }
                        break; //znaczy to ze znalezlismy, takie same lancuchy znakow
                    }
                }
                counting_words++;
                counting_letters=0;
                act_length=0;
            }

        }
        last_char=x;
        x=getchar();
        start=0;
    }

    int template;
    for(int i=0;i<MAX_IDS;i++){
        for(int j=0;j<MAX_IDS;j++){
            if(i!=j){
                if((identifiers[i][0]>=65 && identifiers[i][0]<=90) || (identifiers[i][0]>=97 && identifiers[i][0]<=122) || identifiers[i][0]==95){
                    exsists[i]=1;
                }
                else{
                    exsists[i]=0;
                }

            }
        }
    }
    for(int i=0;i<MAX_IDS;i++){
        for(int j=0;j<MAX_IDS;j++){
            if(i!=j){
                template=strcmp(identifiers[i],identifiers[j]);
                if(template==0 && ((identifiers[i][0]>=65 && identifiers[i][0]<=90) || (identifiers[i][0]>=97 && identifiers[i][0]<=122) || identifiers[i][0]==95)){
                        exsists[i]++;
                }

            }
        }

    }

    int help=0;
    for(int i=0;i<MAX_IDS;i++){ //sortuje moja wynikowa tablice
        for(int j=0;j<MAX_IDS-1;j++){
            if(exsists[j]>exsists[j+1]){
                help=exsists[j];
                exsists[j]=exsists[j+1];
                exsists[j+1]=help;
            }
        }
    }

    int series=1;
    int probe=0;
    int sum=0;

    for(int i=0;i<MAX_IDS;i++){
        for(int j=0;j<MAX_IDS-1;j++){
            if(exsists[j]==1){
                sum++;
            }
            if(exsists[j]==exsists[j+1] && exsists[j]>1){
                probe=exsists[j];
                series++;
            }
            else if(series>1){ //znaczy to ze seria nasza sie skonczyla
                series=series/probe;
                sum+=series;
                series=1;
            }
        }
    }
    if(series>1){ //znaczy to ze seria nasza sie skonczyla
        series=series/probe;
        sum+=series;
    }

    return sum/1024;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
    printf("%d\n", find_idents());
	return 0;
}