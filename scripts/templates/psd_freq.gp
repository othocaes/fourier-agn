# This method assumes y values are in logarithm units already.

set terminal png
set output "%OUTPUTFILE"

set title "%TITLE\n{/*0.7 %SUBTITLE}"
set xlabel "Temporal Frequency [days^{-1}]"
set xrange [0.005:0.603]

# Use this if you want to show PSD directly on a log axis
# set logscale xy
# set ylabel "PSD"
# set yrange [%YMIN:%YMAX]
# plot '%FILE1' using 1:(10**($2+2)):3:4 with xyerrorbars title "%LABEL1", \
# '%FILE2' using 1:(10**($2+2)):3:4 with xyerrorbars title "%LABEL2"

# Use this if you want to show the log value of the PSD
set logscale x
set ylabel "log_{10} PSD"
#set yrange [%YMIN:%YMAX]
set yrange [-5.5:0.5]
plot '%FILE1' using 1:2:3:4 with xyerrorbars title "%LABEL1", \
'%FILE2' using 1:2:3:4 with xyerrorbars title "%LABEL2"
