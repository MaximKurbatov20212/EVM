#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <x86intrin.h>
#include "lab9.hpp"

#define OFFSET 65536 * 2
#define MAX_FRAGMENTS 32
#define REPEAT 100000

unsigned long long test_memory_access_and_get_ticks(int* arr, int max_fragments) {
    float** Imat = create_I(100);
    float** Zmat = create_zero_matrix(100);
    mul(100, Imat, Zmat);

    int max = max_fragments * REPEAT;
    int k, i;
    for (i = 0, k = 0; i < max_fragments; i++) k = arr[k];  // разогрев

    if (k == -1 || Zmat[0][0] == -1) printf("foo");
    
    unsigned long long start = __rdtsc();
    for (i = 0, k = 0; i < max; i++) k = arr[k];
    unsigned long long end = __rdtsc();
    
    if (k == -1 || Zmat[0][0] == -1) printf("foo");

    return end - start;
}

void fill_array(int* arr, int offset_in_integers, int max_fragments) {
    for (int i = 1; i <= max_fragments; i++) {
      arr[(i - 1) * offset_in_integers] = offset_in_integers * (i);
    }
    arr[(max_fragments - 1) * offset_in_integers] = 0;
}

void TestCache(int offset_in_integers, int max_fragments) {
    FILE* out = fopen("out_cache.csv", "w");
    long long offset = offset_in_integers * max_fragments;
    
    int* arr = (int*)malloc(sizeof(int) * offset);
    memset(arr, 0, sizeof(int) * offset);
    
    for (int cur_fragments = 1; cur_fragments <= max_fragments; cur_fragments++) {
      fill_array(arr, offset_in_integers, cur_fragments);

      unsigned long long time = test_memory_access_and_get_ticks(arr, cur_fragments);
      
      fprintf(out, "%d %.02lf\n", cur_fragments, (double)time / (cur_fragments * REPEAT));
    }
    free(arr);
    fclose(out);
}

int main() {  
    TestCache(OFFSET, MAX_FRAGMENTS);
}