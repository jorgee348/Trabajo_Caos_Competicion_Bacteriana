/*#include "head.h"

#define N_NODOS   10000
#define P_ENLACE  0.1f
#define P_HUECO   0.3f
#define P_DEPRED  0.2f
#define PASOS     10000
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
    Estado *estado = crear_estado(N_NODOS);
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
}*/
/*
#include <stdio.h>
#include <stdlib.h>
#include "head.h"

#define N_NODOS   10000
#define P_ENLACE  0.3f //En 0.003 más o menos se estabiliza en muy pocos depredadores pero no se llegan a extinguir
#define P_HUECO   0.3f //La primera era 0.3
#define P_DEPRED  0.2f
#define PASOS     50
#define SEMILLA   42

void guardar_estado(FILE *fichero, int t, Estado *estado, int N);
void imprimir_estado(int t, Estado *estado, int N);

int main(void)
{
    int seed;
    srand(time(NULL));
    seed = rand();
    ini_ran(seed);

    // 2. Crear y rellenar la red
    Red *red = crear_red(N_NODOS);
    generarErdosRenyi(red, P_ENLACE);
    generar_listas(red);

    // 3. Condiciones iniciales
    Estado *estado     = crear_estado(N_NODOS);
    Estado *estado_aux = crear_estado(N_NODOS);
    generaRedInicial(red, estado, P_HUECO, P_DEPRED);

    // 4. Parámetros de la dinámica
    //Parametros params = { .alpha = 0.3, .beta = 0.5, .mu = 0.1 }; Son los que estaban de primeras
    Parametros params = { .alpha = 0.2, .beta = 0.05, .mu = 0.85 }; //Para ir probando
// 5. Abrir fichero
    
    char nombre[256];
    genera_nombre(nombre, &params, N_NODOS, P_ENLACE, P_HUECO, P_DEPRED, PASOS);
    FILE *fichero = crea_fichero(nombre);
// 6. Bucle de simulación
    for (int t = 0; t < PASOS; t++)
    {
        actualiza_fichero(fichero, t, estado, N_NODOS);
        paso_temporal(red, estado, estado_aux, &params);
    }
    
    // 7. Cerrar fichero y liberar memoria
    cierra_fichero(fichero);
    liberar_red(red);
    liberar_estado(estado);
    liberar_estado(estado_aux);

    // 8. Lanza Gnuplot
    char comando[512];
    snprintf(comando, sizeof(comando),
    "\"\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" "
    "-e \"fichero='%s'\" "
    "scripts/plot_evolucion.gp\"",
    nombre);

    system(comando);

    return 0;
}
*/
#include <stdio.h>
#include <stdlib.h>
#include "head.h"

#define N_NODOS   1000
#define P_ENLACE  0.01f
#define P_HUECO   0.5f
#define P_DEPRED  0.4f
#define PASOS     500
#define SEMILLA   42

int main(void)
{
    // ── 1. Semilla ────────────────────────────────────────────────────────
    int seed;
    srand(time(NULL));
    seed = rand();
    ini_ran(seed);

    // ── 2. Elegir modo ────────────────────────────────────────────────────
    int modo;
    printf("=== SIMULADOR RED BACTERIAS ===\n");
    printf("  1. Simulacion con parametros fijos\n");
    printf("  2. Barrido de parametros (alpha x mu)\n");
    printf("  3. Trayectoria en el espacio de fases (P vs D)\n");
    printf("  4. Retrato de fases (P vs D)\n");
    printf("Selecciona modo [1/2/3/4]: ");
    scanf("%d", &modo);


    // ── 3. Crear estructuras comunes ──────────────────────────────────────
    Red    *red        = crear_red(N_NODOS);
    Estado *estado     = crear_estado(N_NODOS);
    Estado *estado_aux = crear_estado(N_NODOS);
    Parametros params = { .alpha = 0.4, .beta = 0.3, .mu = 0.2 };

    // ── MODO 1: Simulación con parámetros fijos ───────────────────────────
    if (modo == 1)
    {
        generarErdosRenyi(red, P_ENLACE);
        generar_listas(red);
        generaRedInicial(red, estado, P_HUECO, P_DEPRED);

        char nombre[256];
        genera_nombre(nombre, &params, N_NODOS, P_ENLACE, P_HUECO, P_DEPRED, PASOS);
        FILE *fichero = crea_fichero(nombre);

        for (int t = 0; t < PASOS; t++)
        {
            actualiza_fichero(fichero, t, estado, N_NODOS);
            paso_temporal(red, estado, estado_aux, &params);
        }
        // Guardar el último paso
        actualiza_fichero(fichero, PASOS, estado, N_NODOS);
        cierra_fichero(fichero);

        printf("Simulacion completada. Datos en: %s\n", nombre);

        // Crear carpeta plots y lanzar gnuplot
        system("if not exist plots mkdir plots");
        // En modo 1:
        char comando[512];
        snprintf(comando, sizeof(comando),
            "start gnuplot -e \"fichero='%s'\" scripts/plot_evolucion.gp",
            nombre);
        system(comando);
    }

    // ── MODO 2: Barrido de parámetros ─────────────────────────────────────
    else if (modo == 2)
    {
        // beta se mantiene fija durante el barrido, solo varían alpha y mu
        printf("Beta fija en el barrido: %.2f\n", params.beta);
        printf("Iniciando barrido alpha x mu (%d pasos por simulacion)...\n", PASOS);

        barrido_parametros(red, estado, estado_aux, &params,
                           N_NODOS, P_ENLACE, P_HUECO, P_DEPRED, PASOS);

        system("if not exist plots mkdir plots");
        system("start gnuplot scripts/plot_mapa_calor.gp");
    }

    // ── MODO 3: Espacio de fases ──────────────────────────────────────────
else if (modo == 3)
{
    generarErdosRenyi(red, P_ENLACE);
    generar_listas(red);
    generaRedInicial(red, estado, P_HUECO, P_DEPRED);

    int pasos_fases = 300;

    char nombre[256];
    genera_nombre(nombre, &params, N_NODOS, P_ENLACE, P_HUECO, P_DEPRED, pasos_fases);
    FILE *fichero = crea_fichero(nombre);

    for (int t = 0; t < pasos_fases; t++)
    {
        actualiza_fichero(fichero, t, estado, N_NODOS);
        paso_temporal(red, estado, estado_aux, &params);
    }
    actualiza_fichero(fichero, pasos_fases, estado, N_NODOS);
    cierra_fichero(fichero);

    printf("Simulacion completada. Datos en: %s\n", nombre);

    system("if not exist plots mkdir plots");
    char comando[512];
    snprintf(comando, sizeof(comando),
        "start gnuplot "
        "-e \"fichero='%s'\" "
        "-e \"N_val=%d\" "
        "-e \"alpha_val=%g\" -e \"beta_val=%g\" "
        "-e \"mu_val=%g\" -e \"p_enlace=%g\" "
        "scripts/plot_espacio_fases.gp",
        nombre, N_NODOS,
        (double)params.alpha, (double)params.beta,
        (double)params.mu, (double)P_ENLACE);
    system(comando);
}

else if (modo == 4)
{
    generarErdosRenyi(red, P_ENLACE);
    generar_listas(red);

    int M = 30;   // Resolución del campo vectorial

    // Generar campo vectorial
    printf("Generando campo vectorial...\n");
    genera_campo(&params, N_NODOS, P_ENLACE, M);

    // Lanzar gnuplot
    system("if not exist Plots mkdir Plots");
    char comando[512];
    snprintf(comando, sizeof(comando),
        "start gnuplot "
        "-e \"N_val=%d\" "
        "-e \"alpha_val=%g\" -e \"beta_val=%g\" "
        "-e \"mu_val=%g\" -e \"p_enlace=%g\" "
        "Scripts/plot_retrato_fases.gp",
        N_NODOS,
        (double)params.alpha, (double)params.beta,
        (double)params.mu,    (double)P_ENLACE);
    system(comando);

    printf("Retrato de fases completado.\n");
}
else
{
    printf("Modo no valido. Elige 1, 2 o 3.\n");
}

// ── 4. Liberar memoria ────────────────────────────────────────────────
liberar_red(red);
liberar_estado(estado);
liberar_estado(estado_aux);

return 0;
}