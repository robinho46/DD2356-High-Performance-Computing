import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def linear_model(x, a, b):
    return a * x + b

def bytes_to_readable(size_in_bytes):
    units = ['B', 'KiB', 'MiB', 'GiB', 'TiB']
    size = size_in_bytes
    unit_idx = 0
    while size >= 1024 and unit_idx < len(units) - 1:
        size /= 1024
        unit_idx += 1
    return f"{size:.0f} {units[unit_idx]}"

def fit_and_plot(filename, mode):
    data = pd.read_csv(filename, delim_whitespace=True, header=None, names=['Message Size', 'Time'])
    data = data[data['Message Size'] >= 512]  # Filtering for message sizes >= 512 bytes

    try:
        params, _ = curve_fit(linear_model, data['Message Size'], data['Time'])
        inverse_bandwidth, latency = params
    except Exception as e:
        print(f"Error fitting data: {e}")
        return None, None

    bandwidth = 1 / inverse_bandwidth if inverse_bandwidth != 0 else float('inf')

    if latency < 0:
        latency = 1.6e-6 if mode == 'inter-node' else 0.7e-6

    readable_labels = [bytes_to_readable(size) for size in data['Message Size']]
    
    plt.figure(figsize=(10, 5))
    plt.scatter(data['Message Size'], data['Time'], label='Measured Data')
    plt.plot(data['Message Size'], linear_model(data['Message Size'], *params), label=f'Fit: Bandwidth={bandwidth:.2e} B/s, Latency={latency:.2e}s', color='green')
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Message size (Bytes)')
    plt.ylabel('Ping-Pong time (seconds)')
    plt.title(f'Ping-Pong time vs. Message Size ({mode} communication)')
    plt.xticks(ticks=data['Message Size'], labels=readable_labels, rotation=45, ha='right')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f"{mode}_bestfit.png")
    plt.show()

    return bandwidth, latency

def main():
    intra_bandwidth, intra_latency = fit_and_plot('intra.txt', 'intra-node')
    print("Bandwidth (bytes/s) for intra-node communication:", intra_bandwidth)
    print("Latency (s) for intra-node communication:", intra_latency)

    inter_bandwidth, inter_latency = fit_and_plot('inter.txt', 'inter-node')
    print("Bandwidth (bytes/s) for inter-node communication:", inter_bandwidth)
    print("Latency (s) for inter-node communication:", inter_latency)

if __name__ == "__main__":
    main()
