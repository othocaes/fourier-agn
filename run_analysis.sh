#!/usr/bin/env bash

# This script is meant to encapsulate all functions in proper order to produce
# the analyses. Parameters that this script should control: Δt, σ type, input
# dataset(lightcurve directory), more?

ref_band=$(cat ref_band)
ref_curve="/lightcurves/${ref_band}.lc"
err_str=$(cat err_type)

mkdir -p analyses/tables

#for analysis in analyses/*
for echo_curve in "lightcurves/*"
do
    # Grab and determine labels of analyses, skip if over the same band.
    echo_band=$(basename $echo_curve|sed 's|\(.*\)\.lc|\1|')
    if [[ $ref_band == $echo_band ]]; then continue; fi

echo "Running analysis for $echo_band ≺ $ref_band"

    python scripts/analyze_lightcurve.py "$ref_curve $echo_curve" >> "analyses/${ref_band}_≺_${echo_band}"
    scripts/extract_tables.pl

echo "Tabling PSD and time lags referred to ${ref_band} for $echo_band{${err_str}}."

    # Propagate tables into analyses/tables
    echoPSD_tabfile=analyses/tables/PSD_${echo_band}_\{${err_str}\}.tab
    refPSD_tabfile=analyses/tables/PSD_${ref_band}_\{${err_str}\}.tab
    timelag_tabfile=analyses/tables/timelag_${ref_band}_≺_${echo_band}_\{${err_str}\}.tab

    # Output curves to temporary files using perl script, move tables to
    # permanent location. This just assumes there are no conflicts.
    scripts/extract_tables.pl $analysis > /dev/null
    mv tmp.echoPSD $echoPSD_tabfile
    mv tmp.refPSD $refPSD_tabfile
    mv tmp.timelag $timelag_tabfile
done

rm tmp.*