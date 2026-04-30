#ifndef HEAD_H
#define HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NormRANu (2.3283063671E-10F)

extern unsigned int irr[256];
extern unsigned int ir1;
extern unsigned char ind_ran,ig1,ig2,ig3;

extern double alpha;  // Probabilidad de reproducción de presas
extern double beta;   // Probabilidad de infección por depredadores
extern double mu;     // Probabilidad de muerte de depredadores

extern float Random(void);
extern void ini_ran(int SEMILLA);
extern float randomIn(double min, double max);


/* ── Typedefs de conveniencia ── */
typedef unsigned int  uint;
typedef unsigned char uchar;
typedef struct 
{
    int *vecinos;  // Arreglo con los IDs de los vecinos
    int grado;     // Cuántos vecinos tiene este nodo
} Nodo;

typedef struct
{
    int *P;   // 1 si el nodo i es Presa,       0 si no
    int *D;   // 1 si el nodo i es Depredador,   0 si no
    int *H;   // 1 si el nodo i es Hueco,        0 si no
} Estado;

typedef struct 
{
    int    N;      // Número de nodos
    int  **A;      // Matriz de adyacencia (N x N)
    Nodo  *nodos;  // Lista de vecinos de cada nodo
} Red;

typedef struct 
{
    double alpha;  // Probabilidad de reproducción de presas
    double beta;   // Probabilidad de infección por depredadores
    double mu;     // Probabilidad de muerte de depredadores
} Parametros;

/* ── Prototipos de tus funciones (añádelos aquí) ── */
/* void mi_funcion(int param); */
void  ini_ran(int SEMILLA); // Generador de Parisi-Rapuano
float Random(void); // Genera un número aleatorio entre (0,1)
float randomIn(double min, double max); // Genera un número aleatorio entre dos valores [a,b]
//void generarErdosRenyi(int n, float p); // Genera la matriz de adyacencia de una red de Erdos-Renyi
int generaNodo(float pH, float pD); // Determina el estado inicial de un nodo de la red
//void generaRedInicial(int n, float p_hueco, float p_vampiro); // Genera las condiciones iniciales de la red
//void generar_listas(int **A, int N, Nodo *red); // A partir de la matriz A, genera las listas de vecinos
//void paso_temporal(Nodo *red, int *P,   int *D,   int *H, int *P_new, int *D_new, int *H_new, int N);


Red *crear_red(int N); //
void liberar_red(Red *red);
void generarErdosRenyi(Red *red, float p);
void generar_listas(Red *red);
Estado *crear_estado(int N);
void liberar_estado(Estado *estado);
void generaRedInicial(Red *red, Estado *estado, float p_hueco, float p_depredador);
void paso_temporal(Red *red, Estado *estado, Estado *estado_aux, Parametros *params);
void imprimir_estado(int t, Estado *estado, int N);
FILE *crea_fichero(const char *nombre);
void  actualiza_fichero(FILE *fichero, int t, Estado *estado, int N);
void  cierra_fichero(FILE *fichero);


#endif /* HEAD_H */