#!/usr/bin/env gnuplot

#
# file.gplot
#
# Copyright (c) 2020 Carlos Braga
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the MIT License.
#
# See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
#

# -----------------------------------------------------------------------------
## graphics output
#set terminal aqua persist size 640,480               enhanced font 'helvetica,14'
#set terminal x11 persist size 640,480 position 0,24 enhanced font 'helvetica,14' linewidth 2.0
set terminal wxt persist size 640,480 position 0,24 enhanced font 'helvetica,14' linewidth 2.0 dashlength 0.1
#set terminal epslatex size 8.0cm,6.0cm color solid colortext rounded font 'Times-Roman,8' standalone header \
#"\\usepackage[utf8]{inputenc}\n \
#\\usepackage[T1]{fontenc}\n \
#\\renewcommand{\\familydefault}{\\rmdefault}\n \
#\\usepackage{mathptmx}\n \
#\\usepackage{helvet}\n \
#\\newcommand{\\ft}[0]{\\footnotesize}"
# set output 'figure.tex'
reset
binwidth = 5
binstart = 0
load 'plot_ent.function'



# -----------------------------------------------------------------------------
## single plot
set size    1.0, 1.0
set origin  0.0, 0.0
set samples 1000

set xrange [0:100.0]; set xtics 10; set mxtics 5
set yrange [0:200.0];  set ytics 10; set mytics 5

set format x '%.1f'; set xlabel '$\mathrm{X}$'
set format y '%.1f'; set ylabel '$\mathrm{\rho(X)}$'

set key at -1.5, 0.4
set key bottom right

unset key

set term wxt 0
set title "kiss32 rng"
show title
plot '/tmp/out.kiss32_plot' @hist ls 1 lw 0.1

set term wxt 1
set title "kiss64 rng"
show title
plot '/tmp/out.kiss64_plot' @hist ls 2 lw 0.1

set term wxt 2
set title "cmwc32 rng"
show title
plot '/tmp/out.cmwc32_plot' @hist ls 3 lw 0.1

set term wxt 3
set title "cmwc64 rng"
show title
plot '/tmp/out.cmwc64_plot' @hist ls 4 lw 0.1
