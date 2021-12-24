#include <iostream>
#include <cblas.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void calc_I(float* I, int N) {

    for (int i = 0; i < N; i++) {
        I[i * (N + 1)] = 1;
    }
}

float sum_max_line(float* A, int N) {
    float* sum_arr = (float*)calloc(sizeof(float), N);
    for (int i = 0; i < N; i++) {
        sum_arr[i] = cblas_sasum(N, A + N * i, 1);//Сжимает матрицу до столбца, а в каждой строке столбца - сумма сжатой строки
        //N элементов в векторе, A + N * i указатель на вектор , каждый 1 элемент используется(шаг)
    }
    float max_sum = sum_arr[cblas_isamax(N, sum_arr, 1)];//Находит наибольшее значение в векторе
    free(sum_arr);
    return max_sum;
}

float sum_max_column(float* A, int N) {
    float* sum_arr = (float*)calloc(sizeof(float), N);
    for (int i = 0; i < N; i++) {
        sum_arr[i] = cblas_sasum(N, A + i, N);//Сжимает матрицу до строки, а в каждой компоненте строки - сумма сжатого столбца
    }
    float max_sum = sum_arr[cblas_isamax(N, sum_arr, 1)];
    free(sum_arr);
    return max_sum;
}

void swap(float** A, float** B) {
    float* tmp = *A;
    *A = *B;
    *B = tmp;
}

int main() {
    srand(124);
    int N; 
    int M ;
    std::cin >> N;
    std::cin >> M;      
    float* I = (float*)calloc(sizeof(float), N * N);
    float* B = (float*)calloc(sizeof(float), N * N);
    float* R = (float*)calloc(sizeof(float), N * N);
    float* A = (float*)calloc(sizeof(float), N * N);
    float* reverseA = (float*)calloc(sizeof(float), N * N);
    float* tmp1 = (float*)calloc(sizeof(float), N * N);
    float* tmp2 = (float*)calloc(sizeof(float), N * N);

    for (int i = 0; i < N * N; i++) { 
        A[i] = rand(); 
    }
   
    calc_I(I, N);
    float del = sum_max_line(A, N) * sum_max_column(A, N);
    cblas_sgemm(CblasRowMajor, CblasTrans, CblasNoTrans, N, N, N, 1 / del, A, N, I, N, 1.0f, B, N);  // B = AT / del
    //A*B*const1 + C * const2 записывается в С
    calc_I(R, N);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, -1.0f, B, N, A, N, 1.0f, R, N); //I - BA

    memcpy(tmp1, R, N * N * sizeof(float));
    for (int i = 0; i < M; i++) {
        cblas_saxpy(N * N, 1.0f, tmp1, 1, I, 1); // суммируем
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0f, tmp1, N, R, N, 0.0f, tmp2, N); //R^i   подходящая степень
        swap(&tmp1, &tmp2);
    }
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0f, I, N, B, N, 0.0f, reverseA, N); //_I * B = A^-1

    free(B);
    free(tmp1);
    free(tmp2);
    free(reverseA);
    free(I);
    free(R);
    free(A);

    return 0;
}

