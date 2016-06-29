


# This method assumes data is in logarithm units already.

plot 'tables/PSD_lcdrive.dat' using 1:(10**($2+2)):3:4 with xyerrorbars, \
'tables/PSD_lc1.dat' using 1:(10**($2+2)):3:4 with xyerrorbars, \
'tables/PSD_lc2.dat' using 1:(10**($2+2)):3:4 with xyerrorbars
#plot 'tmp.timelag' using 1:2:3:4 with xyerrorbars
set logscale xy
set xrange [0.005:0.603]
set yrange [0.0005:100]
# set yrange [:1]
#    (x, y, ydelta),
#    (x, y, ylow, yhigh),
#    (x, y, xdelta),
#    (x, y, xlow, xhigh),
#    (x, y, xdelta, ydelta), or
#    (x, y, xlow, xhigh, ylow, yhigh).
 pause -1
