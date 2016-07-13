set terminal png
set termoption dash
set output "%OUTPUTFILE"

set title "%TITLE\n{/*0.7 %SUBTITLE}"
set xlabel "Temporal Frequency [days^{-1}]"
set xrange [0.005:0.603]
set logscale x

set ylabel "Lag [days]"
#set yrange [%YMIN:%YMAX]

# Draw line at origin
set arrow from 0.005,0 to 0.603,0 nohead lt 3 lc rgb 'black'


plot '%FILE1' using 1:2:3:4 with xyerrorbars title "%LABEL1"
