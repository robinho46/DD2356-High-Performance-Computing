import matplotlib.pyplot as plt

# Read average values from file
with open("avgTime.txt", "r") as file:
    averages = [float(line.strip()) for line in file.readlines()]

# Generating labels for sequences
labels = ["8", "16", "32", "64", "128", "256"]

# Plot
plt.figure(figsize=(8, 5))
plt.bar(labels, averages, color='skyblue')
plt.xlabel('Processes')
plt.ylabel('Average Values')
plt.title('Average Values of Sequences')
plt.xticks(rotation=45, ha='right')
plt.tight_layout()
plt.show()
