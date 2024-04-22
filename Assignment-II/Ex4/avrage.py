import numpy as np

def read_times(filename):
    times = []
    with open(filename, 'r') as file:
        for line in file:
            if "DFTW computation in" in line:
                start = line.find("in") + 3
                end = line.find("seconds")
                time = float(line[start:end].strip())
                times.append(time)
    return times

def calculate_stats(times):
    average_time = np.mean(times)
    std_dev_time = np.std(times)
    return average_time, std_dev_time

thread_counts = [1, 32, 64, 128]
files = ['1.txt', '32.txt', '64.txt', '128.txt']

for count, file in zip(thread_counts, files):
    times = np.array(read_times(file))
    average_time, std_dev_time = calculate_stats(times)
    print(f"{count} threads:")
    print(f"Average Execution Time: {average_time:.6f} seconds")
    print(f"Standard Deviation: {std_dev_time:.6f} seconds")
    print()

