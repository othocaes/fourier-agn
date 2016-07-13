#!/usr/local/bin/bash

mkdir -p analyses/tables
mkdir -p analyses/plots


for analysis in analyses/*
do
    # Grab and determine labels of analyses, skip if over the same band.
    ref_band=$(basename $analysis|sed 's@[^≻]*≻\([^≻_]*\)_[^_]*@\1@')
    echo_band=$(basename $analysis|sed 's@\([^≻]*\)≻[^≻_]*_[^_]*@\1@')
    if [[ $ref_band == $echo_band ]]; then continue; fi

    # Prepare files
    echo "Plotting PSD and time lags for $echo_band, referred to ${ref_band}."
    echoPSD_tabfile=analyses/tables/${echo_band}PSD.tab
    refPSD_tabfile=analyses/tables/${ref_band}PSD.tab
    PSD_plotfile=analyses/plots/${echo_band}_≻_${ref_band}_PSD.png
    timelag_plotfile=analyses/plots/${echo_band}_≻_${ref_band}_timelag.png

    # Output curves to temporary files using perl script, move tables to
    # permanent location. This just assumes there are no conflicts.
    scripts/power_lag_plot.pl $analysis
    mv tmp.echoPSD $echoPSD_tabfile
    mv tmp.refPSD $refPSD_tabfile
    mv 

    # Plot PSD and save using gnuplot
    cat scripts/templates/psd_freq.gp|
        sed "s@%TITLE@Power Spectrum for Lightcurves $echo_band & $ref_band@"|
        sed "s@%SUBTITLE@as reported by Fausnaugh et. al, STORM III, 2016@"|
        sed "s@%FILE1@$refPSD_tabfile@"|
        sed "s@%FILE2@$echoPSD_tabfile@"|
        sed "s@%OUTPUTFILE@$PSD_plotfile@" >
            tmp.gp
    gnuplot tmp.gp

    # Plot time lags and save using gnuplot
    cat scripts/templates/timelag_freq.gp|
        sed "s@%TITLE@Time Lag for Lightcurve $echo_band relative to $ref_band@"|
        sed "s@%SUBTITLE@as reported by Fausnaugh et. al, STORM III, 2016@"|
        sed "s@%FILE1@$refPSD_tabfile@"|
        sed "s@%FILE2@$echoPSD_tabfile@"|
        sed "s@%OUTPUTFILE@$PSD_plotfile@" >
            tmp.gp
    gnuplot tmp.gp
done



