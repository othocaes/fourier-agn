# This method assumes y values are in logarithm units already.

set terminal pdf
set output "%OUTPUTFILE"
# set termoption dash
set termopt enhanced

set title "%TITLE\n{/*0.7 %SUBTITLE}" font "Times"
set xlabel "Temporal Frequency [days^{-1}]" font "Times"
set xrange [0.005:0.620]
set xtics font "Times"
set ytics font "Times"

# Use this if you want to show PSD directly on a log axis
# set logscale xy
# set ylabel "PSD" font "Times"
# set yrange [%YMIN:%YMAX]
# plot '%FILE1' using 1:(10**($2+2)):3:4 with xyerrorbars title "%LABEL1", \
# '%FILE2' using 1:(10**($2+2)):3:4 with xyerrorbars title "%LABEL2"

# Use this if you want to show the log value of the PSD
set logscale x
set ylabel "log_{10} PSD" font "Times"
#set yrange [%YMIN:%YMAX]
#set yrange [-5.5:0.5]

set key outside
set key center bottom
set key font "Times"

set yzeroaxis

plot '%FILE1' using 1:2:3:4 with xyerrorbars title "%LABEL1" pt 0 ps 1 lt rgb "green", \
'%FILE2' using 1:2:3:4 with xyerrorbars title "%LABEL2" pt 0 ps 1 lt rgb "red"
