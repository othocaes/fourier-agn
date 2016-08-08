#!/usr/bin/env bash

timestep="0.1"
newdir="data/STORM_III/lightcurves/Δt＝${timestep}"
mkdir -p $newdir

for file in data/STORM_III/raw/separated/*
do
    newfile=$(basename $file|sed 's@\(.*\)\.tab@\1.lc@')
    scripts/heasoft_to_psdlag.pl < $file > ${newdir}/${newfile}
    echo Processed $file
done