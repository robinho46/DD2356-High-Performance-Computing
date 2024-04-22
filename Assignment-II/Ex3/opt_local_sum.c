#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SIZE 10000000  // 10^7 elements
#define MAX_THREADS 128  // Maximum number of threads
#define CACHE_LINE_SIZE 64  // Typical size of cache line in bytes
#define PAD 8  // Number of doubles in a cache line (64 bytes / 8 bytes per double)

double opt_local_sum(double *input, size_t size, int num_threads) {
	double local_sum[MAX_THREADS * PAD] = {0};  // Initialize local sums with padding
    #pragma omp parallel num_threads(num_threads)
    {
		int id = omp_get_thread_num();
        #pragma omp for
        for (size_t i = 0; i < size; i++) {
            local_sum[id * PAD] += input[i];  // Use padded index
        }
    }
    double sum_val = 0.0;
    for (int i = 0; i < num_threads; i++) {
       sum_val += local_sum[i * PAD];  // Aggregate using padded indices
    }
    return sum_val;
}

void generate_random(double *input, size_t size) {
    for (size_t i = 0; i < size; i++) {
        input[i] = rand() / (double)(RAND_MAX);
    }
}

int main() {
    double *input = (double *)malloc(SIZE * sizeof(double));
    if (input == NULL) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }
    generate_random(input, SIZE);

    int threads[] = {1, 32, 64, 128};
    int num_threads = sizeof(threads) / sizeof(threads[0]);

    printf("Number of Threads, Average Time, Standard Deviation\n");
    for (int thread_index = 0; thread_index < num_threads; thread_index++) {
        omp_set_num_threads(threads[thread_index]);

        double total_time = 0.0, times[10]; // Array to store times for each test
        for (int t = 0; t < 10; t++) {
            double start_time = omp_get_wtime();
            double sum = opt_local_sum(input, SIZE, threads[thread_index]); // Call the optimized sum function
            double end_time = omp_get_wtime();
            times[t] = end_time - start_time;
            total_time += times[t];
        }

        double mean_time = total_time / 10;
        double std_dev = 0.0;
        for (int t = 0; t < 10; t++) {
            std_dev += (times[t] - mean_time) * (times[t] - mean_time);
        }
        std_dev = sqrt(std_dev / 10);

        printf("%d, %f, %f\n", threads[thread_index], mean_time, std_dev);
    }

    free(input);
    return EXIT_SUCCESS;
}

