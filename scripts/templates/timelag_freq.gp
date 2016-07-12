set terminal png
set termoption dash


plot 'tables/timelag_lc1.dat' using 1:2:3:4 with xyerrorbars, \
'tables/timelag_lc2.dat' using 1:2:3:4 with xyerrorbars
set logscale x
set xrange [0.005:0.603]
set arrow from 0.005,0 to 0.603,0 nohead lt 3 lc rgb 'black'
pause -1
