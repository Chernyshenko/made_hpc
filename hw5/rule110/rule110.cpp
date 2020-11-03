#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include "EasyBMP.hpp"
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
    return 0;
}
struct Rules{
    Rules(int ruleid = 110) {
        RULE_ID = ruleid;
        all_rules = new int*[256];
        for (int i = 0; i < 256; i++) {
            all_rules[i] = new int[8];
        }
        int current_rule[8];
        int irule = 0;
        generate_all_rules(all_rules, current_rule, 8, 0, irule);
    }
    ~Rules() {
        for (int i = 0; i < 256; i++) {
            delete[] all_rules[i];
        }
        delete[] all_rules;
    }
    int getID() const {
        return RULE_ID;
    }
    bool apply_rule_100(bool a, bool l, bool r) {

        int wolfram_rule[8] = {0, 1, 1, 0, 1, 1, 1, 0};
        int i = index(l, a, r);
        return wolfram_rule[i];
    }
    bool apply_rule(bool a, bool l, bool r) {
        int i = index(l, a, r);
        return all_rules[RULE_ID][i];
    }
    void set_rule_id(int i) {
        if (i < 0 || i >= 256) return;
        RULE_ID = i;
    }
private:
    int RULE_ID = 110;
    int** all_rules;//( 256 , vector<int> (8)); 
    void generate_all_rules(int** all_rules, int* current_rule, int n, int i, int& irule) {
        if (i == n) {
            memcpy(all_rules[irule], current_rule, n * sizeof(int)); 
            irule++;
            return;
        }
        current_rule[i] = 0;
        generate_all_rules(all_rules, current_rule, n, i + 1, irule);

        current_rule[i] = 1;
        generate_all_rules(all_rules, current_rule, n, i + 1, irule);

    }
};

template<class T>
void print_array(T* a, int n) {
    int NMAX = 16;
    int m = min(n, NMAX);
    for (int i = 0; i < m; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}


void rule110(Rules& rules, bool* a, int n, int nStep, bool periodic=false) {
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
            bool t = rules.apply_rule(a[i], l, r);
            l = a[i];
            a[i] = t;
        }
        print_array(a, n);
    }
    return;
}
void rule110_mpi(Rules& rules, bool* a, long int total_n, long int n, int prank, int psize, int nStep, bool periodic = true) {
    MPI_Status status;
    init_random_bool(a, n, prank);
    EasyBMP::RGBColor white(255, 255, 255);
    string fname = "rule" + to_string(rules.getID()) + ".bmp";  
    EasyBMP::Image img(n, nStep, fname, white);
    if (prank == 0) {
        //print_array(a, n);
    }

    bool l, r;
    int start = 0;
    int end = n - 1;
    if (!periodic) {
        if (prank == 0) {
            start = 1;
        }
        if (prank == psize - 1) {
            end = n - 2;
        }
    }
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
        MPI_Recv(&gosts_recv[1], 1, MPI_SHORT, pnext, tag, MPI_COMM_WORLD, &status);
        MPI_Send(&gosts_send[1], 1, MPI_SHORT, pnext, tag, MPI_COMM_WORLD);
        MPI_Recv(&gosts_recv[0], 1, MPI_SHORT, pprev, tag, MPI_COMM_WORLD, &status);
        
        l = gosts_recv[0];
        if (!periodic && prank == 0) l = a[0];
        for (int i = start; i <= end; i++) {
            if (i == n - 1) r = gosts_recv[1];
            else r = a[i + 1];
            bool t = rules.apply_rule(a[i], l, r);
            l = a[i];
            a[i] = t;
        }
        if (prank == 0) {
            //print_array(a, n);
            for (int px = 0; px < n; px++) {
                int color = 255;
                if (a[px]) color = 0;
                img.SetPixel(px, step, EasyBMP::RGBColor(color, color, color));
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (prank == 0) {
        img.Write();
    }
    return;
}

int main(int argc, char* argv[]) {
    MPI_Status status;
    int psize, prank;
    long int n = 1000;
    long int n_per_proc;
    int nStep = 1000;
    bool* a;
    int RULE_ID = 110;
    bool PERIODIC = true;

    Rules rules;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);

    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    
    if (prank == 0) {
        printf("Usage: mpirun -n {n_proc} ./a.out {n} {nSteps} {rule_id}\n");
        if (argc > 1) {
            n = atol(argv[1]);
        }
        if (argc > 2) {
            nStep = atoi(argv[2]);
        }
        if (argc > 3) {
            RULE_ID = atoi(argv[3]);
            rules.set_rule_id(RULE_ID);
        }
    }
    MPI_Bcast (&n, 1, MPI_LONG, prank, MPI_COMM_WORLD);
    MPI_Bcast (&nStep, 1, MPI_INT, prank, MPI_COMM_WORLD);
    n_per_proc = n / psize;
    //MPI_Bcast (&n_per_proc, 1, MPI_INT, prank, MPI_COMM_WORLD);

    a = new bool[n_per_proc];
    double starttime, endtime;
    starttime = MPI_Wtime();
    rule110_mpi(rules, a, n, n_per_proc, prank, psize, nStep, PERIODIC);
    endtime   = MPI_Wtime();
    if (prank == 0) {
        printf("rule110_mpi took %f seconds\n",endtime - starttime);
    }
   // delete[] a;
    MPI_Finalize();
    return 0;
}