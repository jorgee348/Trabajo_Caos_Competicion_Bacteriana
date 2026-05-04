#include "head.h"
/*
 * Simula UN paso temporal del sistema bacteria-depredador.
 *
 * Parámetros:
 *   red    - Array de Nodos con la estructura de la red
 *   P      - Vector estado actual de Presas   (lectura)
 *   D      - Vector estado actual de Depredadores (lectura)
 *   H      - Vector estado actual de Huecos   (lectura)
 *   P_new  - Vector estado futuro de Presas   (escritura)
 *   D_new  - Vector estado futuro de Depredadores (escritura)
 *   H_new  - Vector estado futuro de Huecos   (escritura)
 *   N      - Número total de nodos
 */

void paso_temporal(Red *red, Estado *estado, Estado *estado_aux, Parametros *params)
{
    int N = red->N;

    // ── 1. Copiar estado actual en el buffer auxiliar ─────────────────────
    for (int i = 0; i < N; i++)
    {
        estado_aux->P[i] = estado->P[i];
        estado_aux->D[i] = estado->D[i];
        estado_aux->H[i] = estado->H[i];
    }

    // ── 2. Aplicar reglas ────────────────────────────────────────────────
    for (int i = 0; i < N; i++)
    {
        // Regla 3: Depredador muere → Hueco  (D --mu--> H)
        if (estado->D[i] == 1)
        {
            if (Random() < params->mu)
            {
                estado_aux->D[i] = 0;
                estado_aux->H[i] = 1;
            }
        }

        // Regla 1: Presa atacada por depredador vecino (P+D --beta--> 2D)
        else if (estado->P[i] == 1)
        {
            // Contar cuántos vecinos son depredadores
            int n_dep = 0;
            for (int k = 0; k < red->nodos[i].grado; k++)
            {
                int j = red->nodos[i].vecinos[k];
                if (estado->D[j] == 1) n_dep++;
            }

            // Probabilidad de que AL MENOS un depredador infecte
            // P(infección) = 1 - (1-beta)^n_dep
            if (n_dep > 0)
            {
                double p_infeccion = 1.0 - pow(1.0 - params->beta, n_dep);
                if (Random() < p_infeccion)
                {
                    estado_aux->P[i] = 0;
                    estado_aux->D[i] = 1;
                }
            }
        }

        // Regla 2: Hueco colonizado por presa vecina (P+H --alpha--> 2P)
        else if (estado->H[i] == 1)
        {
            // Contar cuántas presas vecinas hay
            int n_pre = 0;
            for (int k = 0; k < red->nodos[i].grado; k++)
            {
                int j = red->nodos[i].vecinos[k];
                if (estado->P[j] == 1) n_pre++;
            }

            // Probabilidad de que AL MENOS una presa colonice
            // P(colonización) = 1 - (1-alpha)^n_pre
            if (n_pre > 0)
            {
                double p_colonizacion = 1.0 - pow(1.0 - params->alpha, n_pre);
                if (Random() < p_colonizacion)
                {
                    estado_aux->H[i] = 0;
                    estado_aux->P[i] = 1;
                }
            }
        }
    }

    // ── 3. Volcar el buffer auxiliar sobre el estado actual ───────────────
    for (int i = 0; i < N; i++)
    {
        estado->P[i] = estado_aux->P[i];
        estado->D[i] = estado_aux->D[i];
        estado->H[i] = estado_aux->H[i];
    }
}
    