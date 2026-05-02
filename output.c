#include "head.h"

void genera_nombre(char *nombre, Parametros *params, int N, float p_enlace, float p_hueco, float p_depred, int pasos)
{
    snprintf(nombre, 256, "Dat_Simulaciones/N%d_pe%.2f_ph%.2f_pd%.2f_a%.2f_b%.2f_mu%.2f_t%d.txt", N, p_enlace, p_hueco, p_depred, params->alpha, params->beta, params->mu, pasos);
}

FILE *crea_fichero(const char *nombre)
{
    FILE *fichero = fopen(nombre, "w");
    if (!fichero)
    {
        fprintf(stderr, "Error: no se pudo crear el fichero '%s'\n", nombre);
        exit(EXIT_FAILURE);
    }
    fprintf(fichero, "Paso\tPresa\tDepredador\tHueco\n");
    return fichero;
}

void actualiza_fichero(FILE *fichero, int t, Estado *estado, int N)
{
    int nP = 0, nD = 0, nH = 0;

    for (int i = 0; i < N; i++)
    {
        nP += estado->P[i];
        nD += estado->D[i];
        nH += estado->H[i];
    }

    fprintf(fichero, "%d\t%d\t%d\t%d\n", t, nP, nD, nH);
}

void cierra_fichero(FILE *fichero)
{
    if (!fichero) return;
    fclose(fichero);
}