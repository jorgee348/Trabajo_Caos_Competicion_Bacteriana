#include "head.h"



void generarErdosRenyi(int n, float p) {
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

/*int main() {
    int nodos = 10;
    float probabilidad = 0.1;

    generarErdosRenyi(nodos, probabilidad);

    return 0;
}*/