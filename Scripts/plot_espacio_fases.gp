if (!exists("fichero"))   { print "ERROR: falta fichero";   exit }
if (!exists("N_val"))     { N_val     = 1000  }
if (!exists("alpha_val")) { alpha_val = 0.20  }
if (!exists("beta_val"))  { beta_val  = 0.05  }
if (!exists("mu_val"))    { mu_val    = 0.85  }

set datafile separator "\t"
set datafile commentschars "#"

# ── Nullclinas campo medio ─────────────────────────────
D_nc_P(p) = (alpha_val / beta_val) * (N_val - p) / p
P_nc_D    = (mu_val * N_val) / beta_val

# ── Punto fijo ─────────────────────────────────────────
if (exists("P_fijo_num") && exists("D_fijo_num")) {
    P_fijo = P_fijo_num
    D_fijo = D_fijo_num
} else {
    P_fijo = P_nc_D
    D_fijo = D_nc_P(P_fijo)
}

# ── Nombre fichero de salida ───────────────────────────
prefijo     = "Dat_Simulaciones/"
nombre_base = fichero[strlen(prefijo)+1 : strlen(fichero)-4]
fichero_out = "plots/Plot_fases_" . nombre_base . ".png"

# ── Estilos ────────────────────────────────────────────
set style line 1  lc rgb "#2196F3" lw 2
set style line 10 lc rgb "#E53935" lw 2 dt 2
set style line 11 lc rgb "#FF9800" lw 2 dt 3
set style line 12 lc rgb "black"   pt 7 ps 1.5

# ══════════════════════════════════════════════════════
#  BLOQUE 1 — Guardar PNG
# ══════════════════════════════════════════════════════
set terminal pngcairo size 900,700 enhanced font "Arial,11"
set output fichero_out

set title sprintf("Espacio de fases\nalpha=%.2f  beta=%.2f  mu=%.2f  N=%d",\
                  alpha_val, beta_val, mu_val, N_val) font ",12"
set xlabel "Presas (P)"
set ylabel "Depredadores (D)"
set grid
set key top right
set xrange [0:N_val]
set yrange [0:N_val]

# Nullclina dD/dt=0 como flecha vertical
set arrow 1 from P_nc_D, 0 to P_nc_D, N_val nohead ls 11

plot fichero  using 2:3          with lines  ls 1  title "Trayectoria P-D",     \
     D_nc_P(x)                   with lines  ls 10 title "Nullclina dP/dt = 0", \
     '+' using (P_fijo):(D_fijo) with points ls 12 title sprintf("Punto fijo (%.0f, %.0f)", P_fijo, D_fijo)

set output
print sprintf("Plot guardado en: %s", fichero_out)

# ══════════════════════════════════════════════════════
#  BLOQUE 2 — Ventana interactiva
# ══════════════════════════════════════════════════════
unset arrow 1
set terminal windows size 900,700

set title sprintf("Espacio de fases\nalpha=%.2f  beta=%.2f  mu=%.2f  N=%d",\
                  alpha_val, beta_val, mu_val, N_val) font ",12"
set xlabel "Presas (P)"
set ylabel "Depredadores (D)"
set grid
set key top right
set xrange [0:N_val]
set yrange [0:N_val]

set arrow 1 from P_nc_D, 0 to P_nc_D, N_val nohead ls 11

plot fichero  using 2:3          with lines  ls 1  title "Trayectoria P-D",     \
     D_nc_P(x)                   with lines  ls 10 title "Nullclina dP/dt = 0", \
     '+' using (P_fijo):(D_fijo) with points ls 12 title sprintf("Punto fijo (%.0f, %.0f)", P_fijo, D_fijo)

pause -1 "Pulsa Enter para cerrar"