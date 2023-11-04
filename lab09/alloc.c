#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    return A[row * cols + col];
}

void set(int cols, int row, int col, int *A, int value) {
    A[row * cols + col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    double sum;
    for(int resultRow = 0; resultRow < rowsA; resultRow++) {
        for(int resultCol = 0; resultCol < colsB; resultCol++) {
            sum = 0;
            for(int idx = 0; idx < colsA; idx++) {
                sum += get(colsA, resultRow, idx, A) * get(colsB, idx, resultCol, B);
            }
            set(colsB, resultRow, resultCol, AB, sum);
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            scanf("%d", &t[i * cols + j]);
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols - 1; j++) {
            printf("%d ", get(cols, i, j, t));
        }
        printf("%d\n", get(cols, i, cols - 1, t));
    }
}

int read_char_lines(char *array[]) {
    int char_lines = 0;
    while(1) {
        char *line = malloc(BUF_SIZE * sizeof(char));
        if(fgets(line, BUF_SIZE, stdin) == NULL) {
            break;
        }

        array[char_lines++] = line;
    }

    return char_lines;
}

void write_char_line(char *array[], int n) {
    char *line_ptr = array[n];
    while(*line_ptr != 0) {
        printf("%c", *line_ptr);
        line_ptr++;
    }
}

void delete_lines(char *array[], int n) {
    for(int i = 0; i < n; i++) {
        free(array[i]);
    }
}

int read_int_lines_cont(int *ptr_array[]) {
    int num_lines = 0;
    char line[BUF_SIZE];

    while(fgets(line, BUF_SIZE, stdin) != NULL) {
        int num_count = 0;
        int *nums = malloc(BUF_SIZE * sizeof(int));

        char* token = strtok(line, " ");
        while(token != NULL) {
            nums[num_count++] = atoi(token);
            token = strtok(NULL, " ");
        }
        if(num_count < BUF_SIZE - 1) {
            /* To differentiate NULL = 0 from 0 as value,
             * now condition to end printing would be
             * ... != INT_MIN */
            nums[num_count] = INT_MIN;
        }

        ptr_array[num_lines++] = nums;
    }

    return num_lines;
}

void write_int_line_cont(int *ptr_array[], int n) {
    int *line_ptr = ptr_array[n];
    int idx = 0;
    while(idx < BUF_SIZE && *(line_ptr + idx) != INT_MIN){
        printf("%d ", *(line_ptr + idx++));
    }
    printf("\n");
}

typedef struct {
    int *values;
    int len;
    double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    int num_lines = 0;
    char line[BUF_SIZE];

    while(fgets(line, BUF_SIZE, stdin) != NULL) {
        lines_array[num_lines].len = 0;
        lines_array[num_lines].average = 0;
        lines_array[num_lines].values = (int*)malloc(BUF_SIZE * sizeof(int));

        char* token = strtok(line, " ");
        while(token != NULL) {
            lines_array[num_lines].values[lines_array[num_lines].len++] = atoi(token);
            token = strtok(NULL, " ");
        }
        for(int i = 0; i < lines_array[num_lines].len; i++) {
            lines_array[num_lines].average += lines_array[num_lines].values[i];
        }
        lines_array[num_lines].average  /= lines_array[num_lines].len;
        num_lines++;
    }

    return num_lines;
}

void write_int_line(line_type lines_array[], int n) {
    for(int i = 0; i < lines_array[n].len; i++) {
        printf("%d ", lines_array[n].values[i]);
    }
    printf("\n%.2f\n", lines_array[n].average);
}

void delete_int_lines(line_type array[], int line_count) {
    for(int i = 0; i < line_count; i++) {
        free(array[i].values);
    }
}

int cmp (const void *a, const void *b) {
    line_type *aa = (line_type*)a;
    line_type *bb = (line_type*)b;
    return aa->average > bb->average ? 1 :
        aa->average == bb->average ? 0 : -1;
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array, line_count, sizeof(line_type), cmp);
}

typedef struct {
    int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    for(int i = 0; i < n_triplets; i++) {
        scanf("%d %d %d", &triplet_array[i].r, &triplet_array[i].c, &triplet_array[i].v);
    }

    return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet *tt1 = (triplet*)t1;
    triplet *tt2 = (triplet*)t2;

    int rows = tt1->r - tt2->r;
    int cols = tt1->c - tt2->c;

    return rows != 0 ? rows : cols;
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {

    qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);

    for(int i = 0; i < n_triplets; i++) {
        C[i] = triplet_array[i].c;
        V[i] = triplet_array[i].v;
    }

    R[0] = 0;
    int sum = 0;
    for(int i = 1; i <= rows; i++) {
        for(int j = 0; j < n_triplets; j++) {
            if(triplet_array[j].r == i - 1) {
                sum++;
            }
        }
        R[i] = sum;
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    for(int i = 0; i < rows; i++) {
        y[i] = 0;
        for(int j = R[i]; j < R[i + 1]; j++) {
            y[i] += V[j] * x[C[j]];
        }
    }
}

void read_vector(int *v, int n) {
    for(int i = 0; i < n; i++) {
        scanf("%d", &v[i]);
    }
}

void write_vector(int *v, int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

int read_int() {
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
    int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    int rows, cols, n_triplets;
    char *char_lines_array[TAB_SIZE] = { NULL };
    int continuous_array[TAB_SIZE];
    int *ptr_array[TAB_SIZE];
    triplet triplet_array[TAB_SIZE];
    int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
    int x[TAB_SIZE], y[TAB_SIZE];
    line_type int_lines_array[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d", &rowsA, &colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d", &rowsB, &colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA, colsA, colsB, A, B, AB);
            print_mat(rowsA, colsB, AB);
            break;
        case 2:
            n = read_int() - 1; // we count from 1 :)
            ptr_array[0] = continuous_array;
            read_int_lines_cont(ptr_array);
            write_int_line_cont(ptr_array, n);
            break;
        case 3:
            n = read_int() - 1;
            read_char_lines(char_lines_array);
            write_char_line(char_lines_array, n);
            delete_lines(char_lines_array, n);
            break;
        case 4:
            n = read_int() - 1;
            lines_counter = read_int_lines(int_lines_array);
            sort_by_average(int_lines_array, lines_counter);
            write_int_line(int_lines_array, n);
            delete_int_lines(int_lines_array, lines_counter);
            break;
        case 5:
            scanf("%d %d %d", &rows, &cols, &n_triplets);
            n_triplets = read_sparse(triplet_array, n_triplets);
            read_vector(x, cols);
            make_CSR(triplet_array, n_triplets, rows, V, C, R);
            multiply_by_vector(rows, V, C, R, x, y);
            write_vector(V, n_triplets);
            write_vector(C, n_triplets);
            write_vector(R, rows + 1);
            write_vector(y, rows);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}