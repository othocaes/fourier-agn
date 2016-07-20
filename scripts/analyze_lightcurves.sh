#!/usr/local/bin/bash

for curvefile in data/STORM_III/lightcurves/Δt＝0.1/*
do 
    echo Running psdlag for $curvefile.
    newfilename="analyses/$(basename $curvefile|sed 's@\(.*\)\.lc@\1@')≻HST1367_$(basename $(dirname $curvefile))"
    echo "2" > tmp.psdlagargs
    echo "data/STORM_III/lightcurves/Δt＝0.1/HST1367.lc 0" >> tmp.psdlagargs
    echo "$curvefile 0" >> tmp.psdlagargs
    echo "8 0.005 0.019 0.0425 0.07 0.11 0.17 0.24 0.4 0.603" >> tmp.psdlagargs
    # covariance matrix errors
    # echo "0" >> tmp.psdlagargs

    # likelihood function errors
    echo "1" >> tmp.psdlagargs

    # monte carlo errors
    # echo "2" >> tmp.psdlagargs

    echo ".9999 .9999 .9999 0.49 0.42 0.30 0.36 1.2 -0.60 -0.89 -0.72 2.39 -0.74 -1.5 -1 -3.1 -1.3 -2.3 -2 -1.9 -1.6 -2.6 -2 -1.3 -1.8 -2.6 -1.2 0 -2 -2.7 -1.3 0.63" >> tmp.psdlagargs
    echo "0:0 0" >> tmp.psdlagargs
    echo "0" >> tmp.psdlagargs
    echo "0" >> tmp.psdlagargs
    echo "100 50 50 mcmc.dat" >> tmp.psdlagargs
    time bin/psdlag tmp.psdlagargs >> $newfilename
done