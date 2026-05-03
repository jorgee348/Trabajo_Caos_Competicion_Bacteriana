fichero  = "Dat_Simulaciones/barrido_alpha_mu.dat"

set datafile separator "\t"
set datafile commentschars "#"

# ══════════════════════════════════════════════════════
#  BLOQUE 1 — Guardar PNG
# ══════════════════════════════════════════════════════
fichero_out = "plots/Plot_barrido_alpha_mu.png"
set terminal pngcairo size 1800,700 enhanced font "Arial,11"
set output fichero_out

set multiplot layout 1,2 title "Diagrama de fases - Red depredador-presa" font ",13"

# ─── Panel izquierdo: P estacionario ──────────────────
set title "Poblacion estacionaria de Presa (P)"
set xlabel "alpha"
set ylabel "mu"
set cblabel "Nodos P"
set xrange [0.04:0.81]
set yrange [0.04:0.86]
set cbrange [0:400]
set palette defined (0 "#1565C0", 100 "#42A5F5", 200 "#A5D6A7", 300 "#FF7043", 400 "#B71C1C")

plot fichero using 1:2:3 with image

# ─── Panel derecho: D estacionario ────────────────────
set title "Poblacion estacionaria de Depredador (D)"
set cblabel "Nodos D"
set cbrange [300:950]
set palette defined (300 "#ECEFF1", 500 "#EF9A9A", 700 "#E53935", 950 "#880E4F")

plot fichero using 1:2:4 with image

unset multiplot
set output
print sprintf("Plot guardado en: %s", fichero_out)

# ══════════════════════════════════════════════════════
#  BLOQUE 2 — Ventana interactiva
# ══════════════════════════════════════════════════════
set terminal windows size 1800,700

set multiplot layout 1,2 title "Diagrama de fases - Red depredador-presa" font ",13"

# ─── Panel izquierdo: P estacionario ──────────────────
set title "Poblacion estacionaria de Presa (P)"
set xlabel "alpha"
set ylabel "mu"
set cblabel "Nodos P"
set xrange [0.04:0.81]
set yrange [0.04:0.86]
set cbrange [0:400]
set palette defined (0 "#1565C0", 100 "#42A5F5", 200 "#A5D6A7", 300 "#FF7043", 400 "#B71C1C")

plot fichero using 1:2:3 with image

# ─── Panel derecho: D estacionario ────────────────────
set title "Poblacion estacionaria de Depredador (D)"
set cblabel "Nodos D"
set cbrange [300:950]
set palette defined (300 "#ECEFF1", 500 "#EF9A9A", 700 "#E53935", 950 "#880E4F")

plot fichero using 1:2:4 with image pixels

unset multiplot
pause -1 "Pulsa Enter para cerrar"