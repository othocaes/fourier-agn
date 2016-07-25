#!/usr/local/bin/bash

timestep="0.1"
lc_dir="data/STORM_III/lightcurves/Δt＝${timestep}"
ref_band='HST(𝛌＝1367)'
ref_curve="${lc_dir}/${ref_band}.lc"
error_type="2"
# error types:
#   0 for covariance matrix, 1 for likelihood function, 2 for monte carlo

echo Using lightcurves in $lc_dir.

for echo_curve in $lc_dir/*
do 
    echo_band=$(basename $echo_curve|sed 's@\(.*\)\.lc@\1@')
    echo -n $(date "+%R")\: Running psdlag for $echo_band against $ref_band.
    newfilename="analyses/${echo_band}≻${ref_band}_Δt＝${timestep}"
    echo "2" > tmp.psdlagargs
    echo "$ref_curve 0" >> tmp.psdlagargs
    echo "$echo_curve 0" >> tmp.psdlagargs
    echo "8 0.005 0.019 0.0425 0.07 0.11 0.17 0.24 0.4 0.603" >> tmp.psdlagargs
    echo "0" >> tmp.psdlagargs
    echo ".9999 .9999 .9999 0.49 0.42 0.30 0.36 1.2 -0.60 -0.89 -0.72 2.39 -0.74 -1.5 -1 -3.1 -1.3 -2.3 -2 -1.9 -1.6 -2.6 -2 -1.3 -1.8 -2.6 -1.2 0 -2 -2.7 -1.3 0.63" >> tmp.psdlagargs
    echo "0:0 0" >> tmp.psdlagargs
    echo $error_type >> tmp.psdlagargs
    echo "0" >> tmp.psdlagargs
    echo "100 50 50 mcmc.dat" >> tmp.psdlagargs
    time bin/psdlag tmp.psdlagargs >> $newfilename
    echo ""
done