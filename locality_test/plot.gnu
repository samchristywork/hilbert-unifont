#!/usr/bin/gnuplot

set terminal svg size 500,400
set output 'data/gnuplot.svg'

set title 'Locality Test (n=128)'
set xlabel "Coordinate in 1-dimension"
set ylabel "Distance from (64, 64)"

#set xrange [7192:9192]

plot "data/line1" title 'Traditional Gradient Representation' with lines, \
     "data/line2" title 'Hilbert Representation' with lines
