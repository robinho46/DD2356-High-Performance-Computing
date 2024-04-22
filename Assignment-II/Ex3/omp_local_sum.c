#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SIZE 10000000  // 10^7 elementsi

double serial_sum(double *x, size_t size) {
    double sum_val = 0.0;
    for (size_t i = 0; i < size; i++) {
        sum_val += x[i];
    }
    return sum_val;
}

double omp_local_sum(double *x, size_t size, int num_threads){
	double local_sum[num_threads];
	#pragma omp parallel shared(local_sum)
	{
    	int id = omp_get_thread_num();
		local_sum[id] = 0;
        #pragma omp for
            for (size_t i = 0; i<size; i++) {
                local_sum[id] += x[i];
            }
    }
    double sum_val = 0.0;
    for (int i = 0; i < num_threads; i++) {
       sum_val += local_sum[i];
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
            double sum = omp_local_sum(input, SIZE, threads[thread_index]); // Call the correct sum function
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
