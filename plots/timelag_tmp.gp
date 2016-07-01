

# plot 'file' with errorbars
# plot 'file' using 1:2:(sqrt($1)) with xerrorbars
# plot 'file' using 1:2:($1-$3):($1+$3):4:5 with xyerrorbars

plot 'tmp.timelag' using 1:2:3:4 with xyerrorbars
#plot 'tmp.timelag' using 1:2:3:4 with xyerrorbars
set logscale x
set xrange [0.005:0.603]
set arrow from 0.005,0 to 0.603,0 nohead lt 3 lc rgb 'black'
# set yrange [:1]
#    (x, y, ydelta),
#    (x, y, ylow, yhigh),
#    (x, y, xdelta),
#    (x, y, xlow, xhigh),
#    (x, y, xdelta, ydelta), or
#    (x, y, xlow, xhigh, ylow, yhigh).
 pause -1
