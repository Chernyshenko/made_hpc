#include <cstring>
#include <algorithm>
#include <iostream>
#include <math.h>
//#include <random>

#include <omp.h>

#ifdef USE_CBLAS
#include <cblas.h>

void matrix_mult_matrix_cblas(double* a, double* b, int m, int n, int k, double *r) {
    double alpha = 1.0; 
    double beta = 0.0;

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                m, k, n, alpha, a, m, b, k, beta, r, m);
}
void matrix_mult_vector_cblas(double* a, double* x, int m, int n, double *r) {
    double alpha = 1.0; 
    double beta = 0.0;
    cblas_dgemv(CblasRowMajor, CblasNoTrans, 
                m, n, alpha, a, m, x, 1, beta, r, 1);
}
#endif

using namespace std;

void matrix_mult_vector(double* a, double* v, int m, int n, double *r) {
#ifdef USE_CBLAS
    matrix_mult_vector_cblas(a, v, m, n, r);
    return;
#endif
#ifdef USE_OMP
    #pragma omp parallel for shared(a, r)
#endif
    for (int i = 0; i < m; i++) {
        double c = 0;
        for (int j = 0; j < n; j++) {
            c += a[i * n + j] * v[j];
        }
        r[i] = c;
    }
}
void matrix_mult_matrix(double* a, double* b, int m, int n, int k, double *r) {
#ifdef USE_CBLAS
    matrix_mult_matrix_cblas(a, b, m, n, k, r );
    return;
#endif
    std::memset(r, 0, m * k * sizeof(double));
    int i, j, l;
#ifdef USE_OMP
    //omp_set_num_threads(omp_get_num_procs());
    #pragma omp parallel for default(none)  private(i, j, l) shared(a, b, r, m, n, k) 
#endif
    for (j = 0; j < n; j++) {
      	for (i = 0; i < m; i++) {    
	        for (l = 0; l < k; l++) {
                r[i * k + l] += a[i * n + j] * b[j * k + l];
            }
        }
    }
}

void generate_matrix(double* a, int m, int n) {
#ifdef USE_OMP
    #pragma omp parallel for shared(a)
#endif
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = abs(i - j) == 1 ? 1 : 0;
        }
        a[0 * n + n - 1] = 1;
        a[(n - 1) * n] = 1;
    }
}
void init_random_vector(double* v, int n) {
    srand(n);
    for(int i = 0; i < n; i++) {
        v[i] = rand();
    }
}

void matrix_sq(double* a, int n, double* res) {
    matrix_mult_matrix(a, a, n, n, n, res);
}

void matrix_power(double* a, int n, int p, double* res) {
    int cur_p = 1;
    double *t = new double [n * n];
    std::memcpy(t, a, n * n * sizeof(double));
    while(cur_p * 2 <= p) {
        matrix_sq(t, n, res);
        std::memcpy(t, res, n * n * sizeof(double));
        cur_p *= 2;
    }
    while(cur_p < p) {
        matrix_mult_matrix(t, a, n, n, n, res);
        std::memcpy(t, res, n * n * sizeof(double));
        cur_p++;
    }
    delete[] t;
}
void print_matrix(double* a, int m, int n) {
    int MAX_N = 8;
    for (int i = 0; i < std::min(m, MAX_N); i++) {
        for (int j = 0; j < std::min(n, MAX_N); j++)
        {
            cout << a[i * n + j] << "\t";
        }
        cout << endl;
    }
}
void print_vector(double* a, int n) {
    int MAX_N = 10;
    for (int i = 0; i < std::min(n, MAX_N); i++) {
        cout << a[i] << "\t";
    }
    cout << endl;
}


double l2norm(double* a, int n) {
#ifdef USE_CBLAS
    return cblas_dnrm2(n, a, 1);
#endif
    double res = 0;
    for (int i = 0; i < n; ++i)
    {
        res += a[i] * a[i];
    }
    return sqrt(res);
}
double l1norm(double* a, int n) {
    double res = 0;
#ifdef USE_OMP
    #pragma omp parallel for reduction(+:res)
#endif
    for (int i = 0; i < n; ++i)
    {
        res += fabs(a[i]);
    }
    return res;
}