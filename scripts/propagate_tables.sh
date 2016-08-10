#!/usr/bin/env bash

mkdir -p analyses/tables

for analysis in analyses/*
do
    # Grab and determine labels of analyses, skip if over the same band.
    ref_band=$(basename $analysis|sed 's|\([^≺]*\)[_ ]≺[_ ][^≺_ ]*[_ ]{[^_ ]*}|\1|')
    echo_band=$(basename $analysis|sed 's|[^≺]*[_ ]≺[_ ]\([^≺_ ]*\)[_ ]{[^_ ]*}|\1|')
    if [[ $ref_band == $echo_band ]]; then continue; fi
    err_str=$(basename $analysis|sed 's|[^≺]*[_ ]≺[_ ][^≺_ ]*[_ ]{[^_ ]*;\(σ∊[CLM][MFC]\)}|\1|')

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