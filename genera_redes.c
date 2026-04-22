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
    int estado_inicial[n];
    int i;

    for(i = 0;i<n;i++)
    estado_inicial[i] = 0; //Inicializamos el pulmón vacío

    for(i = 0;i<n;i++)
    estado_inicial[i] = generaNodo(p_hueco,p_vampiro);

    for(i = 0;i<n;i++)
    printf("%d", estado_inicial[i]);
}
