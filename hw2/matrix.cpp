#include <cstring>
void matrix_mult_vector(double* a, double* v, int m, int n, double *r) {
    for (int i = 0; i < m; i++) {
        double c = 0;
        for (int j = 0; j < n; j++) {
            c += a[i * n + j] * v[j];
        }
        r[i] = c;
    }
}
void matrix_mult_matrix(double* a, double* b, int m, int n, int k, double *r) {
    std::memset(r, 0, n * k * sizeof(double));
    for (int j = 0; j < n; j++) {
      	for (int i = 0; i < m; i++) {    
	    for (int l = 0; l < k; l++) {
                r[i * k + l] += a[i * n + j] * b[j * k + l];
            }
        }
    }
}

void generate_matrix(double* a, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = i + j;
        }
    }
}
