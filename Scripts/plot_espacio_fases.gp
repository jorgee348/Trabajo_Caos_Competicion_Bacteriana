# ═══════════════════════════════════════════════════════
#  ESPACIO DE FASES: trayectoria P vs D
#  Usa el fichero de evolución temporal normal
# ═══════════════════════════════════════════════════════

if (!exists("fichero")) {
    print "ERROR: usa -e \"fichero='nombre.txt'\""
    exit
}

beta_val = 0.05
alpha_val = 0.50
mu_val    = 0.85
N_val     = 10000.0

set datafile separator "\t"
set title sprintf("Espacio de fases P vs D\n%s", fichero) font ",11"
set xlabel "Presas (P)"
set ylabel "Depredadores (D)"
set grid
set key top right

# ── Nullclinas campo medio ─────────────────────────────
# dP/dt = 0  →  D = (alpha * N) / (beta * P) * (N - P - D) / N
#            Simplificado: D_nc_P = alpha/beta * (N-P)/N * N/P
# dD/dt = 0  →  P = mu * N / beta  (independiente de D)

P_nc_D = mu_val * N_val / beta_val          # nullclina D: P = cte (línea vertical)
D_nc_P(p) = (alpha_val / beta_val) * (N_val - p) / p   # nullclina P

# Punto fijo
P_fijo = P_nc_D
D_fijo = D_nc_P(P_fijo)

set style line 1  lc rgb "#2196F3" lw 2              # trayectoria
set style line 10 lc rgb "red"     lw 2 dt 2         # nullclina P
set style line 11 lc rgb "orange"  lw 2 dt 3         # nullclina D
set style line 12 lc rgb "black"   pt 7 ps 2.0       # punto fijo

# Rango dinámico
set xrange [0:N_val]
set yrange [0:N_val]

# Flecha indicando dirección temporal
set arrow from first 0,0 to first 1,1 nohead  # placeholder

plot fichero       using 2:3           with lines    ls 1  title "Trayectoria P-D", \
     D_nc_P(x)                         with lines    ls 10 title "Nullclina dP/dt=0", \
     P_nc_D, x                         with lines    ls 11 title "Nullclina dD/dt=0", \
     '+' using (P_fijo):(D_fijo)       with points   ls 12 title sprintf("Punto fijo (%.0f, %.0f)", P_fijo, D_fijo)

pause -1 "Pulsa Enter para cerrar"