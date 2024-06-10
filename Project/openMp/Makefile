CC = g++
CFLAGS = -O2 -fopenmp

SRCS = main.cpp
EXEC = main.out

run: $(EXEC)
	srun -n 1 ./$(EXEC)

$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) -DNUM_THREADS=$(OMP_NUM_THREADS) $(SRCS) -o $(EXEC)

clean:
	rm -f $(EXEC)

