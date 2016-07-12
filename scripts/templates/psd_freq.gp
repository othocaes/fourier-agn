# This method assumes y values are in logarithm units already.

set title "%TITLE"
set terminal png
set output "tmp.png"

set title "Yo"
set xlabel "Temporal Frequency [1/days]"
set xrange [0.005:0.603]

# Use this if you want to show PSD directly on a log axis
# set logscale xy
# set ylabel "PSD"
# set yrange [0.0005:100]
# plot '%FILE1' using 1:(10**($2+2)):3:4 with xyerrorbars, \
# '%FILE2' using 1:(10**($2+2)):3:4 with xyerrorbars

# Use this if you want to show the log value of the PSD
set logscale x
set ylabel "log_{10} PSD"
set yrange [-5.5:0.5]
plot '%FILE1' using 1:2:3:4 with xyerrorbars, \
'%FILE2' using 1:2:3:4 with xyerrorbars