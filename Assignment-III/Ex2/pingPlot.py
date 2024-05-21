import numpy as np
import matplotlib.pyplot as plt

def read_data(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    sizes = [int(line.split()[0]) for line in lines]
    times = [float(line.split()[1]) for line in lines]
    return sizes, times

def bytes_to_readable(size_in_bytes):
    units = ['B', 'KiB', 'MiB', 'GiB', 'TiB']
    size = size_in_bytes
    unit_idx = 0
    while size >= 1024 and unit_idx < len(units) - 1:
        size /= 1024
        unit_idx += 1
    return f"{size:.0f} {units[unit_idx]}"

def plot_ping_pong_times(sizes, times, communication_type, filename):
    readable_labels = [bytes_to_readable(size) for size in sizes]
    plt.figure(figsize=(12, 6))
    plt.plot(sizes, times, marker='o', linestyle='-')
    plt.title(f"Ping-Pong Time vs Message Size ({communication_type} communication)")
    plt.xlabel("Message Size")
    plt.ylabel("Time (seconds)")
    plt.grid(True)
    plt.xscale('log')
    plt.yscale('log')

    # Manually set x-axis ticks and labels
    plt.xticks(ticks=sizes, labels=readable_labels, rotation=45, ha='right')

    plt.tight_layout()
    plt.savefig(filename)
    plt.show()

def main():
    # Read data for intra-node communication
    intra_sizes, intra_times = read_data("results/intra.txt")
    plot_ping_pong_times(intra_sizes, intra_times, "Intra-node", "intra_node_communication.png")

    # Read data for inter-node communication
    inter_sizes, inter_times = read_data("results/inter.txt")
    plot_ping_pong_times(inter_sizes, inter_times, "Inter-node", "inter_node_communication.png")

if __name__ == "__main__":
    main()
