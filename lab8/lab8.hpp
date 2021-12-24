#include <stdlib.h>

float** create_I(int size) {
	float** matrix = (float**)malloc(sizeof(float*) * size);
	for (int row = 0; row < size; row++) {
		matrix[row] = (float*)calloc(sizeof(float), size);
		matrix[row][row] = 1;
	}
	return matrix;
}

float** create_zero_matrix(int size) {
	float** matrix = (float**)malloc(sizeof(float*) * size);
	for (int row = 0; row < size; row++) {
		matrix[row] = (float*)calloc(sizeof(float), size);
	}
	return matrix;
}

void free_matrix(int size, float** matrix) {
	for (int row = 0; row < size; row++) {
		free(matrix[row]);
	}
	free(matrix);
}
void copy_matrix_to_matrix(int size, float** matrix_from, float** matrix_to) {
	for (int row = 0; row < size; row++) {
		for (int column = 0; column < size; column++) {
		matrix_to[row][column] = matrix_from[row][column];
		}
	}
}

void mul(int size, float** matrix_to_mult, float** matrix_on) {
	float** result = create_zero_matrix(size);
	for (int row_mat1 = 0; row_mat1 < size; row_mat1++) {
		for (int col_mat1 = 0; col_mat1 < size; col_mat1++) {
		float mult_koef = matrix_to_mult[row_mat1][col_mat1];
		for (int col_mat2 = 0; col_mat2 < size; col_mat2++) {
			result[row_mat1][col_mat2] += mult_koef * matrix_on[col_mat1][col_mat2];
		}
		}
	}
	copy_matrix_to_matrix(size, result, matrix_to_mult);
	free_matrix(size, result);
}

