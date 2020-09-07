#run with 4 processes 2 threads each
export OMP_NUM_THREADS=2
mpirun -np 4 ../../build/bin/FlowSolver


