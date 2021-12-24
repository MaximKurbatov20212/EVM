#include <iostream>
#include <stdlib.h>
#include <xmmintrin.h>
inline int ind(const int& a, const int& b, int& N){
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
    for (int i = 0; i < N * N / 4; i++) {
        ((__m128*)R)[i] = _mm_add_ps(((__m128*)A)[i], ((__m128*)B)[i]);
    }
}

void sub(float* R,float* A, float* B, int N) {
for (int i = 0; i < N * N / 4; i++) {
        ((__m128*)R)[i] = _mm_sub_ps(((__m128*)A)[i], ((__m128*)B)[i]);
    }
}

void transposeMatrix(float* R,float* A, int N) { 
    __m128* mA = (__m128*)A;  
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N / 4; j++){
            __m128 tmp = _mm_load_ps((float*)mA);
            R[ind(j * 4, i, N)] = tmp[0];
            R[ind(j * 4 + 1, i, N)] = tmp[1];
            R[ind(j * 4 + 2, i, N)] = tmp[2];
            R[ind(j * 4 + 3, i, N)] = tmp[3];
            mA++;
        }
    }
}

 float inner2(float* x, float* y, int n){
    __m128 *xx, *yy;
    __m128 p, s;
    xx = (__m128*)x;
    yy = (__m128*)y;
    s = _mm_setzero_ps();
    for(int i=0; i < n / 4; ++i){
        p = _mm_mul_ps(xx[i],yy[i]);
        s = _mm_add_ps(s,p);
    }
    p = _mm_movehl_ps(p,s);
    s = _mm_add_ps(s,p);
    p = _mm_shuffle_ps(s,s,1);
    s = _mm_add_ss(s,p);
    float sum;
    _mm_store_ss(&sum,s);
    return sum;
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
            R[ind(i,j,N)] = inner2(&A[i*N], &transB[j*N], N); // cкалярное произведение
        }
    }
    free(transB);
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

    for(int i = 0; i < N*N / 4; i++) ((__m128*)tmp_1)[i] = ((__m128*)R)[i];
    
    for(int i = 0; i < M; i++){
        mul(tmp, R, tmp_1, N);
        add(left, left, tmp, N);
        for(int i = 0; i < N*N / 4; i++){
            ((__m128*)R)[i] = ((__m128*)tmp)[i];
            ((__m128*)tmp)[i] = _mm_setzero_ps();
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


// bool comapre(float* A, int N){
//     for(int i = 0; i < N; i++){
//         for(int j = 0; j < N; j++){
//             if(i == j) {
//                 if (abs(1 - abs(A[ind(i,j,N)])) > 0.2) return false;
//             }
//             else if (abs(A[ind(i,j,N)]) > 0.2) return false;
//         }
//     }
//     return true;
// }

int main() {
    timespec start , end;
    int N, M;
    std::cin >> N;
    std::cin >> M;  
    float* result = new float[N*N];
    float* I = new float[N*N];
    float* R = new float[N*N];
    float* A = new float[N*N];
    float* B = new float[N*N];
    float* _A = new float[N*N];

    for(int i = 0 ; i < N; i++){
        for(int j = 0 ; j < N; j++){
            A[ind(i,j,N)] = (i >= j);
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    calc_I(I, N);
    calc_B(B, A, N);
    calc_R(R, A, B, I, N);
    calc_reverse_matrix(result, R, B, I, N, M);
    print_matrix(result, N);
    print_matrix(A, N);
    mul(_A, result, A, N);
    print_matrix(_A, N);

    delete[] result;
    delete[] I;
    delete[] R;
    delete[] A;
    delete[] B;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    std::cerr << "Time taken : " << end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec) << " sec." << std::endl;
    return 0;
}
