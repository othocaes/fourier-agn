#!/usr/local/bin/bash

for analysis in analyses/*
do
    driving_name=$(basename $analysis|sed 's@[^≻]*≻\([^≻_]*\)_[^_]*@\1@')
    reproc_name=$(basename $analysis|sed 's@\([^≻]*\)≻[^≻_]*_[^_]*@\1@')
    # scripts/power_lag_plot.pl $analysis
    # mv tmp.sourcePSD analyses/tables/$reproc_namePSD.tab
    echo $driving_name with $reproc_name
done
