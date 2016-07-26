#!/usr/bin/env bash


for analysis in analysis/*
do
    basename $analysis|
        sed 's|A_|Å\)|g'|
        sed 's|_\([0-9]+\)|\(𝛌＝\1|g'|
        sed 's|_|≻|'|
        sed 's|Δt|Δt＝|'
done