#include <iostream>
#include <time.h>
#include <omp.h>
#include <cstring>
#include <stdlib.h>

#include "matrix.h"

using namespace std;

int main(int argc, char* argv[]) {
    int n = 1024;
    int p = 2;
    if (argc > 1) {
       n = atoi(argv[1]);
    }
    else {
        cin >> n;
    }
    if (argc > 2) {
       p = atoi(argv[2]);
    }
    else {
        cin >> p;
    }
    double *a = new double [n * n];
    double *ap = new double [n * n];

    generate_matrix(a, n, n);
    print_matrix(a, n, n);

    double t = clock();
    double start, end;
#ifdef USE_OMP
    start = omp_get_wtime();
#endif
    matrix_power(a, n, p, ap);
    std::cout << "Powered matrix:\n";
    print_matrix(ap, n, n);
#ifdef USE_OMP
    end = omp_get_wtime();
    cout << "N = " << n << " OMP time is " <<  end - start << endl;
#else
    t = (clock() - t) / CLOCKS_PER_SEC;
    cout << "N = " << n <<" Time for matrix_power: " << t << endl;
#endif
   

    delete[] a;
    delete[] ap;
    return 0;
}
