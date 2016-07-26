#!/usr/bin/env bash


for analysis in analyses/*
do
    new_name=$(basename $analysis|
        sed 's|A_|Å\)|g'|
        sed 's|_\([0-9]\+\)|\(𝛌＝\1|g'|
        sed 's|_|_≻_|'|
        sed 's|Δt|Δt＝|'|
        sed 's|err|σ∊|')_σ∊MC
    mv $analysis analyses/$new_name
done