#include <cblas.h>

void matrix_mult_matrix_cblas(double* a, double* b, int m, int k, int n, double *r) {
    double alpha = 1.0; 
    double beta = 0.0;

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                m, n, k, alpha, a, k, b, n, beta, r, n);
}