set terminal pdf size 3.5,5
set output "timelag_atlas.pdf"
set termopt enhanced

set macros

    # Placement of the a,b,c,d labels in the graphs
    POS = "at graph 0.61,0.85 font 'Times,12'"
    
    # x- and ytics for each row resp. column
    NOXNUMS = "unset xlabel;\
               set format x ''"
    XNUMS = "unset xlabel;\
             set format x '10^{%+3T}'"
    XLABEL = "set xlabel 'Temporal Frequency [days^{-1}]' font 'Times' off 0,1;\
              set format x '10^{%+3T}'"
    YLABEL = "set ylabel 'Lag [days]' font 'Times' offset 1,2.5;\
              set format y '%.0t'"
    NOYNUMS = "set format y ''; unset ylabel"
    YNUMS = "set format y '%.0t'"
    

    VSET_1 = "set tmargin at screen 0.97; set bmargin at screen 0.825"
    VSET_2 = "set tmargin at screen 0.825; set bmargin at screen 0.68"
    VSET_3 = "set tmargin at screen 0.68; set bmargin at screen 0.535"
    VSET_4 = "set tmargin at screen 0.535; set bmargin at screen 0.39"
    VSET_5 = "set tmargin at screen 0.39; set bmargin at screen 0.245"
    VSET_6 = "set tmargin at screen 0.245; set bmargin at screen 0.10"

    HSET_1 = "set lmargin at screen 0.15; set rmargin at screen 0.42"
    HSET_2 = "set lmargin at screen 0.42; set rmargin at screen 0.69"
    HSET_3 = "set lmargin at screen 0.69; set rmargin at screen 0.96"

unset key
set logscale x
set xtics auto font 'Times,9' offset 0,.5
set ytics (-1,0,1,2,3,4,5) font 'Times,9'
set ytics add ('' -1.5 1,'' -0.5 1,'' 0.5 1,'' 1.5 1,'' 2.5 1,'' 3.5 1,'' 4.5 1,'' 5.5 1)
set xrange [0.005:0.620];
set yrange [-2:6]

# Draw line at origin
set arrow from 0.005,0 to 0.620,0 nohead lt 3 lc rgb 'black'
set pointsize 0

set multiplot layout 6,3 rowsfirst


    # --- GRAPH a
    @VSET_1; @HSET_1
    @NOXNUMS; @YNUMS
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH b
    @NOXNUMS; @NOYNUMS
    @VSET_1; @HSET_2
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    #-- GRAPH c
    @NOXNUMS; @NOYNUMS
    @VSET_1; @HSET_3
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH d
    @NOXNUMS; @YNUMS
    @VSET_2; @HSET_1
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @NOXNUMS; @NOYNUMS
    @VSET_2; @HSET_2
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @NOXNUMS; @NOYNUMS
    @VSET_2; @HSET_3
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @NOXNUMS; @YNUMS
    @VSET_3; @HSET_1
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @NOXNUMS; @NOYNUMS
    @VSET_3; @HSET_2
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @NOXNUMS; @NOYNUMS
    @VSET_3; @HSET_3
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @NOXNUMS; @YLABEL
    @VSET_4; @HSET_1
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @NOXNUMS; @NOYNUMS
    @VSET_4; @HSET_2
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @NOXNUMS; @NOYNUMS
    @VSET_4; @HSET_3
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @NOXNUMS; @YNUMS
    @VSET_5; @HSET_1
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @NOXNUMS; @NOYNUMS
    @VSET_5; @HSET_2
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @NOXNUMS; @NOYNUMS
    @VSET_5; @HSET_3
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @XNUMS; @YNUMS
    @VSET_6; @HSET_1
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @XLABEL; @NOYNUMS
    @VSET_6; @HSET_2
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"
    
    # --- GRAPH a
    @XNUMS; @NOYNUMS
    @VSET_6; @HSET_3
    set label 1 '%LABEL%' @POS
    plot '%FILE%' using 1:2:($2-$4):($2+$4) with yerrorbars pt 7 ps .3 lt rgb "black"


unset multiplot

