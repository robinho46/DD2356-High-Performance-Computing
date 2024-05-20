#!/bin/bash

# Loop to run the command 10 times
for ((i=1; i<=10; i++)); do
    echo "Running iteration $i..."
    mpirun --oversubscribe -np 256 ./pi_seq.out >> 256.txt
done

echo "All iterations complete."

