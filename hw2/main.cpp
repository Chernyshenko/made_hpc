#include <iostream>
#include <time.h>

#include "matrix.h"

//#define USE_CBLAS

using namespace std;

int main(int argc, char* argv[]) {
    int n;
    if (argc > 1) {
	n = std::stoi(argv[1]);
    }
    else{
        cin >> n;
    }
    double *a = new double [n * n];
    double *b = new double [n * n];
    double *c = new double [n * n];
    generate_matrix(a, n, n);
    generate_matrix(b, n, n);
    double t = clock();
#ifdef USE_CBLAS
    matrix_mult_matrix_cblas(a, b, n, n, n, c);
#else
    matrix_mult_matrix(a, b, n, n, n, c);
#endif
    t = (clock() - t) / CLOCKS_PER_SEC;
    cout << "N = " << n <<" Time for mult matrix: " << t << endl;

    delete[] a;
    delete[] b;
    delete[] c;
    return 0;
}
