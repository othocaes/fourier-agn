set terminal pdf
set output "%OUTPUTFILE%"
set termopt enhanced

set macros

    # Placement of the a,b,c,d labels in the graphs
    POS = "at graph 0.8,0.92 font 'Times,24'"
    XLABEL = "set xlabel 'Temporal Frequency [days^{-1}]' font 'Times' off 0,1;\
              set format x '10^{%+3T}'"
    YLABEL = "set ylabel 'log_{10} PSD' font 'Times' offset 1,2.5;\
              set format y '%.0t'"

unset title
unset key

set xlabel "Temporal Frequency [days^{-1}]" font "Times,24"
set xrange [0.005:0.620]
set xtics auto font 'Times,16'
set logscale x
set xzeroaxis

set ylabel "Lag [days]" font "Times,24"
set yrange [-2:6]
#set ytics (-1,0,1,2,3,4,5) font 'Times,16'
set ytics auto font 'Times,16'
set ytics add ('' -1.5 1,'' -0.5 1,'' 0.5 1,'' 1.5 1,'' 2.5 1,'' 3.5 1,'' 4.5 1,'' 5.5 1)



# Draw line at origin
#set arrow from 0.005,0 to 0.620,0 nohead lt 3 lc rgb 'black'
#set pointsize 0

set label 1 '%LABEL1%' @POS
plot '%FILE1%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .5 lt rgb "black"