

# plot 'file' with errorbars
# plot 'file' using 1:2:(sqrt($1)) with xerrorbars
# plot 'file' using 1:2:($1-$3):($1+$3):4:5 with xyerrorbars
set terminal png
set output "tmp.png"

set title "Yo"
set xlabel "Temporal Frequency [1/days]"
set ylabel "log_{10} PSD"
set logscale x
set xrange [0.005:0.603]
set yrange [-5.5:0.5]

plot 'tmp.sourcePSD' using 1:($2):3:4 with xyerrorbars title "Driving Emission", \
'tmp.reprocPSD' using 1:($2):3:4 with xyerrorbars title "Reprocessed Emission",
#'tmp.ccPSD' using 1:($2):3:4 with xyerrorbars
#plot 'tmp.timelag' using 1:2:3:4 with xyerrorbars
# set yrange [:1]
#    (x, y, ydelta),
#    (x, y, ylow, yhigh),
#    (x, y, xdelta),
#    (x, y, xlow, xhigh),
#    (x, y, xdelta, ydelta), or
#    (x, y, xlow, xhigh, ylow, yhigh).
 pause -1
