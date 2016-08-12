set terminal pdf
set output "pres/img/tophat_timedomain.pdf"

set xrange [0:25]
set xlabel "Time [days]" font "Times,24"
set xtics font 'Times,16'
set mxtics

set yrange [0:0.5]
set ylabel "Response [n.u.]" font "Times,24"
set ytics font 'Times,16'
set mytics

unset key


plot "analyses/tables/tophat1.tab" using 1:2 with lines lc rgb "red", \
    "analyses/tables/tophat2.tab" using 1:2 with lines lc rgb "green", \
    "analyses/tables/tophat3.tab" using 1:2 with lines lc rgb "blue"