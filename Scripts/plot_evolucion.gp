if (!exists("fichero")) {
    print "ERROR: debes pasar el nombre del fichero con -e \"fichero='nombre.txt'\""
    exit
}

# ── Construir nombre del plot de salida ───────────────────────
# fichero = "Dat_Simulaciones/N100_pe0.30_..._t1000.txt"
# Extraemos solo el nombre sin carpeta ni extensión para el PNG
nombre_completo = fichero[18:]                        # elimina "Dat_Simulaciones/"
nombre_base     = nombre_completo[:strlen(nombre_completo)-4]  # elimina ".txt"
fichero_out     = "plots/Plot_" . nombre_base . ".png"

# ── Guardar en PNG ────────────────────────────────────────────
set terminal pngcairo size 1200,600 enhanced font "Arial,11"
set output fichero_out

set title sprintf("Evolución de la red\n%s", nombre_base) font ",12"
set xlabel "Paso de simulación"
set ylabel "Número de nodos"
set grid
set key top right
set datafile separator "\t"
set key autotitle columnhead

set style line 1 lc rgb "#2196F3" lw 2
set style line 2 lc rgb "#F44336" lw 2
set style line 3 lc rgb "#9E9E9E" lw 2

plot fichero using 1:2 with lines ls 1, \
     fichero using 1:3 with lines ls 2, \
     fichero using 1:4 with lines ls 3

# ── Cerrar el PNG y abrir ventana interactiva ─────────────────
set output
print sprintf("Plot guardado en: %s", fichero_out)

set terminal wxt size 1200,600 enhanced font "Arial,11"
set output

replot

pause -1 "Pulsa Enter para cerrar"