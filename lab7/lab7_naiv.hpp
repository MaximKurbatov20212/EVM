#pragma once
#include <iostream>
#include <stdlib.h>

int ind(int a, int b, int N){
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

void add(float* R, float* A, float* B, int N){
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
            int index = ind(i,j,N);
            R[index] = A[index] + B[index];
        }
    }
}

void sub(float* R, float* A, float* B, int N){
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
            int index = ind(i,j,N);
            R[index] = A[index] - B[index];
        }
    }
}

void mul(float* R, float* A, float* B, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            for(int k = 0; k < N; k++){
                  R[ind(i,j,N)] += A[ind(i,k,N)] * B[ind(k,j,N)];
            }
        }
    }
}

void transposeMatrix(float* R,float* A, int N) {   
    for(int i = 0; i < N; i++){
        for(int j = i; j < N; j++){
            R[ind(i,j,N)] = A[ind(j,i,N)];
            R[ind(j,i,N)] = A[ind(i,j,N)];
        }
    }
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

void calc_B(float* B, float* A, int N){
    float nenner = max_sum_line(A, N) * max_sum_column(A, N);
    transposeMatrix(B, A, N);
    for(int i = 0; i < N * N; i++){
        B[i] = B[i] / nenner;
    }
}

void calc_R(float* R, float* A, float* B, float* I, int N){
    float* tmp = (float*)calloc(N * N, sizeof(float));
    mul(tmp, B, A, N);
    sub(R, I, tmp, N);
    delete[] tmp;
}

void calc_reverse_matrix(float* result, float* R, float* B, float* I, int N, int M ){
    float* left = I;
    float* tmp = new float[N*N];
    float* tmp_1 = new float[N*N];
    for(int i = 0; i < N*N; i++) tmp_1[i] = R[i];

    for(int i = 0; i < M; i++){
        mul(tmp, R, tmp_1, N);
        add(left, left, tmp, N);

        for(int i = 0; i < N*N; i++){
            R[i] = tmp[i];
            tmp[i] = 0;
        } 
    }
    mul(result, left, B, N);
    delete [] tmp;
    delete [] tmp_1;
}

void calc_I(float* R, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            R[ind(i,j,N)] = (i == j);
        }
    }
}
