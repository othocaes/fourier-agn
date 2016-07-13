set terminal png
set termoption dash
set output "%OUTPUTFILE"


plot '%FILE1' using 1:2:3:4 with xyerrorbars\
set logscale x
set xrange [0.005:0.603]
set arrow from 0.005,0 to 0.603,0 nohead lt 3 lc rgb 'black'
