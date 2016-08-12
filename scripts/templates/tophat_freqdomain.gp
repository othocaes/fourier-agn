set terminal pdf
set termopt enhanced
set output "pres/img/tophat_freqdomain.pdf"

set xrange [0.005:0.620]
set logscale x
set xlabel "Temporal Frequency [days^{-1}]" font "Times,24"
set xtics font 'Times,16'
set mxtics

set yrange [-11:11]
set ylabel "Time Delay [days]" font "Times,24"
set ytics font 'Times,16'
set mytics


unset key


plot "analyses/tables/tophat_fft1.tab" using 1:(-1*$2) with lines lc rgb "red", \
    "analyses/tables/tophat_fft2.tab" using 1:(-1*$2) with lines lc rgb "green", \
    "analyses/tables/tophat_fft3.tab" using 1:(-1*$2) with lines lc rgb "blue"