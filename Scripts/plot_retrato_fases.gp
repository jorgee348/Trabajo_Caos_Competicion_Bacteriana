# Scripts/plot_retrato_fases.gp

set terminal pngcairo enhanced font "Arial,13" size 900,900
set output "Plots/retrato_fases.png"

# ── Parámetros derivados ─────────────────────────────────────────────────────
k_medio  = (N_val - 1) * p_enlace
beta_ef  = beta_val  * k_medio
alpha_ef = alpha_val * k_medio
p_star   = mu_val / beta_ef
d_star   = alpha_ef * (1.0 - p_star) / (alpha_ef + beta_ef)

# ── Ejes ─────────────────────────────────────────────────────────────────────
set xlabel "Fraccion de Presas (p)"       font "Arial,13"
set ylabel "Fraccion de Depredadores (d)" font "Arial,13"
set title  sprintf("Retrato de fases  a=%.2f  b=%.2f  mu=%.2f  <k>=%.1f", \
                    alpha_val, beta_val, mu_val, k_medio) font "Arial,14"

set xrange [0:1]
set yrange [0:1]
set size square
set key top right

# ── Paleta primaveral ─────────────────────────────────────────────────────────
set palette defined (\
    0 "#fff7fb", \
    1 "#ece2f0", \
    2 "#d0d1e6", \
    3 "#a6bddb", \
    4 "#67a9cf", \
    5 "#3690c0", \
    6 "#02818a", \
    7 "#016450")
set cbrange [0:1]
set colorbox vertical user origin 0.87, 0.15 size 0.025, 0.7
set cblabel "|| (d., p.) ||" font "Arial,11"

# ── Nulclinas ────────────────────────────────────────────────────────────────
# Nulclina d.=0: linea vertical en p = p_star
set arrow 1 from p_star, 0.0 to p_star, 1.0 \
    nohead lw 2.5 lc rgb "#cc00cc" front

# Nulclina p.=0: d = alpha_ef*(1-p)/(alpha_ef+beta_ef)
nulclina_p(x) = (x >= 0 && x <= 1) ? \
    alpha_ef * (1.0 - x) / (alpha_ef + beta_ef) : 1/0

# ── Etiquetas ─────────────────────────────────────────────────────────────────
set label 1 "d.=0" at p_star+0.02, 0.90 \
    font "Arial,11" tc rgb "#cc00cc" front
set label 2 "p.=0" at 0.05, nulclina_p(0.05)+0.03 \
    font "Arial,11" tc rgb "#ff6600" front
set label 3 sprintf("E*(%.2f, %.2f)", p_star, d_star) \
    at p_star+0.03, d_star+0.03 \
    font "Arial,12" tc rgb "red" front

# ── Escala de flechas ────────────────────────────────────────────────────────
scale = 0.015

# ── Plot ─────────────────────────────────────────────────────────────────────
plot \
    "campo.txt" using 1:2:5 \
        with image pixels notitle, \
    "campo.txt" using 1:2:($3*scale):($4*scale) \
        with vectors head filled \
        size 0.005,15 lc rgb "black" lw 0.6 notitle, \
    nulclina_p(x) \
        with lines lw 2.5 lc rgb "#ff6600" title "p.=0", \
    "+" using (p_star):(d_star) \
        with points pt 7 ps 2.5 lc rgb "red" notitle, \
    "+" using (p_star):(d_star) \
        with points pt 7 ps 1.2 lc rgb "white" notitle