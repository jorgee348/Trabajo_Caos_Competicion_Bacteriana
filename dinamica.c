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

void paso_temporal(Nodo *red, int *P,   int *D,   int *H, int *P_new, int *D_new, int *H_new, int N)
{
    // ── 1. Copiar el estado actual en los vectores futuros ──────────────────
    // Punto de partida: el futuro es igual al presente,
    // y sólo modificamos lo que cambia.
    for (int i = 0; i < N; i++) {
        P_new[i] = P[i];
        D_new[i] = D[i];
        H_new[i] = H[i];
    }

    // ── 2. Aplicar las reglas sobre cada nodo ───────────────────────────────
    for (int i = 0; i < N; i++) 
    {

        // ── Regla 3: Depredador muere → Hueco  (D --mu--> H) ───────────────
        // Se evalúa primero para no mezclar lógica con las demás reglas.
        if (D[i] == 1)
        {
            if (Random() < mu) 
            {
                D_new[i] = 0;
                H_new[i] = 1;
            }
            // Si no muere, D_new[i] ya vale 1 por la copia inicial
        }

        // ── Regla 1: Presa atacada por vecino depredador (P+D --beta--> 2D) ─
        else if (P[i] == 1) 
        {
            // Recorremos los vecinos buscando algún depredador
            for (int k = 0; k < red[i].grado; k++) 
            {
                int j = red[i].vecinos[k];   // ID del vecino k

                if (D[j] == 1) 
                {             // Vecino es depredador (estado PASADO)
                    if (Random() < beta) 
                    {
                        P_new[i] = 0;
                        D_new[i] = 1;
                        break;               // Un único evento por paso temporal
                    }
                }
            }
        }

        // ── Regla 2: Hueco colonizado por vecino presa (P+H --alpha--> 2P) ──
        else if (H[i] == 1) 
        {
            // Recorremos los vecinos buscando alguna presa
            for (int k = 0; k < red[i].grado; k++) 
            {
                int j = red[i].vecinos[k];   // ID del vecino k

                if (P[j] == 1) 
                {             // Vecino es presa (estado PASADO)
                    if (Random() < alpha) 
                    {
                        H_new[i] = 0;
                        P_new[i] = 1;
                        break;               // Un único evento por paso temporal
                    }
                }
            }
        }
    }

    // ── 3. Actualizar el estado actual con el futuro ────────────────────────
    for (int i = 0; i < N; i++) 
    {
        P[i] = P_new[i];
        D[i] = D_new[i];
        H[i] = H_new[i];
    }
}
    