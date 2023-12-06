#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define INF 9999999

int graph[12][12] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int ptsp(int n, int mask, int pos, int threads, int path_length, int *path)
{
    if (mask == (1 << n) - 1)
    {
        return 0;
    }

    int result = INF;

#pragma omp parallel for num_threads(threads) schedule(static) shared(result, path)
    for (int i = 0; i < n; i++)
    {
        int shift = (1 << i);
        if ((mask & shift) == 0)
        {
            int new_path[n];
            for (int j = 0; j < n; j++)
            {
                new_path[j] = path[j];
            }
            new_path[path_length] = i;
            int newResult = graph[pos][i] + ptsp(n, mask | shift, i, threads, 1 + path_length, new_path);
#pragma omp critical
            {
                if (result > newResult)
                {
                    result = newResult;
                    for (int j = 0; j < n; j++)
                    {
                        path[j] = new_path[j];
                    }
                }
            }
        }
    }
    return result;
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    int t = atoi(argv[2]);
    omp_set_num_threads(t);
    FILE *file = fopen(argv[3], "r");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(file, "%d", &graph[i][j]);
        }
    }

    int path[n];
    path[0] = 0;
    int answer = ptsp(n, 1, 0, t, 1, path);
    printf("Best path: 0");
    for (int i = 1; i < n; i++)
    {
        printf(" %d", path[i]);
    }
    printf("\nDistance: %d\n", answer);
}