#include "head.h"

Red *crear_red(int N)
{
    Red *red = (Red *)malloc(sizeof(Red));
    if (!red) {
        fprintf(stderr, "Error: no se pudo reservar memoria para Red\n");
        exit(EXIT_FAILURE);
    }

    red->N = N;

    // Reservar matriz de adyacencia N x N (inicializada a 0)
    red->A = (int **)malloc(N * sizeof(int *));
    if (!red->A) {
        fprintf(stderr, "Error: no se pudo reservar memoria para A\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) {
        red->A[i] = (int *)calloc(N, sizeof(int));
        if (!red->A[i]) {
            fprintf(stderr, "Error: no se pudo reservar memoria para A[%d]\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Reservar array de nodos (vecinos se asignan después en generar_listas)
    red->nodos = (Nodo *)malloc(N * sizeof(Nodo));
    if (!red->nodos) {
        fprintf(stderr, "Error: no se pudo reservar memoria para nodos\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) {
        red->nodos[i].vecinos = NULL;
        red->nodos[i].grado   = 0;
    }

    return red;
}

void liberar_red(Red *red)
{
    if (!red) return;

    // Liberar filas de la matriz de adyacencia
    for (int i = 0; i < red->N; i++)
        free(red->A[i]);
    free(red->A);

    // Liberar listas de vecinos de cada nodo
    for (int i = 0; i < red->N; i++)
        free(red->nodos[i].vecinos);
    free(red->nodos);

    free(red);
}

Estado *crear_estado(int N)
{
    Estado *estado = (Estado *)malloc(sizeof(Estado));
    if (!estado) {
        fprintf(stderr, "Error: no se pudo reservar memoria para Estado\n");
        exit(EXIT_FAILURE);
    }

    estado->P = (int *)calloc(N, sizeof(int));
    estado->D = (int *)calloc(N, sizeof(int));
    estado->H = (int *)calloc(N, sizeof(int));

    if (!estado->P || !estado->D || !estado->H) {
        fprintf(stderr, "Error: no se pudo reservar memoria para los vectores de estado\n");
        exit(EXIT_FAILURE);
    }

    return estado;
}

void liberar_estado(Estado *estado)
{
    if (!estado) return;

    free(estado->P);
    free(estado->D);
    free(estado->H);
    free(estado);
}

void generarErdosRenyi(Red *red, float p)
{
    int N = red->N;

    // Inicializar matriz a 0
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            red->A[i][j] = 0;

    // Generar enlaces con probabilidad p (matriz simétrica)
    float random;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < i; j++)
        {
            random = Random();
            if (random < p)
            {
                red->A[i][j] = 1;
                red->A[j][i] = 1;
            }
        }
    }

    printf("Matriz de adyacencia (Red Erdos-Renyi %d, %.2f):\n", N, p);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%d ", red->A[i][j]);
        printf("\n");
    }
}

int generaNodo(float pH, float pD)
{
    double random = Random();
    if (random < pH)        return 0;   
    if (random < (pH + pD)) return 1;   
    return 2;                           
}
void generaRedInicial(Red *red, Estado *estado, float p_hueco, float p_depredador)
{
    int N = red->N;

    for (int i = 0; i < N; i++)
        estado->D[i] = estado->P[i] = estado->H[i] = 0;

    for (int i = 0; i < N; i++)
    {
        switch (generaNodo(p_hueco, p_depredador))
        {
            case 0: estado->H[i] = 1; break;  
            case 1: estado->D[i] = 1; break;   
            case 2: estado->P[i] = 1; break;   
        }
    }
}

void generar_listas(Red *red)
{
    int N = red->N;

    for (int i = 0; i < N; i++)
    {
        // 1. Contar el grado del nodo i
        int k = 0;
        for (int j = 0; j < N; j++)
            if (red->A[i][j] == 1) k++;

        red->nodos[i].grado   = k;
        red->nodos[i].vecinos = (int *)malloc(k * sizeof(int));

        // 2. Rellenar la lista de vecinos
        int id_vecino = 0;
        for (int j = 0; j < N; j++)
        {
            if (red->A[i][j] == 1)
            {
                red->nodos[i].vecinos[id_vecino] = j;
                id_vecino++;
            }
        }
    }
}
