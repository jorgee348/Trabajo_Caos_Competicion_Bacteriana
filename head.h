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

/* ── Prototipos de tus funciones (añádelos aquí) ── */
/* void mi_funcion(int param); */
void  ini_ran(int SEMILLA);
float Random(void);
float randomIn(double min, double max);
void generarErdosRenyi(int n, float p);
int generaNodo(float pH, float pD);
void generaRedInicial(int n, float p_hueco, float p_vampiro);
void generar_listas(int **A, int N, Nodo *red);

#endif /* HEAD_H */