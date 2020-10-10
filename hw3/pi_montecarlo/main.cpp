#include <omp.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <stdint.h>

void seedThreads(unsigned int* seeds) {
    unsigned int seed;
    int thread_id;
    #pragma omp parallel private(seed, thread_id) 
    {
        thread_id = omp_get_thread_num();
        seed = (unsigned int) time(NULL);
        seeds[thread_id] = (seed & 0xFFFFFFF0) | (thread_id + 1);
    }
}

double pi_montecarlo(size_t num_threads, long long N) {

    int pointsInCircle = 0;
    int thread_id;
    omp_set_num_threads(num_threads);
    unsigned int *seeds = new unsigned int [num_threads];
    
    seedThreads(seeds);
    unsigned int seed;

    double start, end;
    start = omp_get_wtime();

#pragma omp parallel num_threads(num_threads) shared(seeds) \
        private(thread_id, seed) reduction(+:pointsInCircle) 
    {
        thread_id = omp_get_thread_num();
        seed = seeds[thread_id];
        #pragma omp for
        for(int i = 0; i < N; i++) {
            double x = ((double) rand_r(&seed) / (RAND_MAX));
            double y = ((double) rand_r(&seed) / (RAND_MAX));
            if (x*x + y*y < 1 + 1e-14) {
                pointsInCircle += 1;
            }
        }
    }

    double res = 4.0 * pointsInCircle / N;
    end = omp_get_wtime();
    printf("Time is %lf\n", end - start);
    delete[] seeds;
    return res;
}

int main(int argc, char ** argv){
    size_t num_threads = 1;
    long long N = 10000;
    if (argc > 1) {
        num_threads = std::stoi(argv[1]);
    }
    if (argc > 2) {
        N = std::stoll(argv[2]);
    }
    double pi = pi_montecarlo(num_threads, N);
    printf("Pi = %.10lf\n", pi);
    return 0;
}