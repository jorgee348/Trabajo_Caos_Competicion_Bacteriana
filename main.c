#include "head.h"

#define N_NODOS   100
#define P_ENLACE  0.1f
#define P_HUECO   0.3f
#define P_DEPRED  0.2f
#define PASOS     1000
#define SEMILLA   42

void imprimir_estado(int t, Estado *estado, int N);

int main()
{
    // 1. Inicializar el generador aleatorio
    ini_ran(SEMILLA);

    // 2. Crear y rellenar la red
    Red *red = crear_red(N_NODOS);
    generarErdosRenyi(red, P_ENLACE);   // Rellena red->A
    generar_listas(red);                // Construye red->nodos desde red->A

    // 3. Condiciones iniciales
    Estado *estado     = crear_estado(N_NODOS);
    Estado *estado_aux = crear_estado(N_NODOS);   // Buffer para actualización síncrona
    generaRedInicial(red, estado, P_HUECO, P_DEPRED);

    // 4. Parámetros de la dinámica
    Parametros params = { .alpha = 0.3, .beta = 0.5, .mu = 0.1 };

    // 5. Bucle de simulación
    printf("Paso\tPresta\tDepredador\tHueco\n");
    for (int t = 0; t < PASOS; t++) {
        imprimir_estado(t, estado, N_NODOS);
        paso_temporal(red, estado, estado_aux, &params);
    }

    // 6. Liberar memoria
    liberar_red(red);
    liberar_estado(estado);
    liberar_estado(estado_aux);

    return 0;
}

void imprimir_estado(int t, Estado *estado, int N)
{
    int nP = 0, nD = 0, nH = 0;

    for (int i = 0; i < N; i++) {
        nP += estado->P[i];
        nD += estado->D[i];
        nH += estado->H[i];
    }

    printf("%d\t%d\t%d\t%d\n", t, nP, nD, nH);
}