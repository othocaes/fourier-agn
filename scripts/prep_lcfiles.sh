#!/usr/local/bin/bash

for file in data/STORM_III/raw/separated/*; do scripts/heasoft_to_psdlag.pl < $file > data/STORM_III/lightcurves/Δt≔0.01/$(basename $file|sed 's@\(.*\)\.tab@\1.lc@'); done