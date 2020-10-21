#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <cstring>
#include "../matrix.h"
#include <omp.h>
using namespace std;

double* read_graph(string& fname, int& N) {

    int n;
    int m;
    std::ifstream fin(fname);
    if (!fin) return nullptr;
    fin >> n >> m;
    double *a = new double [n * n];
    memset(a, 0, n * n * sizeof(double));
    int i, j;
    double v;
    for (int e = 0; e < m; e++) {
        fin >> i >> j >> v;
        a[j * n + i] = v;
    }
    N = n;
    return a;
}


void page_rank(double* a, int n, double* rank, int niter, double d) {
    double* ap = new double[n * n];
    double* t = new double[n];
    double h = (1.0 - d) / n;
#ifdef USE_OMP
    #pragma omp parallel for shared(n, ap, a, d, h)
#endif
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            ap[i * n + j] = d * a[i * n + j] + h;
        }
    }
    double l1rank = l1norm(rank, n);
#ifdef USE_OMP
    #pragma omp parallel for shared(n,rank,l1rank)
#endif
    for (int i = 0; i < n; i++) {
        rank[i] = rank[i] / l1rank;
    }
    for (int iter = 0; iter < niter; iter++) {
        matrix_mult_vector(ap, rank, n, n, t);
        double l1t = l1norm(t, n);
#ifdef USE_OMP
    #pragma omp parallel for shared(n,rank,t, l1t)
#endif
        for (int i = 0; i < n; i++) {
            rank[i] = t[i] / l1t;
        }
    }
    double l1t = l1norm(rank, n);
#ifdef USE_OMP
    #pragma omp parallel for shared(n,rank,t, l1t)
#endif
    for (int i = 0; i < n; i++) {
        rank[i] = rank[i] / l1t * 100;
    }
    delete[] ap;
    delete[] t;
}

int main(int argc, char* argv[]) {

   
    string fname = "graph.txt";
    int n;
    double* a = read_graph(fname, n);

    double* rank = new double[n];
    init_random_vector(rank, n);

    int niter = 100;
    double d = 0.85;

    double t = clock();
    page_rank(a, n, rank, niter, d);
    t = (clock() - t) / CLOCKS_PER_SEC;

    print_vector(rank, n);
    cout << "N = " << n <<" Time for page_rank: " << t << endl;


    delete[] a;
    return 0;
}
