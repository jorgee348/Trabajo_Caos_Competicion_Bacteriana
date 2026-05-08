#include "head.h"

unsigned int irr[256];
unsigned int ir1;
unsigned char ind_ran,ig1,ig2,ig3;

float randomIn(double min, double max) {
    return min + (max - min) * Random();
}

float Random(void)
{
    float r;
    ig1=ind_ran-24;
    ig2=ind_ran-55;
    ig3=ind_ran-61;
    irr[ind_ran]=irr[ig1]+irr[ig2];
    ir1=(irr[ind_ran]^irr[ig3]);
    ind_ran++;
    r=ir1*NormRANu;

    return r;
}

void  ini_ran(int SEMILLA)
{
    int INI,FACTOR,SUM,i;
    srand(SEMILLA);
    INI=SEMILLA;
    FACTOR=67397;
    SUM=7364893;

    for(i=0;i<256;i++)
    {
        INI=(INI*FACTOR+SUM);
        irr[i]=INI;
    }

    ind_ran=ig1=ig2=ig3=0;
}

void barrido_parametros(Red *red, Estado *estado, Estado *estado_aux,
                        Parametros *params, int N, float p_enlace,
                        float p_hueco, float p_depred, int pasos)
{
    system("if not exist Dat_Simulaciones mkdir Dat_Simulaciones");

    FILE *fp = fopen("Dat_Simulaciones/barrido_alpha_mu.dat", "wb"); // wb → saltos de línea limpios
    if (!fp) {
        fprintf(stderr, "Error: no se pudo crear barrido_alpha_mu.dat\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "# alpha\tmu\tP_est\tD_est\tregimen\n");
    fflush(fp);

    float alpha_vals[] = {0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40,
                      0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.80};
    float mu_vals[]    = {0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40,
                      0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.85};
    int n_alpha = 15, n_mu = 15;
    int total = n_alpha * n_mu;
    int actual = 0;

    for (int i = 0; i < n_alpha; i++)
    {
        params->alpha = alpha_vals[i];

        for (int j = 0; j < n_mu; j++)
        {
            params->mu = mu_vals[j];

            // ── Liberar listas de vecinos de forma segura ─────────────────
            for (int k = 0; k < N; k++) {
                if (red->nodos[k].vecinos != NULL) {
                    free(red->nodos[k].vecinos);
                    red->nodos[k].vecinos = NULL;
                }
                red->nodos[k].grado = 0;
            }

            generarErdosRenyi(red, p_enlace);
            generar_listas(red);
            generaRedInicial(red, estado, p_hueco, p_depred);

            for (int t = 0; t < pasos; t++)
                paso_temporal(red, estado, estado_aux, params);

            int nP = 0, nD = 0;
            for (int k = 0; k < N; k++) {
                nP += estado->P[k];
                nD += estado->D[k];
            }

            int regimen = 0;
            if      (nD == 0) regimen = 1;
            else if (nP == 0) regimen = 2;

            fprintf(fp, "%.2f\t%.2f\t%d\t%d\t%d\n",
                    alpha_vals[i], mu_vals[j], nP, nD, regimen);
            fflush(fp);

            // Barra de progreso
            actual++;
            int porcentaje = (actual * 100) / total;
            int bloques    = porcentaje / 5;
            printf("\r[");
            for (int b = 0; b < 20; b++)
                printf("%c", b < bloques ? '#' : '-');
            printf("] %3d%%  alpha=%.2f  mu=%.2f  P=%4d  D=%4d",
                   porcentaje, alpha_vals[i], mu_vals[j], nP, nD);
            fflush(stdout);
        }

        if (i < n_alpha - 1) {
        fprintf(fp, "\n\n");
        fflush(fp);
        }
    }

    fclose(fp);
    printf("\nBarrido completado.\n");
}

void genera_campo(Parametros *params, int N, float p_enlace, int M)
{
    double k_medio  = (N - 1) * p_enlace;
    double alpha_ef = params->alpha * k_medio;
    double beta_ef  = params->beta  * k_medio;

    FILE *fichero = fopen("campo.txt", "w");
    if (!fichero)
    {
        fprintf(stderr, "Error: no se pudo crear campo.txt\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < M; i++)
    {
        double p = (double)i / (M - 1);
        for (int j = 0; j < M; j++)
        {
            double d = (double)j / (M - 1);
            double h = 1.0 - p - d;

            // Fuera del simplex: escribir ceros para mantener rejilla regular
            if (h <= 0.0 || p <= 0.0 || d <= 0.0)
            {
                fprintf(fichero, "%.4f %.4f 0.0000 0.0000 0.0000\n", p, d);
                continue;
            }

            double dd  = beta_ef  * p * d - params->mu * d;
            double dp  = p * (alpha_ef * h - beta_ef * d);
            double mag = sqrt(dd * dd + dp * dp);

            double dd_n = 0.0, dp_n = 0.0;
            if (mag > 0)
            {
                dd_n = dd / mag;
                dp_n = dp / mag;
            }

            fprintf(fichero, "%.4f %.4f %.6f %.6f %.6f\n", p, d, dd_n, dp_n, mag);
        }
        // Línea en blanco entre filas para que gnuplot reconozca la rejilla
        fprintf(fichero, "\n");
    }

    fclose(fichero);
    printf("Campo vectorial generado: campo.txt (%dx%d puntos)\n", M, M);
}

void genera_campo_fichero(Parametros *params, int N, double p_enlace, int M, int k)
{
    char nombre[64];
    snprintf(nombre, sizeof(nombre), "Dat_Simulaciones/campo_k%02d.txt", k);

    double k_medio  = (N - 1) * p_enlace;
    double alpha_ef = params->alpha * k_medio;
    double beta_ef  = params->beta  * k_medio;

    FILE *fichero = fopen(nombre, "w");
    if (!fichero)
    {
        fprintf(stderr, "Error: no se pudo crear %s\n", nombre);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < M; i++)
    {
        double p = (double)i / (M - 1);
        for (int j = 0; j < M; j++)
        {
            double d = (double)j / (M - 1);
            double h = 1.0 - p - d;

            if (h <= 0.0 || p <= 0.0 || d <= 0.0)
            {
                fprintf(fichero, "%.4f %.4f 0.0000 0.0000 0.0000\n", p, d);
                continue;
            }

            double dd  = beta_ef  * p * d - params->mu * d;
            double dp  = p * (alpha_ef * h - beta_ef * d);
            double mag = sqrt(dd * dd + dp * dp);

            double dd_n = 0.0, dp_n = 0.0;
            if (mag > 0) { dd_n = dd / mag; dp_n = dp / mag; }

            fprintf(fichero, "%.4f %.4f %.6f %.6f %.6f\n", p, d, dd_n, dp_n, mag);
        }
        fprintf(fichero, "\n");
    }

    fclose(fichero);
}