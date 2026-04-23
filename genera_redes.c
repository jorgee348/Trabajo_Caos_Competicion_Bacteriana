#include "head.h"

void generarErdosRenyi(int n, float p) 
{
    int matriz[n][n];
    int i,j;
    for(i = 0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            matriz[i][j] = 0;
        }
    }

    float random;
    for(i = 0;i<n;i++)
    {
        for(j = 0;j<i; j++)
        {
            random = Random();
            if(random<p)
            {
                matriz[i][j] = 1;
                matriz[j][i] = 1;
            }
        }
    }

    printf("Matriz de adyacencia (Red Erdos-Renyi %d, %.2f):\n", n, p);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

int generaNodo(float pH, float pD)
{
    double random = Random();
    if (random < pH)
    return 0;
    if (random < (pH + pD))
    return 1;
    return 2;
}
void generaRedInicial(int n, float p_hueco, float p_vampiro)
{
    int D[n], P[n], H[n];
    int i;

    for(i = 0;i<n;i++)
    D[i] = P[i] = H[i] = 0;

    for(i = 0;i<n;i++)
    switch (generaNodo(p_hueco,p_vampiro)) 
    {
    case 0:H[i] = 1; break;
    case 1:D[i] = 1; break;
    case 2:P[i] = 1; break;
    }
}

void generar_listas(int **A, int N, Nodo *red) 
{
    int i;
    for (i = 0; i < N; i++) 
    {
        int k = 0;
        for (int j = 0; j < N; j++) {
            if (A[i][j] == 1) k++;
        }
        
        red[i].grado = k;
        red[i].vecinos = (int *)malloc(k * sizeof(int));
        
        int id_vecino = 0;
        for (int j = 0; j < N; j++) {
            if (A[i][j] == 1) {
                red[i].vecinos[id_vecino] = j;
                id_vecino++;
            }
        }
    }
}
