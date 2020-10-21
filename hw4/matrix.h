#pragma once
void matrix_mult_vector(double* a, double* v, int m, int n, double *r);
void matrix_mult_matrix(double* a, double* b, int m, int n, int k, double *r);
void generate_matrix(double* a, int m, int n);
void matrix_mult_matrix_cblas(double* a, double* b, int m, int k, int n, double *r);
void matrix_power(double* a, int n, int p, double* res);
void print_matrix(double* a, int m, int n);
double l2norm(double* a, int n);
double l1norm(double* a, int n);
void print_vector(double* a, int n);
void init_random_vector(double* v, int n);

