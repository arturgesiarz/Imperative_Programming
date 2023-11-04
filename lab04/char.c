#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) { //musze zalozyc, ze
    int counter_chars=0;
    int counter_words=1; //nie zliczam ostatniego wyrazu
    int counter_newlines=0;
    char x,last_char,first_char;
    x=getchar();
    first_char=x;
    last_char=x;
    while (x!=EOF){ //pobieram znak dopoki nie napotkam znaku konca
        counter_chars++; //zliczam kazdy mozliwy znak jaki wystapil
        if ((last_char=='\t' || last_char==' ' ||  last_char=='\n') && x!='\t' && x!=' ' && x!='\n') counter_words++;
        if (x=='\n') counter_newlines++;
        last_char=x;
        x=getchar();
    }
    if(first_char==EOF){
        *nl=0;
        *nc=0;
        *nw=0;
    }
    else{
        *nl=counter_newlines;
        *nc=counter_chars;
        *nw=counter_words;
    }

}

void char_count(int char_no, int *n_char, int *cnt) {
    int chars[95]; //tablica wszystkich mozliwych znakow, w moim dostepnym przedziale
    int result=0;
    for(int i=0;i<95;i++){
        chars[i]=0; //zerujemy wszystkie elementy
    }
    char x;
    x=getchar();
    while (x!=EOF){
        chars[x-33]++; //zliczam wystapienia danych znakow
        x=getchar();
    }
    int counter=1;
    int max=0;
    int max_id=0;
    while(counter+1<=char_no){ //zadaniem tej petli jest pozbycie sie char-no-1 najwiekszych elemntow
        for(int i=0;i<95;i++){
            if(chars[i]>max){
                max=chars[i];
                max_id=i;
            }
        }
        chars[max_id]=0; //pozbywam sie elementow, ktore sa w tamtej kolejnosci wieksze
        max=0;
        counter++;
    }
    for(int i=0;i<95;i++){ //szukam aktualnie najwiekszej wartosic, po usuneciu tamtych
        if(chars[i]>max){
            max=chars[i];
            max_id=i;
        }
    }
    *cnt=max;
    *n_char=max_id+33;

}

void bigram_count(int bigram_no, int bigram[]) { //bigram[0],bigram[1] - oznaczaja znaleziony bigram, ale za to bigram[2]-oznacza krotksc jego wystapienia; bigram_no-liczba od ktorej mamy go szukac

    int bigrams[95][95];
    for(int i=0;i<95;i++){
        for(int j=0;j<95;j++){
            bigrams[i][j]=0;
        }
    }
    char x,last_char;
    x=getchar();
    last_char=x; //dorobic zabezpiecznie na pierwszy element
    while (x!=EOF){
        if(last_char!=' ' && last_char!='\t' && last_char!='\n' && x!=' ' && x!='\t' && x!='\n'){ //znaczy to ze elementy te moga byc digramami
            bigrams[last_char-33][x-33]++;
        }
        last_char=x;
        x=getchar();
    }
    int counter=1;
    int max=0;
    int max_id_i,max_id_j=0;
    while(counter+1<=bigram_no){
        for(int i=0;i<95;i++){
            for(int j=0;j<95;j++){
                if(bigrams[i][j]>max) {
                    max = bigrams[i][j];
                    max_id_i = i;
                    max_id_j=j;
                }
            }
        }
        bigrams[max_id_i][max_id_j]=0; //pozbywam sie elementow, ktore sa w tamtej kolejnosci wieksze
        max=0;
        counter++;
    }
    for(int i=0;i<95;i++){ //szukam aktualnie najwiekszej wartosic, po usuneciu tamtych
        for(int j=0;j<95;j++){
            if(bigrams[i][j]>max) {
                max = bigrams[i][j];
                max_id_i = i;
                max_id_j=j;
            }
        }
    }
    bigram[2]=max;
    bigram[0]=max_id_i+33;
    bigram[1]=max_id_j+33;

}

void find_comments(int *line_comment_counter, int *block_comment_counter) { //moze, byc problem z zaczeciem tekstu do //
    char x,last_char;
    x=getchar();
    last_char=x;
    int start=1; //funkcja sluzaca do wylaczenia pewnych funkcji kiedy, to jest za wczesnnie
    int counter_bc=0; //counter block comment
    int counter_in=0; //counter inline comment
    int activate_bc=0; //zmienna sluzaca, do wiedzy na temat kiedy sie nam zaczal nasz komentarz
    int activate_in=0;
    while (x!=EOF){
        if(last_char=='/' && x=='*' && activate_bc==0 && activate_in==0 && start==0){ //komenatrz sie nam zaczal wielopoziomowy
            activate_bc=1;
            counter_bc++;
            last_char=x;
            x=getchar();
            start=1;
            continue;
        }
        if(last_char=='*' && x=='/' && activate_bc==1 && activate_in==0 && start==0){
            activate_bc=0; //ponieaz komentarz sie nam skonczyl
            last_char=x;
            x=getchar();
            start=1;
            continue;
        }
        if(last_char=='/' && x=='/' && activate_bc==0 && activate_in==0 && start==0){
            activate_in=1;
            counter_in++;
            last_char=x;
            x=getchar();
            start=1;
            continue;
        }
        if(last_char=='\n' && activate_bc==0 && activate_in==1 && start==0){
            activate_in=0;

        }
        last_char=x;
        x=getchar();
        start=0;
    }
    *line_comment_counter=counter_in;
    *block_comment_counter=counter_bc;
}

#define MAX_LINE 128

int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int bigram[3];

	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}