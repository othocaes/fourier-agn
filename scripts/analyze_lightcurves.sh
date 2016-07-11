#!/usr/local/bin/bash

for curvefile in data/STORM_III/lightcurves/Δt≔0.01/*
do 
    echo Running psdlag for $curvefile.
    newfilename=analyses/HST1367∠$(basename $curvefile|sed 's@\(.*\)\.lc@\1@')_$(basename $(dirname $curvefile))
    inputfile.tmp >> 2
    inputfile.tmp >> data/STORM_III/lightcurves/HST1367.lc 0
    inputfile.tmp >> $curvefile 0
    inputfile.tmp >> 8 0.005 0.019 0.0425 0.07 0.11 0.17 0.24 0.4 0.603
    inputfile.tmp >> 0
    inputfile.tmp >> 1 1 1 0.49 0.42 0.30 0.36 1.2 -0.60 -0.89 -0.72 2.39 -0.74 -1.5 -1 -3.1 -1.3 -2.3 -2 -1.9 -1.6 -2.6 -2 -1.3 -1.8 -2.6 -1.2 0 -2 -2.7 -1.3 0.63
    inputfile.tmp >> 0:0 0
    inputfile.tmp >> 0
    inputfile.tmp >> 0
    inputfile.tmp >> 100 50 50 mcmc.dat
    bin/psdlag inputfile.tmp >> $newfilename
done