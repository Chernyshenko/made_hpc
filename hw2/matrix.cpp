
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
    for (int l = 0; l < k; l++) {
      for (int i = 0; i < m; i++) {    
            double c = 0;
            for (int j = 0; j < n; j++) {
                c += a[i * n + j] * b[j * k + l];
            }
            r[i * n + l] = c;
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
