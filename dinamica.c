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

    // ── 2. Aplicar reglas leyendo estado y escribiendo en estado_aux ──────
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
            for (int k = 0; k < red->nodos[i].grado; k++)
            {
                int j = red->nodos[i].vecinos[k];

                if (estado->D[j] == 1)          // Vecino depredador (estado PASADO)
                {
                    if (Random() < params->beta)
                    {
                        estado_aux->P[i] = 0;
                        estado_aux->D[i] = 1;
                        break;
                    }
                }
            }
        }

        // Regla 2: Hueco colonizado por presa vecina (P+H --alpha--> 2P)
        else if (estado->H[i] == 1)
        {
            for (int k = 0; k < red->nodos[i].grado; k++)
            {
                int j = red->nodos[i].vecinos[k];

                if (estado->P[j] == 1)          // Vecino presa (estado PASADO)
                {
                    if (Random() < params->alpha)
                    {
                        estado_aux->H[i] = 0;
                        estado_aux->P[i] = 1;
                        break;
                    }
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
    