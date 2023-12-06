# Parallel-TSP

./tsm x : creates a graph edge matrix of x*x size <br>
./ptsm_n_factorial #cities #threads edges.txt : compute the best path and the distance to take, Parallelization of O(n!) algorithm <br>
./ptsm #cities #threads edges.txt: does it in O(n2^n) time with bit-masking <br>
