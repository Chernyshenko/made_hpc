#include <mpi.h>
#include <iostream>
using namespace std;

void init_random_bool(bool* v, long int n, int seed = 1) {
    srand(n + seed);
    for(int i = 0; i < n; i++) {
        v[i] = bool(rand() % 2);
    } 
}

int index(bool a, bool b, bool c) {
    if (a == true && b == true && c == true) return 0;
    if (a == true && b == true && c == false) return 1;
    if (a == true && b == false && c == true) return 2;
    if (a == true && b == false && c == false) return 3;
    if (a == false && b == true && c == true) return 4;
    if (a == false && b == true && c == false) return 5;
    if (a == false && b == false && c == true) return 6;
    if (a == false && b == false && c == false) return 7;
}
bool apply_rule(bool a, bool l, bool r) {

    int wolfram_rule[8] = {0, 1, 1, 0, 1, 1, 1, 0};
    int i = index(l, a, r);
    return wolfram_rule[i];
}
void print_array(bool* a, int n) {
    int NMAX = 16;
    int m = min(n, NMAX);
    for (int i = 0; i < m; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void rule110(bool* a, int n, int nStep, bool periodic=false) {
    init_random_bool(a, n);
    print_array(a, n);

    bool l, r;
    if (periodic) {
        l = a[n - 1];
    }
    bool a0 = a[0];
    int start = periodic ? 0 : 1;
    int end = periodic ? n - 1 : n - 2;

    for (int step = 0; step < nStep; step++){
        for (int i = start; i <= end; i++) {
            if (i == n - 1) r = a0;
            else r = a[i + 1];
            bool t = apply_rule(a[i], l, r);
            l = a[i];
            a[i] = t;
        }
        print_array(a, n);
    }
    return;
}
void rule110_mpi(bool* a, long int n, int prank, int psize, int nStep, bool periodic = true) {
    MPI_Status status;
    init_random_bool(a, n, prank);
    if (prank == 0) {
        print_array(a, n);
    }

    bool l, r;
    int start = periodic ? 0 : 1;
    int end = periodic ? n - 1 : n - 2;
    int tag = 0;
    short gosts_send[2];
    short gosts_recv[2] = {1, 1};

    int pnext = (prank + 1) % psize;
    int pprev = (prank - 1 + psize) % psize;
  //  cout << prank << " next: " << pnext << " prev: " << pprev << endl;
    MPI_Request request;

    for (int step = 0; step < nStep; step++){
        gosts_send[0] = a[0];
        gosts_send[1] = a[n - 1];
        MPI_Send(&gosts_send[0], 1, MPI_SHORT, pprev, tag, MPI_COMM_WORLD);
        cout << prank << " send to " << pprev << endl;
        MPI_Recv(&gosts_recv[1], 1, MPI_SHORT, pnext, tag, MPI_COMM_WORLD, &status);
        cout << prank << " recv from " << pnext << endl;
       // MPI_Send(&gosts_send[1], 1, MPI_SHORT, pnext, tag, MPI_COMM_WORLD);
       // MPI_Recv(&gosts_recv[0], 1, MPI_SHORT, pprev, tag, MPI_COMM_WORLD, &status);
        
        l = gosts_recv[0];
        for (int i = start; i <= end; i++) {
            if (i == n - 1) r = gosts_recv[1];
            else r = a[i + 1];
            bool t = apply_rule(a[i], l, r);
            l = a[i];
            a[i] = t;
        }
        if (prank == 0) {
            print_array(a, n);
        }
        //MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    return;
}

int main(int argc, char* argv[]) {
    MPI_Status status;
    int psize, prank;
    long int n = 10000;
    long int n_per_proc;
    int nStep = 1000;
    bool* a;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);

    MPI_Comm_rank(MPI_COMM_WORLD, &prank);

    if (prank == 0) {
        if (argc > 1) {
            n = atol(argv[1]);
        }
        if (argc > 2) {
            nStep = atoi(argv[2]);
        }
    }
    MPI_Bcast (&n, 1, MPI_LONG, prank, MPI_COMM_WORLD);
    MPI_Bcast (&nStep, 1, MPI_INT, prank, MPI_COMM_WORLD);
    n_per_proc = n / psize;
    //MPI_Bcast (&n_per_proc, 1, MPI_INT, prank, MPI_COMM_WORLD);

    a = new bool[n_per_proc];
    double starttime, endtime;
    starttime = MPI_Wtime();
    rule110_mpi(a, n_per_proc, prank, psize, nStep, true);
    endtime   = MPI_Wtime();
    //if (prank == 0) {
        printf("rule110_mpi took %f seconds\n",endtime - starttime);
    //}
   // delete[] a;
    MPI_Finalize();
    return 0;
}