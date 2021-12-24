#include <iostream>
// #include "lab7_sse.hpp"
#include "lab7_naiv.hpp"
int main(){
    timespec start , end;
    int N, M;
    std::cin >> N;
    std::cin >> M;  
    float* result = new float[N*N];
    float* I = new float[N*N];
    float* R = new float[N*N];
    float* A = new float[N*N];
    float* B = new float[N*N];

    for(int i = 0 ; i < N; i++){
        for(int j = 0 ; j < N; j++){
            A[ind(i,j,N)] = (i >= j);
        }
    }
    //print_matrix(A, N);
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    calc_I(I, N);
    //print_matrix(I, N);
    calc_B(B, A, N);
    //print_matrix(B, N);
    calc_R(R, A, B, I, N);
    calc_reverse_matrix(result, R, B, I, N, M);
    //print_matrix(result, N);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    std::cerr << "Time taken : " << end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec) << " sec." << std::endl;
    delete[] result;
    delete[] I;
    delete[] R;
    delete[] A;
    delete[] B;

    return 0;
}
