#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define INF 9999999

void ptsp(int n, int mask, int pos, int threads, int **scores, int **path, int **graph)
{
    // If pos, mask state is already explored just return
    if (scores[pos][mask] != -1)
    {
        return;
    }

    // If every vertex is visited, just set the score to 0 and return
    if (mask == (1 << n) - 1)
    {
        scores[pos][mask] = 0;
        return;
    }

    int localResult = INF;
    int localPath = -1;

    #pragma omp parallel for num_threads(threads) reduction(min:localResult) shared(scores, path, graph, localPath) schedule(static)
    for (int i = 0; i < n; i++) {
        int shift = (1 << i);
        // Check if i is visited or not, update the mask and fill the valid states.
        if ((mask & shift) == 0) {
            int updated_mask = (mask | shift);
            ptsp(n, updated_mask, i, threads, scores, path, graph);
            int newResult = graph[pos][i] + scores[i][updated_mask];

            if (newResult < localResult) {
                localResult = newResult;
                #pragma omp critical
                {
                    localPath = i;
                }
            }
        }
    }
    // update the scores and next best vertex for pos
    scores[pos][mask] = localResult;
    path[pos][mask] = localPath;
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    int t = atoi(argv[2]);
    omp_set_num_threads(t);
    int m = (1 << n);
    int **scores = (int **)malloc(n * sizeof(int *));
    int **path = (int **)malloc(n * sizeof(int *));
    int **graph = (int **)malloc(n * sizeof(int *));
    #pragma omp parallel for num_threads(t)
    for (int i = 0; i < n; i++)
    {
        scores[i] = (int *)malloc(m * sizeof(int));
        path[i] = (int *)malloc(m * sizeof(int));
        graph[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < m; j++)
        {
            scores[i][j] = -1;
            path[i][j] = -1;
        }
    }

    FILE *file = fopen(argv[3], "r");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(file, "%d", &graph[i][j]);
        }
    }
    ptsp(n, 1, 0, t, scores, path, graph);
    int mask = 1, pos = 0;
    printf("Best path: 0");
    // code to follow next best vertex to go starting from 0.
    for (int i = 1; i < n; i++)
    {
        int id = path[pos][mask];
        printf(" %d", id);
        pos = id;
        mask = mask | (1 << id);
    }
    printf("\nDistance: %d\n", scores[0][1]);
}
