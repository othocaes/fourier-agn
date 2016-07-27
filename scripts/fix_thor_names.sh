#!/usr/bin/env bash


for analysis in analyses/*A_*
do
    if [[ $analysis == "analyses/*A_*" ]]; then continue; fi
    echo Restoring unicode characters to $analysis.
    new_name=$(basename $analysis|
        #sed 's|_\([0-9]\+\)|\(𝛌＝\1|g'|
        sed 's|_|≺|'|
        sed 's|_|{|'|
        sed 's|_|;|'|
        #sed 's|_|}|'|
        sed 's|A|Å|'|
        sed 's|A|Å_|'|
        sed 's|≺|_≺_|'|
        sed 's|Δt|Δt＝|'|
        sed 's|err|σ∊|')}
    mv $analysis analyses/$new_name
done