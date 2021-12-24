#pragma once
#include <iostream>
#include <stdlib.h>
#include <immintrin.h>
int ind(const int& a, const int& b, int& N){
    return a * N + b;
}

void print_matrix(float* R, int N){
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N; j++){
            int index = ind(i,j,N);
            std::cout << R[index] << ' ';
            }
        std::cout << std::endl;    
        }
    std::cout << std::endl;   
    std::cout << std::endl;  
}

void add(float* R,float* A, float* B, int N) {
    __m256* mR = (__m256*)R; 
    __m256* mA = (__m256*)A; 
    __m256* mB = (__m256*)B; 
    for (int i = 0; i < N * N / 8; i++) {
        __m256 tmp = _mm256_loadu_ps((float*)mA);
        __m256 tmp1 = _mm256_loadu_ps((float*)mB);
        mR[i] = _mm256_add_ps(tmp, tmp1);
        mA++;
        mB++;
    }
}

void sub(float* R,float* A, float* B, int N) {
    std::cout << "1" << std::endl;
    __m256* mR = (__m256*)R; 
    __m256* mA = (__m256*)A; 
    __m256* mB = (__m256*)B; 
    std::cout << "2" << std::endl;
    for (int i = 0; i < (N * N) / 8; i++) {
        std::cout << "3" << std::endl;
        __m256 tmp = _mm256_loadu_ps((float*)mA);
        __m256 tmp1 = _mm256_loadu_ps((float*)mB);
        std::cout << "4" << std::endl;
        mR[i] = _mm256_sub_ps(tmp, tmp1);
        std::cout << "5" << std::endl;
        mA++;
        mB++;
    }
}

void transposeMatrix(float* R,float* A, int N) { 
    __m256* mA = (__m256*)A;  
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N / 8; j++){
            __m256 tmp = _mm256_loadu_ps((float*)mA);
            R[ind(j * 8, i, N)] = tmp[0];
            R[ind(j * 8 + 1, i, N)] = tmp[1];
            R[ind(j * 8 + 2, i, N)] = tmp[2];
            R[ind(j * 8 + 3, i, N)] = tmp[3];
            R[ind(j * 8 + 4, i, N)] = tmp[4];
            R[ind(j * 8 + 5, i, N)] = tmp[5];
            R[ind(j * 8 + 6, i, N)] = tmp[6];
            R[ind(j * 8 + 7, i, N)] = tmp[7];
            mA++;
        }
    }
}

float inner2(float* x, float* y, int n){
    __m256 *xx, *yy;
    __m256 p, s;
    xx = (__m256*)x;
    yy = (__m256*)y;
    s = _mm256_setzero_ps();
    for(int i = 0; i < n / 8; ++i){
        __m256 tmp1 = _mm256_loadu_ps((float*)xx);
        __m256 tmp2 = _mm256_loadu_ps((float*)yy);
        p = _mm256_mul_ps(tmp1,tmp2);
        s = _mm256_add_ps(s,p);
        xx++;
        yy++;
    }
    return s[0] + s[1] + s[2] + s[3] + s[4] + s[5] + s[5] + s[7];
}

float max_sum_line(float* A ,int N){
    float max = 0;
    for (int i = 0; i < N; i++) {
        float sum = 0;
        for (int j = 0; j < N; j++) {
            sum += abs(A[i * N + j]);
        }
        max = std::max(sum, max);
    }
    return max;
}

float max_sum_column(float* A ,int N){
    float max = 0;
    for (int i = 0; i < N; i++) {
        float sum = 0;
        for (int j = 0; j < N; j++) {
            sum += abs(A[j * N + i]);
        }
        max = std::max(sum, max);
    }
    return max;
}

void mul(float* R, float* A, float* B, int N) {
    float* transB = (float*)malloc(N * N * sizeof(float));
    transposeMatrix(transB, B, N);
    
    for (int i = 0; i < N ; i++) {
        for (int j = 0; j < N ; j++) {
            R[ind(i,j,N)] = inner2(&A[i*N], &transB[j*N], N);
        }
    }
    std::cout << "free" << std::endl;
    free(transB);
    std::cout << "free" << std::endl;
}

void calc_B(float* B, float* A, int N){
    float nenner = max_sum_line(A, N) * max_sum_column(A, N);
    std::cout << "transponse" << std::endl;
    transposeMatrix(B, A, N);
    std::cout << "transponse" << std::endl;
    for(int i = 0; i < N * N; i++){
        B[i] = B[i] / nenner;
    }
}

void calc_R(float* R, float* A, float* B, float* I, int N){
    float* tmp = (float*)calloc(N * N, sizeof(float));
    std::cout << "sub1" << std::endl;
    mul(tmp, B, A, N);
    std::cout << "sub" << std::endl;
    // print_matrix(tmp, N); 
    // print_matrix(I, N);   
    //print_matrix(R, N);     
    std::cout << "sub" << std::endl;
    sub(R, I, tmp, N);
    std::cout << "sub" << std::endl;
    delete[] tmp;
    std::cout << "sub" << std::endl;
}

void calc_reverse_matrix(float* result, float* R, float* B, float* I, int N, int M ){
    float* left = I;
    float* tmp = new float[N*N];
    float* tmp_1 = new float[N*N];
    float* left1 = new float[N*N];

    for(int i = 0; i < N*N; i++) tmp_1[i] = R[i];
    
    for(int i = 0; i < M; i++){
        mul(tmp, R, tmp_1, N);
        add(left1, left, tmp, N);
        for(int i = 0; i < N*N; i++){
            R[i] = tmp[i];
            tmp[i] = 0;
            left[i] = left1[i];
            left1[i] = 0;
        } 
    }
    mul(result, left, B, N);
    delete [] tmp;
    delete [] tmp_1;
    delete [] left1;
}

void calc_I(float* R, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){

            R[ind(i,j,N)] = (i == j);
        }
    }
}

