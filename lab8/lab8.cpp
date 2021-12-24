#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <x86intrin.h>
#include "lab8.hpp"
#define SIZE 256 // 1 kB
#define MAX_SIZE 256 * 1024 * 32 // 32 mB
#define STEP 1.1
#define REPEAT 4

unsigned long long test_memory_access_and_get_tick(int size, int* arr) {
	float** Imat = create_I(100);
	float** Zmat = create_zero_matrix(100);
	mul(100, Imat, Zmat);       

	int max = size * REPEAT;
	int k, i;
	for (i = 0, k = 0; i < size; i++) k = arr[k];  // разогрев
	
    if (k == -1 || Zmat[0][0] == -1) printf("foo");  

	unsigned long long start = __rdtsc();
	for (i = 0, k = 0; i < max; i++) k = arr[k];
	unsigned long long end = __rdtsc();
	
	if (k == -1 || Zmat[0][0] == -1) printf("foo");

	return end - start;
}

void fill_array_direct(int* arr, int size) {
	for(int i = 0; i < size; i++){
		arr[i] = i == size - 1 ? 0 : i + 1; 
	}
}

void fill_array_backward(int* arr, int size) {
	for(int i = 0; i < size; i++){
		arr[i] = i == 0 ? size - 1 : i - 1; 
	}
}

void fill_array_random(int* arr, int size) {
	memset(arr, 0, size * sizeof(int));
	int max = size;
	int count = size - 1;
	int count_of_filled = 1;
	srand(time(NULL));
	int zero_coord = rand() % max;
	for (int i = 0, cur = 0; i < count; i++) {
		if (count_of_filled == count) {  // если последний элемент, то зацикливаем его на нулевую координату
			arr[cur] = zero_coord;
			continue;
		}
		int buf = (rand() + 1) % max;
		if (buf != zero_coord && arr[buf] == 0 && buf != cur) {  // мы должны указать не на себя, не на нулевую координату, на неиспользуемое ранее число
			arr[cur] = buf;
			cur = buf;
		} 
		else {  // если не попали, то вручную ищем.
			for (; buf == zero_coord || arr[buf] != 0 || buf == cur || buf == 0;) {
				buf = (buf + 1) % max;
		}
		arr[cur] = buf;
		cur = buf;
		}
		count_of_filled++;
	}
}

void print_array(int* arr, int size) {
	for (size_t i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void test_direct(int size) {
	FILE* out = fopen("out_direct.csv", "w");
	for (double cur_size = size; cur_size <= MAX_SIZE; cur_size *= STEP) {

		int cur_size_int = (int)floor((double)cur_size); // округлим 
		int* arr = (int*)malloc(sizeof(int) * cur_size_int);
		fill_array_direct(arr, cur_size_int);

		unsigned long long time = test_memory_access_and_get_tick(cur_size_int, arr);

		fprintf(out, "%d %.02lf\n", cur_size_int, (double)time / (cur_size_int * REPEAT));
		free(arr);
	}
	fclose(out);
}

void test_random(int size) {
	FILE* out = fopen("out_random.csv", "w");
	for (double cur_size = size; cur_size <= MAX_SIZE; cur_size *= STEP) {
		
        int cur_size_int = (int)floor((double)cur_size);
		int* arr = (int*)malloc(sizeof(int) * cur_size_int);
		fill_array_random(arr, cur_size_int);
		
        unsigned long long time = test_memory_access_and_get_tick(cur_size_int, arr);
		
        fprintf(out, "%d %.02lf\n", cur_size_int, (double)time / (cur_size_int * REPEAT));
		free(arr);
	}
	fclose(out);
}

void test_backward(int size) {
	FILE* out = fopen("out_backward.csv", "w");
	for (double cur_size = size; cur_size <= MAX_SIZE; cur_size *= STEP) {

		int cur_size_int = (int)floor((double)cur_size);
		int* arr = (int*)malloc(sizeof(int) * cur_size_int);
		
		fill_array_backward(arr, cur_size_int);
		
		unsigned long long time = test_memory_access_and_get_tick(cur_size_int, arr);

		fprintf(out, "%d %.02lf\n", cur_size_int, (double)time / (cur_size_int * REPEAT));
		
		free(arr);
	}
	fclose(out);
}

int main() {
	printf("direct\n");
  	test_direct(SIZE);
	printf("back\n");	  
  	test_backward(SIZE);
	printf("random\n");	  
  	test_random(SIZE);
}